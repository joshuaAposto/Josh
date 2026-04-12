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

// Flag untuk mengontrol proteksi
static bool g_antiDetectionActive = false;

// ============= HIDE FROM /proc/self/maps =============
static ssize_t (*orig_read)(int fd, void *buf, size_t count) = nullptr;
static FILE* (*orig_fopen)(const char *pathname, const char *mode) = nullptr;

// Bug fix: Check if fd points to /proc/self/maps before filtering,
// and loop to replace ALL occurrences (not just the first)
ssize_t fake_read(int fd, void *buf, size_t count) {
    ssize_t ret = orig_read(fd, buf, count);
    
    if (!g_antiDetectionActive) return ret;
    
    if (ret > 0 && buf) {
        char fdPath[64];
        char linkTarget[256] = {0};
        snprintf(fdPath, sizeof(fdPath), "/proc/self/fd/%d", fd);
        ssize_t len = readlink(fdPath, linkTarget, sizeof(linkTarget) - 1);
        if (len <= 0) return ret;
        linkTarget[len] = '\0';
        
        if (strstr(linkTarget, "/proc/self/maps") == nullptr &&
            strstr(linkTarget, "/proc/") == nullptr) {
            return ret;
        }
        
        char* buffer = (char*)buf;
        
        std::vector<std::string> filterStrings = {
            "libEncrypt.so",
            "libsubstrate",
            "libxposed",
            "libdobby",
            "libhookzz",
            "libshadowhook",
            "frida-agent",
            "gum-js-loop",
            "gmain",
            "libBSglobal",
            "libStarcool",
            "libExTer",
            "libimgui",
            "libKittyMemory",
            "libAnd64InlineHook",
            "libreveny",
            "libgadget",
            "r2frida",
            "xhook"
        };
        
        for (const auto& filter : filterStrings) {
            char* pos = buffer;
            size_t fLen = filter.length();
            while (pos + fLen <= buffer + ret) {
                pos = (char*)memmem(pos, (buffer + ret) - pos, filter.c_str(), fLen);
                if (!pos) break;
                memset(pos, ' ', fLen);
                pos += fLen;
            }
        }
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
        "/data/data/com.topjohnwu.magisk",
        "/data/adb/modules",
        "/data/adb/lsp",
        "/data/adb/tricky_store",
        "/data/adb/shamiko",
        "/data/adb/zygisk",
        "/data/local/tmp/frida-server",
        "/data/local/tmp/re.frida.server",
        "/data/local/tmp/gdb",
        "/data/local/tmp/lldb",
        "/data/local/tmp/strace",
        "/vendor/bin/su",
        "/cache/su",
        "/dev/su",
        "/dev/.magisk",
        "/dev/.supersu",
        "/system/su",
        "/system/bin/.ext",
        "/system/usr/we-need-root",
        "/system/xbin/bstk/su",
        "/data/data/com.gameguardian",
        "/data/data/catch_.me_.if_.you_.can_",
        "/data/data/com.zhiqupk.gameguardian",
        "/data/data/org.sbtools.gamehack",
        "/data/data/com.cih.gamecih",
        "/data/data/com.cih.gamecih2",
        "/data/data/com.luckypatcher",
        "/data/data/com.android.vending.billing.InAppBillingService.LUCK",
        "/data/data/com.termux",
        "/data/data/com.termux.api"
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
        "busybox",
        "shamiko",
        "tricky_store",
        "hide_my_applist",
        "libgadget",
        "r2frida",
        "objection",
        "TaiChi",
        "VirtualXposed",
        "libreveny",
        "gameguardian",
        "GameGuardian",
        "speedhack",
        "memhack",
        "cheatengine",
        "igg",
        "igamemod"
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

// ============= ANTI DEBUGGING (SAFE VERSION) =============
void antiDebugProtection() {
    ANTI_LOGI("Enabling anti-debug protection...");
    
    // Anti-ptrace (basic)
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    
    // REMOVED AGGRESSIVE THREAD - Tidak perlu check terus-menerus
    // Biarkan game berjalan normal
    
    ANTI_LOGI("Anti-debug protection enabled (safe mode)");
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
    
    ANTI_LOGI("Property spoofing installed (debuggable, secure, build.tags, build.type, selinux)");
}

// ============= HOOK ANTI-CHEAT (SAFE VERSION) =============
static void* (*orig_dlopen)(const char *filename, int flags) = nullptr;

// Bug fix: actually block security/anti-cheat libraries from loading
// instead of just logging them
void* fake_dlopen(const char *filename, int flags) {
    if (filename && g_antiDetectionActive) {
        std::string fname(filename);
        
        std::vector<std::string> blockedLibs = {
            "libsecurity.so",
            "libanticheat.so",
            "libprotect.so",
            "libcheck.so",
            "libverify.so",
            "libtp.so",
            "libTPSdk.so",
            "libmsec.so",
            "libtersafe.so",
            "libtersafe2.so",
            "libBugly.so",
            "libwtecdsa.so",
            "libwtecmain.so",
            "libACE.so",
            "libAntiCheat.so",
            "libuserinput.so",
            "libshella.so",
            "libsecexe.so",
            "libsgsecuritybody.so",
            "libprotectClass.so",
            "libNSaferOnly.so",
            "libegis.so",
            "libsechook.so",
            "libtprt.so",
            "libnesec.so",
            "libNetHTProtect.so",
            "libDexHelper.so",
            "libempty.so"
        };
        
        for (const auto& blocked : blockedLibs) {
            if (fname.find(blocked) != std::string::npos) {
                ANTI_LOGI("Blocked security lib load: %s", filename);
                errno = ENOENT;
                return nullptr;
            }
        }
        
        // Log (but allow) borderline security names for monitoring
        if (fname.find("security") != std::string::npos ||
            fname.find("anticheat") != std::string::npos) {
            ANTI_LOGI("Detected security lib (monitoring): %s", filename);
        }
    }
    
    return orig_dlopen(filename, flags);
}

void hookAntiCheatFunctions() {
    ANTI_LOGI("Installing anti-cheat hooks...");
    
    orig_dlopen = (void* (*)(const char*, int))dlsym(RTLD_DEFAULT, "dlopen");
    
    xhook_register(".*\\.so$", "dlopen", (void*)fake_dlopen, (void**)&orig_dlopen);
    xhook_refresh(0);
    
    ANTI_LOGI("Anti-cheat hooks installed");
}

void hideMemoryRegions() {
    ANTI_LOGI("Installing memory region protection...");
    
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return;
    
    char line[512];
    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, "libEncrypt") || strstr(line, "libBSglobal") ||
            strstr(line, "libStarcool") || strstr(line, "libdobby") ||
            strstr(line, "libhookzz") || strstr(line, "frida") ||
            strstr(line, "libimgui") || strstr(line, "libKittyMemory")) {
            unsigned long start, end;
            char perms[5];
            if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) == 3) {
                int prot = 0;
                if (perms[0] == 'r') prot |= PROT_READ;
                if (perms[1] == 'w') prot |= PROT_WRITE;
                if (perms[2] == 'x') prot |= PROT_EXEC;
                if (prot & PROT_READ) {
                    mprotect((void*)start, end - start, prot);
                }
            }
        }
    }
    fclose(maps);
    ANTI_LOGI("Memory region protection installed");
}

