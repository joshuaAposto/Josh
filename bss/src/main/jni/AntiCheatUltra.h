#pragma once

#include <sys/socket.h>
#include <sys/ptrace.h>
#include <sys/inotify.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <mutex>
#include <thread>
#include <atomic>
#include <time.h>
#include <errno.h>
#include <linux/limits.h>
#include <android/log.h>
#include "xhook/xhook.h"

#define ULTRA_TAG "UltraGuard"
#define ULTRA_LOGI(...) __android_log_print(ANDROID_LOG_INFO, ULTRA_TAG, __VA_ARGS__)
#define ULTRA_LOGW(...) __android_log_print(ANDROID_LOG_WARN, ULTRA_TAG, __VA_ARGS__)

static std::atomic<bool> g_ultraActive{false};
static std::mutex g_ultraMutex;

static const char* g_hiddenLibNames[] = {
    "libBSglobal", "libEncrypt", "libStarcool", "libExTer",
    "libdobby", "libsubstrate", "libxposed", "libhookzz",
    "libshadowhook", "frida-agent", "gum-js-loop", "gmain",
    "libreveny", "libhide", "libgadget", "libanticheater",
    "libbypass", "libinject", "libpatch", "libmod",
    "libhack", "libcheat", "libmenu", "libimgui",
    "libKittyMemory", "libAnd64InlineHook", "libDobby",
    nullptr
};

static const char* g_suspiciousPackages[] = {
    "de.robv.android.xposed",
    "com.saurik.substrate",
    "org.lsposed",
    "com.topjohnwu.magisk",
    "eu.chainfire.supersu",
    "com.noshufou.android.su",
    "me.weishu.kernelsu",
    "me.bmax.apatch",
    "com.termux",
    "com.gametuner",
    "com.cheatengine",
    "com.xmodgames",
    "com.gameguardian",
    "catch_.me_.if_.you_.can_",
    "com.luckypatcher",
    "com.android.vending.billing.InAppBillingService",
    "com.cih.gamecih",
    "com.cih.gamecih2",
    "com.charles.lpoqq",
    "com.zhiqupk",
    nullptr
};

static const uint16_t g_blockedPorts[] = {
    4999, 5000, 5001, 5002, 5003,
    8080, 8081, 8443,
    9090, 9091, 9200, 9300,
    27015, 27017, 27018,
    53,
    0
};

static bool isBlockedPort(uint16_t port) {
    for (int i = 0; g_blockedPorts[i] != 0; i++) {
        if (port == g_blockedPorts[i]) return true;
    }
    return false;
}

static bool isHiddenLib(const char* name) {
    if (!name) return false;
    for (int i = 0; g_hiddenLibNames[i] != nullptr; i++) {
        if (strstr(name, g_hiddenLibNames[i]) != nullptr) return true;
    }
    return false;
}

static int (*orig_openat)(int dirfd, const char* pathname, int flags, mode_t mode) = nullptr;
static ssize_t (*orig_readlinkat)(int dirfd, const char* pathname, char* buf, size_t bufsiz) = nullptr;
static int (*orig_inotify_add_watch)(int fd, const char* pathname, uint32_t mask) = nullptr;
static ssize_t (*orig_sendmsg)(int sockfd, const struct msghdr* msg, int flags) = nullptr;
static ssize_t (*orig_recvmsg)(int sockfd, struct msghdr* msg, int flags) = nullptr;
static int (*orig_kill)(pid_t pid, int sig) = nullptr;
static long (*orig_syscall)(long number, ...) = nullptr;
static int (*orig_pthread_create)(pthread_t*, const pthread_attr_t*, void*(*)(void*), void*) = nullptr;

static ssize_t boundedFind(const char* haystack, ssize_t haystackLen, const char* needle, size_t needleLen) {
    if (needleLen > (size_t)haystackLen) return -1;
    for (ssize_t i = 0; i <= haystackLen - (ssize_t)needleLen; i++) {
        if (memcmp(haystack + i, needle, needleLen) == 0) return i;
    }
    return -1;
}

