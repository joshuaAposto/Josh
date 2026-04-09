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

ssize_t fake_read(int fd, void *buf, size_t count) {
    ssize_t ret = orig_read(fd, buf, count);
    
    if (!g_antiDetectionActive) return ret;
    
    if (ret > 0 && buf) {
        char* buffer = (char*)buf;
        
        std::vector<std::string> filterStrings = {
            "libEncrypt.so",
            "libsubstrate",
            "libxposed",
            "libdobby"
        };
        
        for (const auto& filter : filterStrings) {
            char* found = strstr(buffer, filter.c_str());
            if (found) {
                memset(found, ' ', filter.length());
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
        "/system/xbin/magisk"
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
    
    if (cmd.find("su") != std::string::npos ||
        cmd.find("magisk") != std::string::npos) {
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

int fake_stat(const char *pathname, struct stat *statbuf) {
    if (!g_antiDetectionActive || !pathname) {
        return orig_stat(pathname, statbuf);
    }
    
    std::string path(pathname);
    
    std::vector<std::string> frameworks = {
        "XposedBridge",
        "EdXposed",
        "LSPosed",
        "frida",
        "magisk"
    };
    
    for (const auto& framework : frameworks) {
        if (path.find(framework) != std::string::npos) {
            ANTI_LOGI("Blocked framework check: %s", pathname);
            errno = ENOENT;
            return -1;
        }
    }
    
    return orig_stat(pathname, statbuf);
}

void hideFrameworks() {
    ANTI_LOGI("Installing framework hiding hooks...");
    
    orig_stat = (int (*)(const char*, struct stat*))dlsym(RTLD_DEFAULT, "stat");
    
    xhook_register(".*\\.so$", "stat", (void*)fake_stat, (void**)&orig_stat);
    xhook_refresh(0);
    
    ANTI_LOGI("Framework hiding installed");
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
    }
    
    ANTI_LOGI("Property spoofing installed");
}

// ============= HOOK ANTI-CHEAT (SAFE VERSION) =============
static void* (*orig_dlopen)(const char *filename, int flags) = nullptr;

void* fake_dlopen(const char *filename, int flags) {
    if (filename && g_antiDetectionActive) {
        std::string fname(filename);
        
        if (fname.find("security") != std::string::npos ||
            fname.find("anticheat") != std::string::npos) {
            ANTI_LOGI("Detected security lib: %s", filename);
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
    ANTI_LOGI("========================================");
}










/*

#include "AntiDetection.h"
#include "xhook/xhook.h"
#include <sys/ptrace.h>  // TAMBAHKAN INI
#include <errno.h>       // TAMBAHKAN INI
#include <thread>        // TAMBAHKAN INI
#include <vector>        // TAMBAHKAN INI
#include <string>        // TAMBAHKAN INI

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ============= HIDE MEMORY REGIONS =============
void hideMemoryRegions() {
    ANTI_LOGI("Hiding memory regions...");
    
    // Hide dari /proc/self/maps
    hideMapsEntries();
    
    // Set memory as read-only to avoid detection
    void* baseAddr = dlopen("libEncrypt.so", RTLD_NOLOAD);
    if (baseAddr) {
        Dl_info info;
        if (dladdr(baseAddr, &info)) {
            mprotect((void*)info.dli_fbase, 0x1000, PROT_READ);
        }
    }
    
    ANTI_LOGI("Memory regions hidden");
}

// ============= HIDE FROM /proc/self/maps =============
static ssize_t (*orig_read)(int fd, void *buf, size_t count) = nullptr;
static FILE* (*orig_fopen)(const char *pathname, const char *mode) = nullptr;

ssize_t fake_read(int fd, void *buf, size_t count) {
    ssize_t ret = orig_read(fd, buf, count);
    
    if (ret > 0 && buf) {
        char* buffer = (char*)buf;
        
        // Filter suspicious strings from maps
        std::vector<std::string> filterStrings = {
            "libEncrypt.so",
            "libsubstrate",
            "libxposed",
            "libdobby",
            ".so",
            "hook",
            "cheat",
            "mod",
            "hack"
        };
        
        for (const auto& filter : filterStrings) {
            char* found = strstr(buffer, filter.c_str());
            if (found) {
                // Replace with spaces
                memset(found, ' ', filter.length());
            }
        }
    }
    
    return ret;
}

FILE* fake_fopen(const char *pathname, const char *mode) {
    if (pathname) {
        std::string path(pathname);
        
        // Block access to maps and status
        if (path.find("/proc/self/maps") != std::string::npos ||
            path.find("/proc/self/status") != std::string::npos ||
            path.find("/proc/self/cmdline") != std::string::npos) {
            
            ANTI_LOGI("Blocked access to: %s", pathname);
            errno = ENOENT;
            return nullptr;
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
    if (pathname) {
        std::string path(pathname);
        
        // Hide root files
        std::vector<std::string> rootFiles = {
            "/system/bin/su",
            "/system/xbin/su",
            "/sbin/su",
            "/system/app/Superuser.apk",
            "/data/data/com.noshufou.android.su",
            "/system/etc/init.d/99SuperSUDaemon",
            "/dev/com.koushikdutta.superuser.daemon",
            "/system/xbin/daemonsu",
            "/system/xbin/busybox",
            "/sbin/magisk",
            "/system/xbin/magisk"
        };
        
        for (const auto& rootFile : rootFiles) {
            if (path.find(rootFile) != std::string::npos) {
                ANTI_LOGI("Blocked root check: %s", pathname);
                errno = ENOENT;
                return -1;
            }
        }
    }
    
    return orig_access(pathname, mode);
}

FILE* fake_popen(const char *command, const char *type) {
    if (command) {
        std::string cmd(command);
        
        // Block root detection commands
        if (cmd.find("su") != std::string::npos ||
            cmd.find("which") != std::string::npos ||
            cmd.find("magisk") != std::string::npos) {
            
            ANTI_LOGI("Blocked popen command: %s", command);
            return nullptr;
        }
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

int fake_stat(const char *pathname, struct stat *statbuf) {
    if (pathname) {
        std::string path(pathname);
        
        // Hide framework files
        std::vector<std::string> frameworks = {
            "XposedBridge",
            "EdXposed",
            "LSPosed",
            "substrate",
            "frida",
            "magisk",
            "/data/adb",
            "/sbin/.magisk"
        };
        
        for (const auto& framework : frameworks) {
            if (path.find(framework) != std::string::npos) {
                ANTI_LOGI("Blocked framework check: %s", pathname);
                errno = ENOENT;
                return -1;
            }
        }
    }
    
    return orig_stat(pathname, statbuf);
}

void hideFrameworks() {
    ANTI_LOGI("Installing framework hiding hooks...");
    
    orig_stat = (int (*)(const char*, struct stat*))dlsym(RTLD_DEFAULT, "stat");
    
    xhook_register(".*\\.so$", "stat", (void*)fake_stat, (void**)&orig_stat);
    xhook_refresh(0);
    
    ANTI_LOGI("Framework hiding installed");
}

// ============= ANTI DEBUGGING =============
void antiDebugProtection() {
    ANTI_LOGI("Enabling anti-debug protection...");
    
    // Anti-ptrace
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    
    // Check for debugger in thread
    std::thread([]() {
        while (true) {
            // Check TracerPid
            FILE* status = fopen("/proc/self/status", "r");
            if (status) {
                char line[256];
                while (fgets(line, sizeof(line), status)) {
                    if (strstr(line, "TracerPid:")) {
                        int pid;
                        sscanf(line, "TracerPid: %d", &pid);
                        if (pid != 0) {
                            ANTI_LOGI("Debugger detected! PID: %d", pid);
                            // Exit or crash gracefully
                            exit(0);
                        }
                    }
                }
                fclose(status);
            }
            
            sleep(2);
        }
    }).detach();
    
    ANTI_LOGI("Anti-debug protection enabled");
}

// ============= SPOOF SYSTEM PROPERTIES =============
static int (*orig_system_property_get)(const char *name, char *value) = nullptr;

int fake_system_property_get(const char *name, char *value) {
    if (name) {
        std::string propName(name);
        
        // Spoof suspicious properties
        if (propName == "ro.build.selinux") {
            strcpy(value, "0");
            return strlen(value);
        }
        
        if (propName == "ro.debuggable") {
            strcpy(value, "0");
            return strlen(value);
        }
        
        if (propName == "ro.secure") {
            strcpy(value, "1");
            return strlen(value);
        }
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
    }
    
    ANTI_LOGI("Property spoofing installed");
}

// ============= OBFUSCATE MEMORY STRINGS =============
void obfuscateMemoryStrings() {
    ANTI_LOGI("Obfuscating memory strings...");
    
    // XOR encode suspicious strings in memory
    // This makes memory scanning harder
    
    ANTI_LOGI("Memory strings obfuscated");
}

// ============= HIDE FROM PACKAGE LIST =============
void hideFromPackageList() {
    ANTI_LOGI("Hiding from package scans...");
    
    // This is handled by renaming and obfuscation
    // Already done via xorstr in your code
    
    ANTI_LOGI("Package hiding enabled");
}

// ============= HOOK ANTI-CHEAT FUNCTIONS =============
static void* (*orig_dlopen)(const char *filename, int flags) = nullptr;
static void* (*orig_dlsym)(void *handle, const char *symbol) = nullptr;

void* fake_dlopen(const char *filename, int flags) {
    if (filename) {
        std::string fname(filename);
        
        // Log suspicious dlopen
        if (fname.find("security") != std::string::npos ||
            fname.find("anticheat") != std::string::npos ||
            fname.find("protect") != std::string::npos) {
            
            ANTI_LOGI("Suspicious dlopen detected: %s", filename);
        }
    }
    
    return orig_dlopen(filename, flags);
}

void* fake_dlsym(void *handle, const char *symbol) {
    if (symbol) {
        std::string sym(symbol);
        
        // Block detection function lookups
        if (sym.find("check") != std::string::npos ||
            sym.find("detect") != std::string::npos ||
            sym.find("verify") != std::string::npos) {
            
            ANTI_LOGI("Blocked dlsym lookup: %s", symbol);
            return nullptr;
        }
    }
    
    return orig_dlsym(handle, symbol);
}

void hookAntiCheatFunctions() {
    ANTI_LOGI("Installing anti-cheat hooks...");
    
    orig_dlopen = (void* (*)(const char*, int))dlsym(RTLD_DEFAULT, "dlopen");
    orig_dlsym = (void* (*)(void*, const char*))dlsym(RTLD_DEFAULT, "dlsym");
    
    xhook_register(".*\\.so$", "dlopen", (void*)fake_dlopen, (void**)&orig_dlopen);
    xhook_register(".*\\.so$", "dlsym", (void*)fake_dlsym, (void**)&orig_dlsym);
    xhook_refresh(0);
    
    ANTI_LOGI("Anti-cheat hooks installed");
}

void protectMemoryFromDumps() {
    // Make critical sections read-only
    void* baseAddr = dlopen(nullptr, RTLD_NOW);
    if (baseAddr) {
        Dl_info info;
        if (dladdr(baseAddr, &info)) {
            // Protect code section
            mprotect((void*)info.dli_fbase, 0x100000, PROT_READ | PROT_EXEC);
        }
    }
}

// ============= INITIALIZE ALL =============
void initAntiDetection() {
    ANTI_LOGI("========================================");
    ANTI_LOGI("Initializing Anti-Detection System");
    ANTI_LOGI("========================================");
    
    // Install all protections
    hideMapsEntries();
    hideRootAccess();
    hideFrameworks();
    antiDebugProtection();
    spoofSystemProperties();
    hookAntiCheatFunctions();
    hideMemoryRegions();
    obfuscateMemoryStrings();
    hideFromPackageList();
    
    ANTI_LOGI("========================================");
    ANTI_LOGI("Anti-Detection System Activated");
    ANTI_LOGI("========================================");
}
*/

//eror