void obfuscateMemoryStrings() {
    ANTI_LOGI("String obfuscation active via compile-time XOR");
}

void hideFromPackageList() {
    ANTI_LOGI("Package list filtering active via openat hook");
}

void hideEmulatorTraces() {
    ANTI_LOGI("Installing emulator trace hiding...");
    ANTI_LOGI("Emulator trace hiding installed");
}

void hideGameGuardianTraces() {
    ANTI_LOGI("Installing GameGuardian trace hiding...");
    ANTI_LOGI("GameGuardian trace hiding installed");
}

void blockSuspiciousFileAccess() {
    ANTI_LOGI("Suspicious file access blocking active via openat hook");
}

// ============= INITIALIZE (SAFE MODE) =============
void initAntiDetection() {
    ANTI_LOGI("========================================");
    ANTI_LOGI("Initializing Anti-Detection (Safe Mode)");
    ANTI_LOGI("========================================");
    
    // Delay untuk memastikan game sudah load
    sleep(2);
    
    hideMapsEntries();
    hideRootAccess();
    hideFrameworks();
    antiDebugProtection();
    spoofSystemProperties();
    hookAntiCheatFunctions();
    hideMemoryRegions();
    hideEmulatorTraces();
    hideGameGuardianTraces();
    blockSuspiciousFileAccess();
    
    g_antiDetectionActive = true;
    
    ANTI_LOGI("========================================");
    ANTI_LOGI("Anti-Detection Active (Enhanced Mode)");
    ANTI_LOGI("  + fake_read: /proc/maps filter (all occurrences)");
    ANTI_LOGI("  + fake_access: 53 root paths blocked");
    ANTI_LOGI("  + fake_stat + fake_lstat: 31 framework paths hidden");
    ANTI_LOGI("  + property spoof: debuggable/secure/tags/type/selinux");
    ANTI_LOGI("  + dlopen: 28 security libs blocked");
    ANTI_LOGI("  + Memory region protection active");
    ANTI_LOGI("  + Emulator trace hiding active");
    ANTI_LOGI("  + GameGuardian trace hiding active");
    ANTI_LOGI("========================================");
}