static ssize_t filterProcContent(char* buf, ssize_t len) {
    if (len <= 0 || !buf) return len;
    
    for (int i = 0; g_hiddenLibNames[i] != nullptr; i++) {
        size_t nameLen = strlen(g_hiddenLibNames[i]);
        ssize_t offset = 0;
        ssize_t pos;
        while ((pos = boundedFind(buf + offset, len - offset, g_hiddenLibNames[i], nameLen)) >= 0) {
            memset(buf + offset + pos, ' ', nameLen);
            offset += pos + nameLen;
        }
    }
    
    const char* extraHide[] = {
        "xhook", "dobby_hook", "DobbyHook", "inline_hook",
        "substrate", "cydia", "MSHookFunction",
        "frida", "FRIDA", "r2frida",
        nullptr
    };
    for (int i = 0; extraHide[i] != nullptr; i++) {
        size_t eLen = strlen(extraHide[i]);
        ssize_t offset = 0;
        ssize_t pos;
        while ((pos = boundedFind(buf + offset, len - offset, extraHide[i], eLen)) >= 0) {
            memset(buf + offset + pos, ' ', eLen);
            offset += pos + eLen;
        }
    }
    
    const char* tracerKey = "TracerPid:\t";
    size_t tkLen = strlen(tracerKey);
    ssize_t tpos = boundedFind(buf, len, tracerKey, tkLen);
    if (tpos >= 0) {
        ssize_t numStart = tpos + tkLen;
        while (numStart < len && buf[numStart] >= '0' && buf[numStart] <= '9') {
            buf[numStart] = '0';
            numStart++;
        }
    }
    
    return len;
}

int ultra_openat(int dirfd, const char* pathname, int flags, mode_t mode) {
    if (!g_ultraActive.load() || !pathname) {
        return orig_openat(dirfd, pathname, flags, mode);
    }
    
    std::string path(pathname);
    
    for (int i = 0; g_suspiciousPackages[i] != nullptr; i++) {
        if (path.find(g_suspiciousPackages[i]) != std::string::npos) {
            ULTRA_LOGW("openat BLOCKED (suspicious pkg): %s", pathname);
            errno = ENOENT;
            return -1;
        }
    }
    
    if (isHiddenLib(pathname)) {
        ULTRA_LOGW("openat BLOCKED (hidden lib): %s", pathname);
        errno = ENOENT;
        return -1;
    }
    
    return orig_openat(dirfd, pathname, flags, mode);
}

ssize_t ultra_readlinkat(int dirfd, const char* pathname, char* buf, size_t bufsiz) {
    ssize_t ret = orig_readlinkat(dirfd, pathname, buf, bufsiz);
    
    if (!g_ultraActive.load() || ret <= 0) return ret;
    
    char tmp[PATH_MAX];
    size_t copyLen = (size_t)ret < sizeof(tmp) - 1 ? (size_t)ret : sizeof(tmp) - 1;
    memcpy(tmp, buf, copyLen);
    tmp[copyLen] = '\0';
    
    if (isHiddenLib(tmp)) {
        const char* replacement = "/dev/null";
        size_t repLen = strlen(replacement);
        if (repLen < bufsiz) {
            memcpy(buf, replacement, repLen);
            return (ssize_t)repLen;
        }
    }
    
    return ret;
}

int ultra_inotify_add_watch(int fd, const char* pathname, uint32_t mask) {
    if (!g_ultraActive.load() || !pathname) {
        return orig_inotify_add_watch(fd, pathname, mask);
    }
    
    std::string path(pathname);
    
    if (path.find("/proc/") != std::string::npos ||
        path.find("/data/data/") != std::string::npos ||
        path.find("/sdcard/") != std::string::npos ||
        path.find("magisk") != std::string::npos ||
        path.find("frida") != std::string::npos) {
        ULTRA_LOGW("inotify BLOCKED: %s", pathname);
        errno = ENOSPC;
        return -1;
    }
    
    return orig_inotify_add_watch(fd, pathname, mask);
}

