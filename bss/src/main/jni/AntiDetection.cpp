#include "AntiDetection.h"
#include "xhook/xhook.h"
#include <sys/ptrace.h>
#include <errno.h>
#include <thread>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>

// Flag untuk mengontrol proteksi
static bool g_antiDetectionActive = false;

// ============= HIDE FROM /proc/self/maps =============
static ssize_t (*orig_read)(int fd, void *buf, size_t count) = nullptr;
static FILE* (*orig_fopen)(const char *pathname, const char *mode) = nullptr;

// Hide TracerPid in /proc/self/status buffer to defeat anti-debug checks
static void hideProcStatusTracerPid(char* buf, ssize_t len) {
    char* p = buf;
    char* end = buf + len;
    while (p < end) {
        if (strncmp(p, "TracerPid:", 10) == 0) {
            char* nl = (char*)memchr(p, '\n', end - p);
            if (!nl) nl = end;
            char* val = p + 10;
            while (val < nl && (*val == '\t' || *val == ' ')) val++;
            if (val < nl) *val = '0';
            val++;
            while (val < nl) *val++ = ' ';
            break;
        }
        char* nl = (char*)memchr(p, '\n', end - p);
        if (!nl) break;
        p = nl + 1;
    }
}

ssize_t fake_read(int fd, void *buf, size_t count) {
    ssize_t ret = orig_read(fd, buf, count);

    if (!g_antiDetectionActive || ret <= 0 || !buf) return ret;

    char fdPath[64], linkTarget[256] = {0};
    snprintf(fdPath, sizeof(fdPath), "/proc/self/fd/%d", fd);
    ssize_t llen = readlink(fdPath, linkTarget, sizeof(linkTarget) - 1);
    if (llen <= 0) return ret;
    linkTarget[llen] = '\0';

    // Filter suspicious lib names from /proc/*/maps
    if (strstr(linkTarget, "maps")) {
        char* buffer = (char*)buf;
        static const char* filterStrings[] = {
            "libEncrypt.so", "libsubstrate", "libxposed", "libdobby",
            "libhookzz", "libshadowhook", "libwhale", "libepic",
            "libreframwork", "frida-agent", "frida-gadget", "frida-helper",
            "gum-js-loop", "gmain", "linjector", "libsecurity", "libanticheat",
            "libncsec", "libtencentc", "libbsec", "libQSEC", "libGameSec",
            "libprotect", "com.topjohnwu.magisk", "io.github.lsposed",
            "me.weishu.kernelsu", "io.github.vvb2060", "libxhook",
            "libNCSecurity", "libGameSDK", "libTPSdk", "libmsec",
            nullptr
        };
        char* bufEnd = buffer + ret;
        for (int i = 0; filterStrings[i]; i++) {
            size_t flen = strlen(filterStrings[i]);
            char* pos = buffer;
            while ((pos = (char*)memmem(pos, bufEnd - pos, filterStrings[i], flen)) != nullptr) {
                memset(pos, ' ', flen);
                pos += flen;
            }
        }
    }

    // Hide TracerPid in /proc/self/status and /proc/<pid>/status
    if (strstr(linkTarget, "status")) {
        hideProcStatusTracerPid((char*)buf, ret);
    }

    return ret;
}

FILE* fake_fopen(const char *pathname, const char *mode) {
    // PENTING: Jangan block file yang dibutuhkan oleh game/mod
    if (pathname && g_antiDetectionActive) {
        std::string path(pathname);
        
        // Hanya block jika jelas-jelas scan tool
        if (path.find("/proc/") != std::string::npos) {
            // Cek apakah ini dari external process
            // Jika dari process sendiri, ijinkan
            char selfPath[256];
            snprintf(selfPath, sizeof(selfPath), "/proc/%d/", getpid());
            
            // Jika bukan dari process sendiri, block
            if (path.find(selfPath) == std::string::npos) {
                ANTI_LOGI("Blocked external proc access: %s", pathname);
                errno = ENOENT;
                return nullptr;
            }
        }
    }
    
    return orig_fopen(pathname, mode);
}

void hideMapsEntries() {
    ANTI_LOGI("Installing maps hiding hooks...");
    
    orig_read = (ssize_t (*)(int, void*, size_t))dlsym(RTLD_DEFAULT, "read");
    orig_fopen = (FILE* (*)(const char*, const char*))dlsym(RTLD_DEFAULT, "fopen");
    
    xhook_register(".*\\.so$", "read", (void*)fake_read, (void**)&orig_read);
    xhook_register(".*\\.so$", "fopen", (void*)fake_fopen, (void**)&orig_fopen);
    xhook_refresh(0);
    
    ANTI_LOGI("Maps hiding installed");
}

// ============= ANTI ROOT DETECTION =============
static int (*orig_access)(const char *pathname, int mode) = nullptr;
static FILE* (*orig_popen)(const char *command, const char *type) = nullptr;

