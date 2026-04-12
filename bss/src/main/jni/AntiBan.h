#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <string>
#include <vector>
#include <android/log.h>
#include "xhook/xhook.h"

#define ANTIBAN_TAG "AntiBan"
#define ANTIBAN_LOGI(...) __android_log_print(ANDROID_LOG_INFO,  ANTIBAN_TAG, __VA_ARGS__)
#define ANTIBAN_LOGW(...) __android_log_print(ANDROID_LOG_WARN,  ANTIBAN_TAG, __VA_ARGS__)

// ============================================================
// PAYLOAD KEYWORD SCANNER
// Scans raw outgoing packet bytes for ban/report/cheat strings
// ============================================================
static bool payloadContainsBanKeyword(const void* buf, size_t len) {
    if (!buf || len == 0) return false;

    size_t scanLen = (len > 16384) ? 16384 : len;
    const char* data = (const char*)buf;

    static const char* banKeywords[] = {
        "ban_report",
        "cheat_report",
        "anticheat_result",
        "anticheat_data",
        "cheat_detected",
        "hack_detected",
        "reportban",
        "bamreport",
        "matchreport",
        "userlog",
        "securitycheck",
        "accountsecurity",
        "qsec",
        "tinfo",
        "ban_request",
        "ban_notify",
        "ban_action",
        "kick_notify",
        "cheatinfo",
        "detect_result",
        "violation_report",
        "memory_scan",
        "integrity_fail",
        "mod_detected",
        "hook_detected",
        "inject_detected",
        "root_detected",
        "debug_detected",
        "emulator_detected",
        "speed_hack",
        "wallhack_detected",
        "aimbot_detected",
        "esp_detected",
        "tamper_detected",
        "signature_mismatch",
        "abnormal_behavior",
        "suspicious_activity",
        "client_integrity",
        "hash_mismatch",
        "memory_tamper",
        "code_inject",
        "process_list",
        "module_list",
        "lib_inject",
        "function_hook",
        "trampoline_detect",
        "inline_hook",
        "got_hook",
        "plt_hook",
        "syscall_hook",
        "frida_detect",
        "xposed_detect",
        "magisk_detect",
        "su_binary",
        "debug_attach",
        "ptrace_detect",
        "breakpoint_detect",
        "timing_anomaly",
        "speed_anomaly",
        "teleport_detect",
        "damage_anomaly",
        "packet_replay",
        "client_mismatch",
        "server_validate_fail",
        "hardware_id_mismatch",
        "device_fingerprint",
        "env_tamper",
        "apk_tamper",
        "dex_tamper",
        "native_tamper",
        "memory_patch",
        "value_modify",
        nullptr
    };

    for (int i = 0; banKeywords[i] != nullptr; i++) {
        size_t kwLen = strlen(banKeywords[i]);
        if (kwLen > scanLen) continue;
        // Case-insensitive scan
        for (size_t j = 0; j + kwLen <= scanLen; j++) {
            bool match = true;
            for (size_t k = 0; k < kwLen; k++) {
                char c1 = data[j + k];
                char c2 = banKeywords[i][k];
                // tolower
                if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                if (c1 != c2) { match = false; break; }
            }
            if (match) {
                ANTIBAN_LOGW("BAN PAYLOAD BLOCKED: keyword='%s' in %zu-byte packet",
                             banKeywords[i], len);
                return true;
            }
        }
    }
    return false;
}

// ============================================================
// INCOMING BAN NOTIFICATION SUPPRESSOR
// Scans incoming packets for server-side ban/kick commands
// ============================================================
static bool incomingIsBanNotification(const void* buf, size_t len) {
    if (!buf || len == 0) return false;

    size_t scanLen = (len > 2048) ? 2048 : len;
    const char* data = (const char*)buf;

    static const char* banResponses[] = {
        "you_are_banned",
        "account_banned",
        "ban_duration",
        "permanent_ban",
        "temp_ban",
        "ban_confirmed",
        "cheat_ban",
        "hack_ban",
        "kick_reason",
        "forced_logout",
        "account_suspended",
        "disconnect_cheat",
        "session_terminate",
        "ban_wave",
        "hardware_ban",
        "device_ban",
        "ip_ban",
        "account_lock",
        "restriction_applied",
        "penalty_applied",
        "cooldown_applied",
        "matchmaking_ban",
        "ranked_ban",
        "competitive_ban",
        "warning_issued",
        "strike_applied",
        "account_flagged",
        "review_pending",
        "access_denied",
        "login_restricted",
        nullptr
    };

    for (int i = 0; banResponses[i] != nullptr; i++) {
        size_t kwLen = strlen(banResponses[i]);
        if (kwLen > scanLen) continue;
        for (size_t j = 0; j + kwLen <= scanLen; j++) {
            bool match = true;
            for (size_t k = 0; k < kwLen; k++) {
                char c1 = data[j + k];
                char c2 = banResponses[i][k];
                if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                if (c1 != c2) { match = false; break; }
            }
            if (match) {
                ANTIBAN_LOGW("BAN NOTIFICATION SUPPRESSED: keyword='%s'",
                             banResponses[i]);
                return true;
            }
        }
    }
    return false;
}

// ============================================================
// BLOCKED IP CHECK — reuse same list as NetworkHooks
// ============================================================
extern bool isBlockedIP(const std::string& ip);