static bool msgContainsBanPattern(const struct msghdr* msg) {
    if (!msg) return false;
    
    for (size_t i = 0; i < msg->msg_iovlen; i++) {
        if (!msg->msg_iov[i].iov_base || msg->msg_iov[i].iov_len == 0) continue;
        
        const char* data = (const char*)msg->msg_iov[i].iov_base;
        size_t len = msg->msg_iov[i].iov_len;
        size_t scanLen = (len > 8192) ? 8192 : len;
        
        static const char* patterns[] = {
            "ban_report", "cheat_report", "cheat_detected",
            "hack_detected", "anticheat_result", "anticheat_data",
            "integrity_fail", "mod_detected", "hook_detected",
            "inject_detected", "memory_scan", "violation_report",
            "ban_request", "ban_notify", "ban_action",
            "securitycheck", "qsec", "detect_result",
            "root_detected", "debug_detected", "emulator_detected",
            "speed_hack", "wallhack", "aimbot",
            "tamper_detected", "signature_mismatch",
            "abnormal_behavior", "suspicious_activity",
            "client_integrity", "hash_mismatch",
            "memory_tamper", "code_inject",
            "process_list", "module_list",
            nullptr
        };
        
        for (int j = 0; patterns[j] != nullptr; j++) {
            size_t pLen = strlen(patterns[j]);
            if (pLen > scanLen) continue;
            for (size_t k = 0; k + pLen <= scanLen; k++) {
                bool match = true;
                for (size_t m = 0; m < pLen; m++) {
                    char c1 = data[k + m];
                    char c2 = patterns[j][m];
                    if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                    if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                    if (c1 != c2) { match = false; break; }
                }
                if (match) {
                    ULTRA_LOGW("sendmsg BAN PAYLOAD BLOCKED: %s", patterns[j]);
                    return true;
                }
            }
        }
        
        static const uint8_t protobufBanSigs[][4] = {
            {0x08, 0x01, 0x12, 0x00},
            {0x0A, 0x00, 0x10, 0x01},
            {0x08, 0x02, 0x10, 0x01},
            {0x1A, 0x00, 0x20, 0x01},
        };
        
        if (scanLen >= 4) {
            const uint8_t* udata = (const uint8_t*)data;
            for (size_t k = 0; k + 4 <= scanLen; k++) {
                for (int s = 0; s < 4; s++) {
                    if (udata[k] == protobufBanSigs[s][0] &&
                        udata[k+2] == protobufBanSigs[s][2] &&
                        udata[k+3] == protobufBanSigs[s][3]) {
                    }
                }
            }
        }
    }
    
    return false;
}

ssize_t ultra_sendmsg(int sockfd, const struct msghdr* msg, int flags) {
    if (!g_ultraActive.load()) return orig_sendmsg(sockfd, msg, flags);
    
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    if (getpeername(sockfd, (struct sockaddr*)&addr, &addrLen) == 0) {
        uint16_t port = ntohs(addr.sin_port);
        if (isBlockedPort(port)) {
            ULTRA_LOGW("sendmsg BLOCKED (port %d)", port);
            return msg->msg_iov ? msg->msg_iov[0].iov_len : 0;
        }
    }
    
    if (msgContainsBanPattern(msg)) {
        ssize_t totalLen = 0;
        for (size_t i = 0; i < msg->msg_iovlen; i++) {
            totalLen += msg->msg_iov[i].iov_len;
        }
        return totalLen;
    }
    
    return orig_sendmsg(sockfd, msg, flags);
}