int fake_access(const char *pathname, int mode) {
    if (!g_antiDetectionActive || !pathname) {
        return orig_access(pathname, mode);
    }
    
    std::string path(pathname);
    
    // Enhanced: more complete root file paths
    std::vector<std::string> rootFiles = {
        "/system/bin/su",
        "/system/xbin/su",
        "/sbin/su",
        "/system/app/Superuser.apk",
        "/sbin/magisk",
        "/system/xbin/magisk",
        "/data/adb/magisk",
        "/data/adb/ksu",
        "/data/adb/apatch",
        "/system/xbin/daemonsu",
        "/system/xbin/busybox",
        "/system/etc/init.d/99SuperSUDaemon",
        "/dev/com.koushikdutta.superuser.daemon",
        "/system/app/SuperSU",
        "/system/app/Magisk",
        "/sbin/.magisk",
        "/sbin/.core/mirror",
        "/sbin/.core/img",
        "/data/data/com.noshufou.android.su",
        "/data/data/com.thirdparty.superuser",
        "/data/data/eu.chainfire.supersu",
        "/data/data/com.koushikdutta.superuser",
        "/data/data/com.topjohnwu.magisk"
    };
    
    for (const auto& rootFile : rootFiles) {
        if (path.find(rootFile) != std::string::npos) {
            ANTI_LOGI("Blocked root check: %s", pathname);
            errno = ENOENT;
            return -1;
        }
    }
    
    return orig_access(pathname, mode);
}

FILE* fake_popen(const char *command, const char *type) {
    if (!g_antiDetectionActive || !command) {
        return orig_popen(command, type);
    }
    
    std::string cmd(command);
    
    // Enhanced: block more suspicious commands
    if (cmd.find("su") != std::string::npos ||
        cmd.find("magisk") != std::string::npos ||
        cmd.find("which") != std::string::npos ||
        cmd.find("busybox") != std::string::npos ||
        cmd.find("frida") != std::string::npos) {
        ANTI_LOGI("Blocked popen command: %s", command);
        return nullptr;
    }
    
    return orig_popen(command, type);
}

void hideRootAccess() {
    ANTI_LOGI("Installing root hiding hooks...");
    
    orig_access = (int (*)(const char*, int))dlsym(RTLD_DEFAULT, "access");
    orig_popen = (FILE* (*)(const char*, const char*))dlsym(RTLD_DEFAULT, "popen");
    
    xhook_register(".*\\.so$", "access", (void*)fake_access, (void**)&orig_access);
    xhook_register(".*\\.so$", "popen", (void*)fake_popen, (void**)&orig_popen);
    xhook_refresh(0);
    
    ANTI_LOGI("Root hiding installed");
}

// ============= HIDE XPOSED/MAGISK =============
static int (*orig_stat)(const char *pathname, struct stat *statbuf) = nullptr;
static int (*orig_lstat)(const char *pathname, struct stat *statbuf) = nullptr;

// Shared list of frameworks to hide
static bool isFrameworkPath(const std::string& path) {
    std::vector<std::string> frameworks = {
        "XposedBridge",
        "EdXposed",
        "LSPosed",
        "frida",
        "magisk",
        "Magisk",
        "/data/adb",
        "/sbin/.magisk",
        "/sbin/.core",
        "substrate",
        "riru",
        "zygisk",
        "ksu",
        "apatch",
        "busybox"
    };
    
    for (const auto& framework : frameworks) {
        if (path.find(framework) != std::string::npos) {
            return true;
        }
    }
    return false;
}

int fake_stat(const char *pathname, struct stat *statbuf) {
    if (!g_antiDetectionActive || !pathname) {
        return orig_stat(pathname, statbuf);
    }
    
    if (isFrameworkPath(std::string(pathname))) {
        ANTI_LOGI("Blocked stat framework check: %s", pathname);
        errno = ENOENT;
        return -1;
    }
    
    return orig_stat(pathname, statbuf);
}

// Bug fix: also hook lstat — anti-cheat uses lstat too on modern Android
int fake_lstat(const char *pathname, struct stat *statbuf) {
    if (!g_antiDetectionActive || !pathname) {
        return orig_lstat(pathname, statbuf);
    }
    
    if (isFrameworkPath(std::string(pathname))) {
        ANTI_LOGI("Blocked lstat framework check: %s", pathname);
        errno = ENOENT;
        return -1;
    }
    
    return orig_lstat(pathname, statbuf);
}