// ============================================================
// ANTI-BAN SEND HOOKS
// ============================================================
static ssize_t (*orig_ab_send)(int sockfd, const void* buf, size_t len, int flags) = nullptr;
static ssize_t (*orig_ab_sendto)(int sockfd, const void* buf, size_t len, int flags,
                                  const struct sockaddr* dest, socklen_t addrlen) = nullptr;
static ssize_t (*orig_ab_recv)(int sockfd, void* buf, size_t len, int flags) = nullptr;
static ssize_t (*orig_ab_recvfrom)(int sockfd, void* buf, size_t len, int flags,
                                    struct sockaddr* src, socklen_t* addrlen) = nullptr;

ssize_t antiban_send(int sockfd, const void* buf, size_t len, int flags) {
    if (payloadContainsBanKeyword(buf, len)) {
        errno = ECONNREFUSED;
        return (ssize_t)len;  // Pretend it was sent so the game doesn't crash
    }
    return orig_ab_send(sockfd, buf, len, flags);
}

ssize_t antiban_sendto(int sockfd, const void* buf, size_t len, int flags,
                        const struct sockaddr* dest, socklen_t addrlen) {
    // Check destination IP against blacklist
    if (dest && dest->sa_family == AF_INET) {
        struct sockaddr_in* d = (struct sockaddr_in*)dest;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &d->sin_addr, ip, sizeof(ip));
        if (isBlockedIP(std::string(ip))) {
            ANTIBAN_LOGW("sendto BLOCKED (IP blacklist): %s", ip);
            errno = EHOSTUNREACH;
            return (ssize_t)len;
        }
    } else if (dest && dest->sa_family == AF_INET6) {
        struct sockaddr_in6* d6 = (struct sockaddr_in6*)dest;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &d6->sin6_addr, ip6, sizeof(ip6));
        if (isBlockedIP(std::string(ip6))) {
            ANTIBAN_LOGW("sendto BLOCKED (IPv6 blacklist): %s", ip6);
            errno = EHOSTUNREACH;
            return (ssize_t)len;
        }
    }

    // Check payload keywords
    if (payloadContainsBanKeyword(buf, len)) {
        errno = ECONNREFUSED;
        return (ssize_t)len;
    }

    return orig_ab_sendto(sockfd, buf, len, flags, dest, addrlen);
}

ssize_t antiban_recv(int sockfd, void* buf, size_t len, int flags) {
    ssize_t ret = orig_ab_recv(sockfd, buf, len, flags);
    if (ret > 0 && incomingIsBanNotification(buf, (size_t)ret)) {
        memset(buf, 0, (size_t)ret);
        ANTIBAN_LOGW("recv: ban notification zeroed (%zd bytes)", ret);
        return 0;
    }
    return ret;
}

ssize_t antiban_recvfrom(int sockfd, void* buf, size_t len, int flags,
                          struct sockaddr* src, socklen_t* addrlen) {
    ssize_t ret = orig_ab_recvfrom(sockfd, buf, len, flags, src, addrlen);
    if (ret > 0 && incomingIsBanNotification(buf, (size_t)ret)) {
        memset(buf, 0, (size_t)ret);
        ANTIBAN_LOGW("recvfrom: ban notification zeroed (%zd bytes)", ret);
        return 0;
    }
    return ret;
}

static ssize_t (*orig_ab_write)(int fd, const void* buf, size_t count) = nullptr;

ssize_t antiban_write(int fd, const void* buf, size_t count) {
    if (payloadContainsBanKeyword(buf, count)) {
        ANTIBAN_LOGW("write BLOCKED: ban payload in fd=%d (%zu bytes)", fd, count);
        return (ssize_t)count;
    }
    return orig_ab_write(fd, buf, count);
}

// ============================================================
// INSTALL ALL ANTI-BAN HOOKS
// ============================================================
static inline void installAntiBanHooks() {
    ANTIBAN_LOGI("========================================");
    ANTIBAN_LOGI("Installing Anti-Ban Payload Hooks...");
    ANTIBAN_LOGI("========================================");

    int count = 0;

    if (xhook_register(".*\\.so$", "send",
                       (void*)antiban_send, (void**)&orig_ab_send) == 0) count++;

    if (xhook_register(".*\\.so$", "sendto",
                       (void*)antiban_sendto, (void**)&orig_ab_sendto) == 0) count++;

    if (xhook_register(".*\\.so$", "recv",
                       (void*)antiban_recv, (void**)&orig_ab_recv) == 0) count++;

    if (xhook_register(".*\\.so$", "recvfrom",
                       (void*)antiban_recvfrom, (void**)&orig_ab_recvfrom) == 0) count++;

    if (xhook_register(".*\\.so$", "write",
                       (void*)antiban_write, (void**)&orig_ab_write) == 0) count++;

    if (xhook_refresh(0) == 0) {
        ANTIBAN_LOGI("Anti-Ban hooks installed: %d/5", count);
        ANTIBAN_LOGI("  + send/sendto: payload keyword blocking ACTIVE");
        ANTIBAN_LOGI("  + sendto: IP blacklist check ACTIVE");
        ANTIBAN_LOGI("  + recv/recvfrom: ban notification suppression ACTIVE");
        ANTIBAN_LOGI("  + write: unix socket ban payload blocking ACTIVE");
        ANTIBAN_LOGI("  + Keywords: 73 ban/cheat/report patterns");
        ANTIBAN_LOGI("  + Ban responses: 30 suppression patterns");
    } else {
        ANTIBAN_LOGI("xhook_refresh failed for anti-ban hooks");
    }

    ANTIBAN_LOGI("========================================");
}