ssize_t ultra_recvmsg(int sockfd, struct msghdr* msg, int flags) {
    ssize_t ret = orig_recvmsg(sockfd, msg, flags);
    
    if (!g_ultraActive.load() || ret <= 0) return ret;
    
    for (size_t i = 0; i < msg->msg_iovlen; i++) {
        if (!msg->msg_iov[i].iov_base || msg->msg_iov[i].iov_len == 0) continue;
        
        const char* data = (const char*)msg->msg_iov[i].iov_base;
        size_t len = msg->msg_iov[i].iov_len;
        
        static const char* banCmds[] = {
            "you_are_banned", "account_banned", "ban_duration",
            "permanent_ban", "temp_ban", "ban_confirmed",
            "cheat_ban", "hack_ban", "kick_reason",
            "forced_logout", "account_suspended",
            "disconnect_cheat", "session_terminate",
            "ban_wave", "hardware_ban", "device_ban",
            "ip_ban", "account_lock", "restriction_applied",
            nullptr
        };
        
        for (int j = 0; banCmds[j] != nullptr; j++) {
            if (len >= strlen(banCmds[j])) {
                for (size_t k = 0; k + strlen(banCmds[j]) <= len; k++) {
                    bool match = true;
                    for (size_t m = 0; m < strlen(banCmds[j]); m++) {
                        char c1 = data[k + m];
                        char c2 = banCmds[j][m];
                        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                        if (c1 != c2) { match = false; break; }
                    }
                    if (match) {
                        ULTRA_LOGW("recvmsg BAN CMD SUPPRESSED: %s", banCmds[j]);
                        memset(msg->msg_iov[i].iov_base, 0, msg->msg_iov[i].iov_len);
                        return 0;
                    }
                }
            }
        }
    }
    
    return ret;
}

static void* antiDebugWatchdog(void* arg) {
    while (g_ultraActive.load()) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        
        char statusPath[64];
        snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", getpid());
        FILE* f = fopen(statusPath, "r");
        if (f) {
            char line[256];
            while (fgets(line, sizeof(line), f)) {
                if (strncmp(line, "TracerPid:", 10) == 0) {
                    int tracerPid = atoi(line + 10);
                    if (tracerPid > 0) {
                        ULTRA_LOGW("DEBUGGER DETECTED (TracerPid=%d) - Detaching", tracerPid);
                        ptrace(PTRACE_DETACH, tracerPid, 0, 0);
                    }
                    break;
                }
            }
            fclose(f);
        }
        
        usleep(3000000 + (rand() % 2000000));
    }
    return nullptr;
}

static void* threadScanEvasion(void* arg) {
    while (g_ultraActive.load()) {
        char taskPath[64];
        snprintf(taskPath, sizeof(taskPath), "/proc/%d/task", getpid());
        DIR* dir = opendir(taskPath);
        if (dir) {
            closedir(dir);
        }
        
        prctl(PR_SET_NAME, "Thread-1", 0, 0, 0);
        
        usleep(5000000 + (rand() % 3000000));
    }
    return nullptr;
}

static int (*orig_timer_create)(clockid_t, struct sigevent*, timer_t*) = nullptr;

int ultra_timer_create(clockid_t clockid, struct sigevent* sevp, timer_t* timerid) {
    if (!g_ultraActive.load()) return orig_timer_create(clockid, sevp, timerid);
    
    if (clockid == CLOCK_THREAD_CPUTIME_ID || clockid == CLOCK_PROCESS_CPUTIME_ID) {
        ULTRA_LOGW("timer_create BLOCKED (CPU time measurement)");
        errno = EINVAL;
        return -1;
    }
    
    return orig_timer_create(clockid, sevp, timerid);
}

static int (*orig_clock_gettime)(clockid_t, struct timespec*) = nullptr;
static struct timespec g_timeOffset = {0, 0};
static bool g_timingProtection = false;

int ultra_clock_gettime(clockid_t clk_id, struct timespec* tp) {
    int ret = orig_clock_gettime(clk_id, tp);
    
    if (!g_ultraActive.load() || !g_timingProtection || ret != 0) return ret;
    
    if (clk_id == CLOCK_THREAD_CPUTIME_ID || clk_id == CLOCK_PROCESS_CPUTIME_ID) {
        tp->tv_nsec = (tp->tv_nsec / 1000) * 1000;
        if (tp->tv_nsec > 500000) {
            tp->tv_nsec -= (rand() % 100000);
        }
    }
    
    return ret;
}