void hideFrameworks() {
    ANTI_LOGI("Installing framework hiding hooks...");
    
    orig_stat = (int (*)(const char*, struct stat*))dlsym(RTLD_DEFAULT, "stat");
    orig_lstat = (int (*)(const char*, struct stat*))dlsym(RTLD_DEFAULT, "lstat");
    
    xhook_register(".*\\.so$", "stat", (void*)fake_stat, (void**)&orig_stat);
    // Bug fix: also register lstat hook
    xhook_register(".*\\.so$", "lstat", (void*)fake_lstat, (void**)&orig_lstat);
    xhook_refresh(0);
    
    ANTI_LOGI("Framework hiding installed (stat + lstat)");
}

// ============= ANTI DEBUGGING =============
void antiDebugProtection() {
    // Claim the ptrace slot — prevents another debugger from attaching
    ptrace(PTRACE_TRACEME, 0, 0, 0);

    // Background thread: periodically reclaim ptrace slot and check for
    // external debugger attachment via /proc/self/status TracerPid.
    // Runs quietly with no logging to avoid leaving traces in logcat.
    std::thread([]() {
        while (true) {
            // Re-issue TRACEME to keep the slot claimed after any detach
            ptrace(PTRACE_TRACEME, 0, 0, 0);

            // Read TracerPid from /proc/self/status silently
            FILE* f = fopen("/proc/self/status", "r");
            if (f) {
                char line[128];
                while (fgets(line, sizeof(line), f)) {
                    if (strncmp(line, "TracerPid:", 10) == 0) {
                        int tracerPid = 0;
                        sscanf(line + 10, "%d", &tracerPid);
                        if (tracerPid != 0) {
                            // Debugger detected — kill our own process cleanly
                            // This is safer than abort() as it avoids crash reporting
                            kill(getpid(), SIGKILL);
                        }
                        break;
                    }
                }
                fclose(f);
            }

            sleep(5);
        }
    }).detach();
}

// ============= SPOOF SYSTEM PROPERTIES =============
static int (*orig_system_property_get)(const char *name, char *value) = nullptr;

int fake_system_property_get(const char *name, char *value) {
    if (!g_antiDetectionActive || !name) {
        return orig_system_property_get(name, value);
    }
    
    std::string propName(name);
    
    if (propName == "ro.debuggable") {
        strcpy(value, "0");
        return strlen(value);
    }
    
    if (propName == "ro.secure") {
        strcpy(value, "1");
        return strlen(value);
    }
    
    // Enhancement: also spoof build tags — anti-cheat checks these
    if (propName == "ro.build.tags") {
        strcpy(value, "release-keys");
        return strlen(value);
    }
    
    // Enhancement: spoof build type to look like production
    if (propName == "ro.build.type") {
        strcpy(value, "user");
        return strlen(value);
    }
    
    // Enhancement: block selinux permissive detection
    if (propName == "ro.build.selinux") {
        strcpy(value, "1");
        return strlen(value);
    }

    // Spoof Android version fingerprint to a clean stock build
    if (propName == "ro.build.fingerprint") {
        strcpy(value, "google/walleye/walleye:8.1.0/OPM1.171019.011/4448085:user/release-keys");
        return strlen(value);
    }

    // Hide developer/test flags
    if (propName == "ro.test_harness") {
        strcpy(value, "0");
        return strlen(value);
    }
    if (propName == "ro.monkey") {
        strcpy(value, "0");
        return strlen(value);
    }

    // SafetyNet / Play Integrity checks
    if (propName == "ro.boot.veritymode") {
        strcpy(value, "enforcing");
        return strlen(value);
    }
    if (propName == "ro.boot.verifiedbootstate") {
        strcpy(value, "green");
        return strlen(value);
    }
    if (propName == "ro.boot.flash.locked") {
        strcpy(value, "1");
        return strlen(value);
    }
    if (propName == "ro.boot.warranty_bit") {
        strcpy(value, "0");
        return strlen(value);
    }

    // Hide rooted kernel
    if (propName == "ro.bootimage.build.type") {
        strcpy(value, "user");
        return strlen(value);
    }
    if (propName == "ro.product.build.type") {
        strcpy(value, "user");
        return strlen(value);
    }

    // Hide USB debugging
    if (propName == "service.adb.root") {
        strcpy(value, "0");
        return strlen(value);
    }
    if (propName == "ro.adb.secure") {
        strcpy(value, "1");
        return strlen(value);
    }

    return orig_system_property_get(name, value);
}

