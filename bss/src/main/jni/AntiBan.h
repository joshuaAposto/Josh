#pragma once

#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/stat.h>
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
// Silenced: logcat output exposes cheat activity to anti-cheat log scanners
#define ANTIBAN_LOGI(...) ((void)0)
#define ANTIBAN_LOGW(...) ((void)0)

// ============================================================
// SOCKET FD TRACKER
// Tracks socket file descriptors so write()/writev() hooks
// can distinguish socket writes from regular file writes
// ============================================================
static bool isSocketFd(int fd) {
    struct stat st;
    if (fstat(fd, &st) != 0) return false;
    return S_ISSOCK(st.st_mode);
}

// ============================================================
// PAYLOAD KEYWORD SCANNER
// Scans raw outgoing packet bytes for ban/report/cheat strings
// Increased scan window to 8192 bytes
// ============================================================
static bool payloadContainsBanKeyword(const void* buf, size_t len) {
    if (!buf || len == 0) return false;

    size_t scanLen = (len > 8192) ? 8192 : len;
    const char* data = (const char*)buf;

    static const char* banKeywords[] = {
        // Ban and report pipeline
        "ban_report",
        "cheat_report",
        "player_report",
        "user_report",
        "anticheat_result",
        "anticheat_data",
        "anticheat_report",
        "anticheat_upload",
        "cheat_detected",
        "hack_detected",
        "cheat_flag",
        "hack_flag",
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
        "ban_evidence",
        "ban_payload",
        "kick_notify",
        "cheatinfo",
        "detect_result",
        "violation_report",
        "violation_detected",
        "suspicious_activity",
        "abnormal_behavior",
        // Memory and process scanning reports
        "memory_scan",
        "memory_tamper",
        "integrity_fail",
        "integrity_check_fail",
        "mod_detected",
        "hook_detected",
        "inject_detected",
        "root_detected",
        "debug_detected",
        "debugger_detected",
        "emulator_detected",
        "frida_detected",
        "xposed_detected",
        "substrate_detected",
        "magisk_detected",
        // Gameplay anomaly reports
        "aim_anomaly",
        "speed_anomaly",
        "wallhack_detected",
        "aimbot_detected",
        "speedhack_detected",
        "teleport_detected",
        "no_recoil_detected",
        "esp_detected",
        // NetEase-specific field names (from observed network traffic)
        "report_type",
        "cheat_type",
        "hack_type",
        "security_event",
        "risk_event",
        "risk_level",
        "abnormal_data",
        "behavior_data",
        "suspicious_data",
        "scan_result",
        "process_scan",
        "lib_scan",
        "maps_scan",
        "file_tamper",
        "signature_fail",
        "checksum_fail",
        "verify_fail",
        "anti_tamper",
        "cheat_evidence",
        "report_evidence",
        "upload_evidence",
        "game_cheat",
        "fair_play",
        "unfair_play",
        nullptr
    };

    for (int i = 0; banKeywords[i] != nullptr; i++) {
        size_t kwLen = strlen(banKeywords[i]);
        if (kwLen > scanLen) continue;
        for (size_t j = 0; j + kwLen <= scanLen; j++) {
            bool match = true;
            for (size_t k = 0; k < kwLen; k++) {
                char c1 = data[j + k];
                char c2 = banKeywords[i][k];
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

    size_t scanLen = (len > 8192) ? 8192 : len;
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
        "ban_wave",
        "account_action",
        "cheating_confirmed",
        "violation_action",
        "account_terminated",
        "gameplay_violation",
        "fair_play_violation",
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
// SSL INTERCEPT
// Hooks SSL_write from libssl.so / libboringssl.so to
// intercept encrypted ban report payloads sent over HTTPS
// ============================================================
static int (*orig_SSL_write)(void* ssl, const void* buf, int num) = nullptr;
static int (*orig_SSL_read)(void* ssl, void* buf, int num) = nullptr;

int antiban_SSL_write(void* ssl, const void* buf, int num) {
    if (num > 0 && payloadContainsBanKeyword(buf, (size_t)num)) {
        ANTIBAN_LOGW("SSL_write BLOCKED: ban payload in encrypted stream (%d bytes)", num);
        return num;
    }
    return orig_SSL_write(ssl, buf, num);
}

int antiban_SSL_read(void* ssl, void* buf, int num) {
    int ret = orig_SSL_read(ssl, buf, num);
    if (ret > 0 && incomingIsBanNotification(buf, (size_t)ret)) {
        ANTIBAN_LOGW("SSL_read: incoming ban notification suppressed (%d bytes)", ret);
        memset(buf, 0, (size_t)ret);
        return 0;
    }
    return ret;
}

// ============================================================
// WRITE / WRITEV HOOKS
// Catches anti-cheat payloads sent via write()/writev() directly
// on socket file descriptors, bypassing send()
// ============================================================
static ssize_t (*orig_ab_write)(int fd, const void* buf, size_t count) = nullptr;
static ssize_t (*orig_ab_writev)(int fd, const struct iovec* iov, int iovcnt) = nullptr;

ssize_t antiban_write(int fd, const void* buf, size_t count) {
    if (isSocketFd(fd) && payloadContainsBanKeyword(buf, count)) {
        ANTIBAN_LOGW("write() BLOCKED: ban payload on socket fd=%d (%zu bytes)", fd, count);
        return (ssize_t)count;
    }
    return orig_ab_write(fd, buf, count);
}

ssize_t antiban_writev(int fd, const struct iovec* iov, int iovcnt) {
    if (isSocketFd(fd) && iov && iovcnt > 0) {
        for (int i = 0; i < iovcnt; i++) {
            if (iov[i].iov_base && iov[i].iov_len > 0) {
                if (payloadContainsBanKeyword(iov[i].iov_base, iov[i].iov_len)) {
                    ANTIBAN_LOGW("writev() BLOCKED: ban payload on socket fd=%d", fd);
                    ssize_t total = 0;
                    for (int j = 0; j < iovcnt; j++) total += iov[j].iov_len;
                    return total;
                }
            }
        }
    }
    return orig_ab_writev(fd, iov, iovcnt);
}

// ============================================================
// SENDMSG HOOK
// sendmsg() is another socket send path that bypasses send/sendto
// ============================================================
static ssize_t (*orig_ab_sendmsg)(int sockfd, const struct msghdr* msg, int flags) = nullptr;

ssize_t antiban_sendmsg(int sockfd, const struct msghdr* msg, int flags) {
    if (msg && msg->msg_iov) {
        for (size_t i = 0; i < (size_t)msg->msg_iovlen; i++) {
            if (msg->msg_iov[i].iov_base && msg->msg_iov[i].iov_len > 0) {
                if (payloadContainsBanKeyword(msg->msg_iov[i].iov_base,
                                              msg->msg_iov[i].iov_len)) {
                    ssize_t total = 0;
                    for (size_t j = 0; j < (size_t)msg->msg_iovlen; j++)
                        total += msg->msg_iov[j].iov_len;
                    return total;
                }
            }
        }
    }
    return orig_ab_sendmsg(sockfd, msg, flags);
}

// ============================================================
// SEND / SENDTO / RECV / RECVFROM HOOKS
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
        return (ssize_t)len;
    }
    return orig_ab_send(sockfd, buf, len, flags);
}

ssize_t antiban_sendto(int sockfd, const void* buf, size_t len, int flags,
                        const struct sockaddr* dest, socklen_t addrlen) {
    if (dest && dest->sa_family == AF_INET) {
        struct sockaddr_in* d = (struct sockaddr_in*)dest;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &d->sin_addr, ip, sizeof(ip));
        if (isBlockedIP(std::string(ip))) {
            ANTIBAN_LOGW("sendto BLOCKED (IP blacklist IPv4): %s", ip);
            errno = EHOSTUNREACH;
            return (ssize_t)len;
        }
    } else if (dest && dest->sa_family == AF_INET6) {
        struct sockaddr_in6* d6 = (struct sockaddr_in6*)dest;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &d6->sin6_addr, ip6, sizeof(ip6));
        if (isBlockedIP(std::string(ip6))) {
            ANTIBAN_LOGW("sendto BLOCKED (IP blacklist IPv6): %s", ip6);
            errno = EHOSTUNREACH;
            return (ssize_t)len;
        }
    }
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
        return 0;
    }
    return ret;
}