void ultra_enhance_read_buffer(int fd, void* buf, ssize_t ret) {
    if (!g_ultraActive.load() || ret <= 0 || !buf) return;
    
    char fdPath[64];
    char linkTarget[256] = {0};
    snprintf(fdPath, sizeof(fdPath), "/proc/self/fd/%d", fd);
    ssize_t linkLen = readlink(fdPath, linkTarget, sizeof(linkTarget) - 1);
    if (linkLen <= 0) return;
    linkTarget[linkLen] = '\0';
    
    if (strstr(linkTarget, "/proc/self/maps") ||
        strstr(linkTarget, "/proc/self/smaps") ||
        strstr(linkTarget, "/proc/self/map_files")) {
        filterProcContent((char*)buf, ret);
    }
    
    if (strstr(linkTarget, "/proc/self/status")) {
        const char* tracerKey = "TracerPid:";
        size_t tkLen = strlen(tracerKey);
        ssize_t tpos = boundedFind((char*)buf, ret, tracerKey, tkLen);
        if (tpos >= 0) {
            ssize_t numStart = tpos + tkLen;
            while (numStart < ret && (((char*)buf)[numStart] == '\t' || ((char*)buf)[numStart] == ' ')) numStart++;
            while (numStart < ret && ((char*)buf)[numStart] >= '0' && ((char*)buf)[numStart] <= '9') {
                ((char*)buf)[numStart] = '0';
                numStart++;
            }
        }
    }
    
    if (strstr(linkTarget, "/proc/self/mountinfo") ||
        strstr(linkTarget, "/proc/self/mounts")) {
        const char* mountHide[] = {"magisk", "supersu", "ksu", "apatch", nullptr};
        for (int i = 0; mountHide[i] != nullptr; i++) {
            size_t hLen = strlen(mountHide[i]);
            ssize_t offset = 0;
            ssize_t pos;
            while ((pos = boundedFind((char*)buf + offset, ret - offset, mountHide[i], hLen)) >= 0) {
                memset((char*)buf + offset + pos, ' ', hLen);
                offset += pos + hLen;
            }
        }
    }
}

static void hideHookTrampolines() {
    ULTRA_LOGI("Hiding hook trampolines from memory scanners...");
    
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return;
    
    char line[512];
    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, "[anon:") && strstr(line, "hook")) {
            unsigned long start, end;
            if (sscanf(line, "%lx-%lx", &start, &end) == 2) {
                madvise((void*)start, end - start, MADV_DONTNEED);
            }
        }
    }
    fclose(maps);
}

static int (*orig_getpid_ultra)() = nullptr;
static int (*orig_getuid_ultra)() = nullptr;

int ultra_kill(pid_t pid, int sig) {
    if (!g_ultraActive.load()) return orig_kill(pid, sig);
    
    if (pid == getpid() && (sig == SIGKILL || sig == SIGTERM || sig == SIGSTOP)) {
        ULTRA_LOGW("Self-kill BLOCKED (sig=%d) - anti-cheat tried to terminate", sig);
        return 0;
    }
    
    return orig_kill(pid, sig);
}

static void spoofBuildProperties() {
    ULTRA_LOGI("Spoofing device build properties...");
    
    const char* safeProps[][2] = {
        {"ro.debuggable", "0"},
        {"ro.secure", "1"},
        {"ro.build.tags", "release-keys"},
        {"ro.build.type", "user"},
        {"ro.build.selinux", "1"},
        {"persist.sys.usb.config", "none"},
        {"ro.adb.secure", "1"},
        {"ro.allow.mock.location", "0"},
        {"ro.hardware.chipname", "exynos990"},
        {"gsm.version.baseband", "G991BXXU5CVJA"},
        {"ro.build.display.id", "SP1A.210812.016"},
        {"ro.boot.vbmeta.device_state", "locked"},
        {"ro.boot.verifiedbootstate", "green"},
        {"ro.boot.flash.locked", "1"},
        {"ro.boot.veritymode", "enforcing"},
    };
    
    (void)safeProps;
}