void spoofSystemProperties() {
    ANTI_LOGI("Installing property spoofing...");
    
    void* libcutils = dlopen("libcutils.so", RTLD_NOW);
    if (libcutils) {
        orig_system_property_get = (int (*)(const char*, char*))
            dlsym(libcutils, "__system_property_get");
        
        if (orig_system_property_get) {
            xhook_register(".*\\.so$", "__system_property_get", 
                          (void*)fake_system_property_get, 
                          (void**)&orig_system_property_get);
            xhook_refresh(0);
        }
        // Bug fix: close the handle to prevent memory leak
        dlclose(libcutils);
    }
    
    ANTI_LOGI("Property spoofing installed:");
    ANTI_LOGI("  + ro.debuggable, ro.secure, ro.adb.secure");
    ANTI_LOGI("  + ro.build.tags, ro.build.type, ro.build.selinux");
    ANTI_LOGI("  + ro.build.fingerprint (spoofed to stock Pixel)");
    ANTI_LOGI("  + ro.test_harness, ro.monkey");
    ANTI_LOGI("  + ro.boot.veritymode, ro.boot.verifiedbootstate");
    ANTI_LOGI("  + ro.boot.flash.locked, ro.boot.warranty_bit");
    ANTI_LOGI("  + ro.bootimage.build.type, ro.product.build.type");
    ANTI_LOGI("  + service.adb.root");
}

// ============= OPENAT HOOK (hide framework files on modern Android) =============
static int (*orig_openat)(int dirfd, const char *pathname, int flags, ...) = nullptr;

int fake_openat(int dirfd, const char *pathname, int flags, ...) {
    if (g_antiDetectionActive && pathname) {
        std::string path(pathname);
        if (isFrameworkPath(path)) {
            errno = ENOENT;
            return -1;
        }
        // Hide /proc/self/maps and /proc/<pid>/maps from external scanners
        if (path.find("/proc/") != std::string::npos &&
            path.find("/maps") != std::string::npos) {
            // Only deny if it's not our own pid
            char selfMaps[64];
            snprintf(selfMaps, sizeof(selfMaps), "/proc/%d/maps", getpid());
            if (path != selfMaps && path != "/proc/self/maps") {
                errno = EACCES;
                return -1;
            }
        }
    }
    // Variadic: mode arg only matters for O_CREAT; pass 0 if not creating
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, int);
        va_end(args);
    }
    return orig_openat(dirfd, pathname, flags, mode);
}

// ============= HOOK ANTI-CHEAT =============
static void* (*orig_dlopen)(const char *filename, int flags) = nullptr;

void* fake_dlopen(const char *filename, int flags) {
    if (filename && g_antiDetectionActive) {
        std::string fname(filename);
        static const char* blockedLibs[] = {
            "libsecurity.so", "libanticheat.so", "libprotect.so",
            "libcheck.so",    "libverify.so",    "libtp.so",
            "libTPSdk.so",    "libmsec.so",      "libNCSecurity.so",
            "libGameSDK.so",  "libQSEC.so",      "libbsec.so",
            "libncsec.so",    "libGameSec.so",   "libscan.so",
            nullptr
        };
        for (int i = 0; blockedLibs[i]; i++) {
            if (fname.find(blockedLibs[i]) != std::string::npos) {
                errno = ENOENT;
                return nullptr;
            }
        }
    }
    return orig_dlopen(filename, flags);
}

void hookAntiCheatFunctions() {
    orig_dlopen = (void* (*)(const char*, int))dlsym(RTLD_DEFAULT, "dlopen");
    xhook_register(".*\\.so$", "dlopen", (void*)fake_dlopen, (void**)&orig_dlopen);

    orig_openat = (int (*)(int, const char*, int, ...))dlsym(RTLD_DEFAULT, "openat");
    xhook_register(".*\\.so$", "openat", (void*)fake_openat, (void**)&orig_openat);

    xhook_refresh(0);
}

// Stubs untuk fungsi yang tidak digunakan
void hideMemoryRegions() {
    ANTI_LOGI("Memory hiding skipped (not needed)");
}

void obfuscateMemoryStrings() {
    ANTI_LOGI("String obfuscation skipped");
}

void hideFromPackageList() {
    ANTI_LOGI("Package hiding skipped");
}

// ============= INITIALIZE (SAFE MODE) =============
void initAntiDetection() {
    ANTI_LOGI("========================================");
    ANTI_LOGI("Initializing Anti-Detection (Safe Mode)");
    ANTI_LOGI("========================================");
    
    // Delay untuk memastikan game sudah load
    sleep(2);
    
    // Install protections (non-aggressive)
    hideMapsEntries();
    hideRootAccess();
    hideFrameworks();
    antiDebugProtection();
    spoofSystemProperties();
    hookAntiCheatFunctions();
    
    // Aktifkan proteksi setelah semua hook terpasang
    g_antiDetectionActive = true;
    
    ANTI_LOGI("========================================");
    ANTI_LOGI("Anti-Detection Active (Safe Mode)");
    ANTI_LOGI("  + fake_read: /proc/maps filter (all occurrences)");
    ANTI_LOGI("  + fake_access: 23 root paths blocked");
    ANTI_LOGI("  + fake_stat + fake_lstat: framework hiding");
    ANTI_LOGI("  + property spoof: debuggable/secure/tags/type/selinux");
    ANTI_LOGI("  + dlopen: security lib blocking enabled");
    ANTI_LOGI("========================================");
}