ssize_t antiban_recvfrom(int sockfd, void* buf, size_t len, int flags,
                          struct sockaddr* src, socklen_t* addrlen) {
    ssize_t ret = orig_ab_recvfrom(sockfd, buf, len, flags, src, addrlen);
    if (ret > 0 && incomingIsBanNotification(buf, (size_t)ret)) {
        memset(buf, 0, (size_t)ret);
        return 0;
    }
    return ret;
}

// ============================================================
// INSTALL ALL ANTI-BAN HOOKS
// ============================================================
static inline void installAntiBanHooks() {
    ANTIBAN_LOGI("========================================");
    ANTIBAN_LOGI("Installing Anti-Ban Payload Hooks...");
    ANTIBAN_LOGI("========================================");

    int count = 0;

    // Core socket hooks
    if (xhook_register(".*\\.so$", "send",
                       (void*)antiban_send, (void**)&orig_ab_send) == 0) count++;
    if (xhook_register(".*\\.so$", "sendto",
                       (void*)antiban_sendto, (void**)&orig_ab_sendto) == 0) count++;
    if (xhook_register(".*\\.so$", "recv",
                       (void*)antiban_recv, (void**)&orig_ab_recv) == 0) count++;
    if (xhook_register(".*\\.so$", "recvfrom",
                       (void*)antiban_recvfrom, (void**)&orig_ab_recvfrom) == 0) count++;

    // write/writev hooks — catches anti-cheat bypassing send()
    if (xhook_register(".*\\.so$", "write",
                       (void*)antiban_write, (void**)&orig_ab_write) == 0) count++;
    if (xhook_register(".*\\.so$", "writev",
                       (void*)antiban_writev, (void**)&orig_ab_writev) == 0) count++;

    // sendmsg hook — another socket send path bypassing send/sendto
    if (xhook_register(".*\\.so$", "sendmsg",
                       (void*)antiban_sendmsg, (void**)&orig_ab_sendmsg) == 0) count++;

    xhook_refresh(0);

    // SSL hooks — intercepts encrypted HTTPS ban reports
    // Try both OpenSSL and BoringSSL library names
    static const char* sslLibs[] = {
        "libssl.so", "libssl.so.1.1", "libboringssl.so",
        "libcrypto.so", "libssl_1_1.so", nullptr
    };
    for (int i = 0; sslLibs[i] != nullptr; i++) {
        void* handle = dlopen(sslLibs[i], RTLD_NOW | RTLD_GLOBAL);
        if (handle) {
            void* ssl_write_fn = dlsym(handle, "SSL_write");
            void* ssl_read_fn  = dlsym(handle, "SSL_read");
            if (ssl_write_fn && orig_SSL_write == nullptr) {
                if (xhook_register(sslLibs[i], "SSL_write",
                                   (void*)antiban_SSL_write,
                                   (void**)&orig_SSL_write) == 0) {
                    ANTIBAN_LOGI("SSL_write hooked in %s", sslLibs[i]);
                    count++;
                }
            }
            if (ssl_read_fn && orig_SSL_read == nullptr) {
                if (xhook_register(sslLibs[i], "SSL_read",
                                   (void*)antiban_SSL_read,
                                   (void**)&orig_SSL_read) == 0) {
                    ANTIBAN_LOGI("SSL_read hooked in %s", sslLibs[i]);
                    count++;
                }
            }
            dlclose(handle);
        }
    }

    xhook_refresh(0);

    ANTIBAN_LOGI("Anti-Ban hooks installed: %d total", count);
    ANTIBAN_LOGI("  + send/sendto:      payload keyword blocking ACTIVE");
    ANTIBAN_LOGI("  + sendto:           IP blacklist check ACTIVE (IPv4+IPv6)");
    ANTIBAN_LOGI("  + recv/recvfrom:    ban notification suppression ACTIVE");
    ANTIBAN_LOGI("  + write/writev:     socket bypass coverage ACTIVE");
    ANTIBAN_LOGI("  + SSL_write/read:   encrypted payload interception ACTIVE");
    ANTIBAN_LOGI("  + Keywords:         54 ban/cheat/report patterns");
    ANTIBAN_LOGI("  + Ban responses:    18 incoming ban/kick patterns");
    ANTIBAN_LOGI("========================================");
}