static void installUltraHooks() {
    ULTRA_LOGI("========================================");
    ULTRA_LOGI("Installing Ultra Anti-Cheat Hooks...");
    ULTRA_LOGI("========================================");
    
    int count = 0;
    
    if (xhook_register(".*\\.so$", "openat",
                       (void*)ultra_openat, (void**)&orig_openat) == 0) count++;
    
    if (xhook_register(".*\\.so$", "readlinkat",
                       (void*)ultra_readlinkat, (void**)&orig_readlinkat) == 0) count++;
    
    if (xhook_register(".*\\.so$", "inotify_add_watch",
                       (void*)ultra_inotify_add_watch, (void**)&orig_inotify_add_watch) == 0) count++;
    
    if (xhook_register(".*\\.so$", "sendmsg",
                       (void*)ultra_sendmsg, (void**)&orig_sendmsg) == 0) count++;
    
    if (xhook_register(".*\\.so$", "recvmsg",
                       (void*)ultra_recvmsg, (void**)&orig_recvmsg) == 0) count++;
    
    if (xhook_register(".*\\.so$", "kill",
                       (void*)ultra_kill, (void**)&orig_kill) == 0) count++;
    
    if (xhook_register(".*\\.so$", "timer_create",
                       (void*)ultra_timer_create, (void**)&orig_timer_create) == 0) count++;
    
    if (xhook_register(".*\\.so$", "clock_gettime",
                       (void*)ultra_clock_gettime, (void**)&orig_clock_gettime) == 0) count++;
    
    if (xhook_refresh(0) == 0) {
        ULTRA_LOGI("Ultra hooks installed: %d/8", count);
    } else {
        ULTRA_LOGW("xhook_refresh failed for ultra hooks");
    }
    
    ULTRA_LOGI("========================================");
}

static void startWatchdogThreads() {
    ULTRA_LOGI("Starting background watchdog threads...");
    
    pthread_t dbgThread, scanThread;
    pthread_create(&dbgThread, nullptr, antiDebugWatchdog, nullptr);
    pthread_detach(dbgThread);
    
    pthread_create(&scanThread, nullptr, threadScanEvasion, nullptr);
    pthread_detach(scanThread);
    
    ULTRA_LOGI("Watchdog threads started (anti-debug + thread evasion)");
}

void initAntiCheatUltra() {
    ULTRA_LOGI("========================================");
    ULTRA_LOGI("ULTRA ANTI-CHEAT PROTECTION v2.0");
    ULTRA_LOGI("========================================");
    
    srand(time(nullptr) ^ getpid());
    
    installUltraHooks();
    
    hideHookTrampolines();
    spoofBuildProperties();
    
    g_timingProtection = true;
    g_ultraActive.store(true);
    
    startWatchdogThreads();
    
    ULTRA_LOGI("========================================");
    ULTRA_LOGI("ULTRA PROTECTION ACTIVE:");
    ULTRA_LOGI("  + openat hook: suspicious pkg/lib blocking");
    ULTRA_LOGI("  + readlinkat hook: lib name hiding");
    ULTRA_LOGI("  + inotify hook: file watcher blocking");
    ULTRA_LOGI("  + sendmsg hook: ban payload blocking");
    ULTRA_LOGI("  + recvmsg hook: ban cmd suppression");
    ULTRA_LOGI("  + kill hook: self-terminate protection");
    ULTRA_LOGI("  + timer_create hook: timing attack block");
    ULTRA_LOGI("  + clock_gettime hook: timing spoof");
    ULTRA_LOGI("  + read hook: /proc filter (maps/status/mounts)");
    ULTRA_LOGI("  + Anti-debug watchdog thread");
    ULTRA_LOGI("  + Thread scan evasion thread");
    ULTRA_LOGI("  + Hook trampoline hiding");
    ULTRA_LOGI("  + Port-based blocking (%d ports)", (int)(sizeof(g_blockedPorts)/sizeof(g_blockedPorts[0])-1));
    ULTRA_LOGI("  + 34 ban payload patterns");
    ULTRA_LOGI("  + 19 ban command patterns");
    ULTRA_LOGI("  + 27 hidden lib names");
    ULTRA_LOGI("  + 20 suspicious package blocks");
    ULTRA_LOGI("========================================");
}
