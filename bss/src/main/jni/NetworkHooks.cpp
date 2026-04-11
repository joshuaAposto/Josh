#include "NetworkHooks.h"
#include "AntiTelemetry.h"
#include <dlfcn.h>
#include <android/log.h>
#include <cstdlib>
#include <errno.h>

#define NET_TAG "NetworkHook"
#define NET_LOGI(...) __android_log_print(ANDROID_LOG_INFO, NET_TAG, __VA_ARGS__)

// Define global function pointers
int (*orig_connect)(int sockfd, const struct sockaddr* addr, socklen_t addrlen) = nullptr;
struct hostent* (*orig_gethostbyname)(const char* name) = nullptr;
int (*orig_getaddrinfo)(const char* node, const char* service, 
                        const struct addrinfo* hints, struct addrinfo** res) = nullptr;

// ============= IP BLACKLIST =============
// Helper: check if an IPv4 address (in dotted string) is in a CIDR subnet
static bool ipInSubnet(const std::string& ip, const std::string& subnet, int prefixLen) {
    struct in_addr ipAddr, subnetAddr;
    if (inet_pton(AF_INET, ip.c_str(), &ipAddr) != 1) return false;
    if (inet_pton(AF_INET, subnet.c_str(), &subnetAddr) != 1) return false;
    uint32_t mask = (prefixLen == 0) ? 0 : htonl(~((1u << (32 - prefixLen)) - 1));
    return (ipAddr.s_addr & mask) == (subnetAddr.s_addr & mask);
}

bool isBlockedIP(const std::string& ip) {
    // ======= EXACT IP BLOCKLIST =======
    static const char* blockedIPs[] = {
        // Easebar telemetry & tracking
        "23.40.101.91",
        "13.248.222.62",
        "8.221.213.15",

        // AppsFlyer tracking
        "47.84.148.228",
        "47.84.149.192",

        // Telemetry IPs (port 4999 group)
        "8.221.191.207",
        "23.216.54.234",
        "35.198.101.208",
        "34.40.26.90",
        "34.107.46.90",
        "35.215.240.171",
        "35.215.213.18",
        "35.215.243.19",
        "35.198.19.228",
        "34.151.214.253",
        "35.198.38.209",
        "34.0.149.4",
        "34.0.130.125",
        "34.0.128.72",
        "34.174.233.33",
        "8.219.41.88",
        "34.174.152.62",
        "34.174.0.22",
        "8.219.42.187",
        "34.0.150.67",
        "34.149.47.227",
        "34.0.156.13",
        "34.0.131.165",
        "34.174.96.119",
        "34.174.74.33",
        "8.219.40.19",
        "23.205.117.99",
        "34.120.144.63",
        "23.205.117.56",
        "34.174.169.226",

        // Netease anti-cheat & ban server IPs
        "182.254.116.117",
        "140.207.69.43",
        "140.207.127.155",
        "140.207.123.184",
        "119.167.164.85",
        "106.61.26.249",

        // Known Netease reporting infrastructure
        "59.111.0.251",
        "59.111.179.170",
        "59.111.160.195",
        "59.111.181.60",
        "223.252.199.69",
        "223.252.199.11",
        "113.96.232.215",
        "113.96.208.100",
        "112.13.122.1",
        "112.13.119.17",

        // Firebase / Google telemetry endpoints
        "74.125.200.95",
        "142.250.4.95",
        "142.250.185.46",

        // Sentry crash reporting
        "35.168.22.104",
        "54.148.24.159",

        nullptr
    };

    for (int i = 0; blockedIPs[i] != nullptr; i++) {
        if (ip == blockedIPs[i]) return true;
    }

    // ======= SUBNET / RANGE BLOCKLIST =======
    // Block known Netease server IP ranges used for anti-cheat/telemetry
    struct { const char* subnet; int prefix; } subnets[] = {
        { "8.219.0.0",    16 },  // Alibaba Cloud (Netease telemetry)
        { "8.221.0.0",    16 },  // Alibaba Cloud (Netease telemetry)
        { "47.84.0.0",    16 },  // Alibaba Cloud (AppsFlyer tracking)
        { "35.215.0.0",   16 },  // Google Cloud (BloodStrike reporting)
        { "34.174.0.0",   16 },  // Google Cloud (BloodStrike reporting)
        { nullptr, 0 }
    };

    for (int i = 0; subnets[i].subnet != nullptr; i++) {
        if (ipInSubnet(ip, subnets[i].subnet, subnets[i].prefix)) return true;
    }

    return false;
}

// ============= DOMAIN FILTER =============
bool isTargetDomain(const std::string& domain) {
    std::vector<std::string> targetDomains = {
        "easebar.com", "netease.com", "163.com", "126.com", "127.net",
        "bloodstrike", "newspike", "qiyukf.com", "urs.netease.com",
        "game.163.com", "api.netease.com", "sdk.netease.com"
    };
    
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    for (const auto& target : targetDomains) {
        if (lowerDomain.find(target) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

// ============= HOOK CONNECT =============
int hook_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    if (!addr) return orig_connect(sockfd, addr, addrlen);
    
    // Handle IPv4
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
        int port = ntohs(addr_in->sin_port);
        
        // CHECK IP BLACKLIST FIRST!
        if (isBlockedIP(ip)) {
            NET_LOGI("🚫 IP BLOCKED (IPv4): %s:%d", ip, port);
            errno = EHOSTUNREACH;
            return -1;
        }
        
        // Try reverse DNS
        struct hostent* he = gethostbyaddr(&(addr_in->sin_addr), 
                                          sizeof(addr_in->sin_addr), AF_INET);
        if (he && he->h_name) {
            std::string hostname(he->h_name);
            
            // CHECK ANTI-TELEMETRY!
            if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(hostname)) {
                NET_LOGI("🚫 Connection BLOCKED to: %s:%d (%s)", hostname.c_str(), port, ip);
                errno = EHOSTUNREACH;
                return -1;
            }
            
            // If not blocked, log normally
            if (globalScanner && globalScanner->isEnabled()) {
                if (isTargetDomain(hostname)) {
                    NET_LOGI("[PRIORITY] Target domain: %s", hostname.c_str());
                }
                globalScanner->addDomain(hostname, port);
            }
        }
    }
    // Bug fix: also handle IPv6 — anti-cheat servers can use IPv6 to bypass IP blocking
    else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)addr;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip6, INET6_ADDRSTRLEN);
        int port = ntohs(addr_in6->sin6_port);
        
        struct hostent* he = gethostbyaddr(&(addr_in6->sin6_addr),
                                           sizeof(addr_in6->sin6_addr), AF_INET6);
        if (he && he->h_name) {
            std::string hostname(he->h_name);
            
            if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(hostname)) {
                NET_LOGI("🚫 Connection BLOCKED (IPv6) to: %s:%d (%s)", hostname.c_str(), port, ip6);
                errno = EHOSTUNREACH;
                return -1;
            }
            
            if (globalScanner && globalScanner->isEnabled()) {
                if (isTargetDomain(hostname)) {
                    NET_LOGI("[PRIORITY] Target domain (IPv6): %s", hostname.c_str());
                }
                globalScanner->addDomain(hostname, port);
            }
        }
    }
    
    return orig_connect(sockfd, addr, addrlen);
}

// ============= HOOK GETHOSTBYNAME =============
struct hostent* hook_gethostbyname(const char* name) {
    if (name) {
        std::string domain(name);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            NET_LOGI("🚫 DNS BLOCKED: %s", name);
            h_errno = HOST_NOT_FOUND;
            return nullptr;
        }
        
        // Log if not blocked
        NET_LOGI("DNS Lookup: %s", name);
        
        if (globalScanner && globalScanner->isEnabled()) {
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s ★★★", name);
            }
            globalScanner->addDomain(domain, 0);
        }
    }
    
    return orig_gethostbyname(name);
}

// ============= HOOK GETADDRINFO =============
int hook_getaddrinfo(const char* node, const char* service,
                    const struct addrinfo* hints, struct addrinfo** res) {
    
    if (node) {
        std::string domain(node);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            int port = service ? atoi(service) : 0;
            NET_LOGI("🚫 getaddrinfo BLOCKED: %s:%d", node, port);
            return EAI_NONAME;
        }
        
        NET_LOGI("getaddrinfo: %s:%s", node, service ? service : "N/A");
        
        if (globalScanner && globalScanner->isEnabled()) {
            int port = service ? atoi(service) : 0;
            
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s:%d ★★★", node, port);
            }
            
            globalScanner->addDomain(domain, port);
        }
    }
    
    return orig_getaddrinfo(node, service, hints, res);
}

// ============= INSTALL HOOKS =============
void installNetworkHooks() {
    void* libc = dlopen("libc.so", RTLD_NOW);
    if (!libc) {
        NET_LOGI("Failed to open libc.so");
        return;
    }
    
    orig_connect = reinterpret_cast<int (*)(int, const struct sockaddr*, socklen_t)>(
        dlsym(libc, "connect")
    );
    
    orig_gethostbyname = reinterpret_cast<struct hostent* (*)(const char*)>(
        dlsym(libc, "gethostbyname")
    );
    
    orig_getaddrinfo = reinterpret_cast<int (*)(const char*, const char*, 
                                                 const struct addrinfo*, struct addrinfo**)>(
        dlsym(libc, "getaddrinfo")
    );
    
    if (!orig_connect || !orig_gethostbyname || !orig_getaddrinfo) {
        NET_LOGI("Failed to get original functions");
        dlclose(libc);
        return;
    }
    
    NET_LOGI("Original functions obtained");
    
    int hook_count = 0;
    
    if (xhook_register(".*\\.so$", "connect", 
                      (void*)hook_connect, (void**)&orig_connect) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "gethostbyname", 
                      (void*)hook_gethostbyname, (void**)&orig_gethostbyname) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "getaddrinfo", 
                      (void*)hook_getaddrinfo, (void**)&orig_getaddrinfo) == 0) {
        hook_count++;
    }
    
    if (xhook_refresh(0) == 0) {
        NET_LOGI("========================================");
        NET_LOGI("Network Hooks: %d/3 installed", hook_count);
        NET_LOGI("Anti-Telemetry: ACTIVE");
        NET_LOGI("IP Blocking: 40+ IPs blacklisted");
        NET_LOGI("Domain Blocking: 50+ domains");
        NET_LOGI("Targeting: easebar.com, netease.com");
        NET_LOGI("BloodStrike telemetry blocked");
        NET_LOGI("========================================");
    } else {
        NET_LOGI("xhook_refresh failed!");
    }
    
    dlclose(libc);
}












/*
#include "NetworkHooks.h"
#include "AntiTelemetry.h"
#include <dlfcn.h>
#include <android/log.h>
#include <cstdlib>
#include <errno.h>

#define NET_TAG "NetworkHook"
#define NET_LOGI(...) __android_log_print(ANDROID_LOG_INFO, NET_TAG, __VA_ARGS__)

// Define global function pointers
int (*orig_connect)(int sockfd, const struct sockaddr* addr, socklen_t addrlen) = nullptr;
struct hostent* (*orig_gethostbyname)(const char* name) = nullptr;
int (*orig_getaddrinfo)(const char* node, const char* service, 
                        const struct addrinfo* hints, struct addrinfo** res) = nullptr;

// ============= IP BLACKLIST =============
bool isBlockedIP(const std::string& ip) {
    // Daftar IP yang harus diblock
    std::vector<std::string> blockedIPs = {
        "23.40.101.91",      // Easebar telemetry
        "13.248.222.62"      // Easebar tracking
    };
    
    for (const auto& blocked : blockedIPs) {
        if (ip == blocked) {
            return true;
        }
    }
    return false;
}

// ============= DOMAIN FILTER =============
bool isTargetDomain(const std::string& domain) {
    std::vector<std::string> targetDomains = {
        "easebar.com", "netease.com", "163.com", "126.com", "127.net",
        "bloodstrike", "newspike", "qiyukf.com", "urs.netease.com",
        "game.163.com", "api.netease.com", "sdk.netease.com"
    };
    
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    for (const auto& target : targetDomains) {
        if (lowerDomain.find(target) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

// ============= HOOK CONNECT =============
int hook_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    if (!addr) return orig_connect(sockfd, addr, addrlen);
    
    // Handle IPv4
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
        int port = ntohs(addr_in->sin_port);
        
        // CHECK IP BLACKLIST FIRST!
        if (isBlockedIP(ip)) {
            NET_LOGI("🚫 IP BLOCKED (IPv4): %s:%d", ip, port);
            errno = EHOSTUNREACH;
            return -1;
        }
        
        // Try reverse DNS
        struct hostent* he = gethostbyaddr(&(addr_in->sin_addr), 
                                          sizeof(addr_in->sin_addr), AF_INET);
        if (he && he->h_name) {
            std::string hostname(he->h_name);
            
            // CHECK ANTI-TELEMETRY!
            if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(hostname)) {
                NET_LOGI("🚫 Connection BLOCKED to: %s:%d (%s)", hostname.c_str(), port, ip);
                errno = EHOSTUNREACH;
                return -1;
            }
            
            // If not blocked, log normally
            if (globalScanner && globalScanner->isEnabled()) {
                if (isTargetDomain(hostname)) {
                    NET_LOGI("[PRIORITY] Target domain: %s", hostname.c_str());
                }
                globalScanner->addDomain(hostname, port);
            }
        }
    }
    // Bug fix: also handle IPv6 — anti-cheat servers can use IPv6 to bypass IP blocking
    else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)addr;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip6, INET6_ADDRSTRLEN);
        int port = ntohs(addr_in6->sin6_port);
        
        struct hostent* he = gethostbyaddr(&(addr_in6->sin6_addr),
                                           sizeof(addr_in6->sin6_addr), AF_INET6);
        if (he && he->h_name) {
            std::string hostname(he->h_name);
            
            if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(hostname)) {
                NET_LOGI("🚫 Connection BLOCKED (IPv6) to: %s:%d (%s)", hostname.c_str(), port, ip6);
                errno = EHOSTUNREACH;
                return -1;
            }
            
            if (globalScanner && globalScanner->isEnabled()) {
                if (isTargetDomain(hostname)) {
                    NET_LOGI("[PRIORITY] Target domain (IPv6): %s", hostname.c_str());
                }
                globalScanner->addDomain(hostname, port);
            }
        }
    }
    
    return orig_connect(sockfd, addr, addrlen);
}

// ============= HOOK GETHOSTBYNAME =============
struct hostent* hook_gethostbyname(const char* name) {
    if (name) {
        std::string domain(name);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            NET_LOGI("🚫 DNS BLOCKED: %s", name);
            h_errno = HOST_NOT_FOUND;
            return nullptr;
        }
        
        // Log if not blocked
        NET_LOGI("DNS Lookup: %s", name);
        
        if (globalScanner && globalScanner->isEnabled()) {
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s ★★★", name);
            }
            globalScanner->addDomain(domain, 0);
        }
    }
    
    return orig_gethostbyname(name);
}

// ============= HOOK GETADDRINFO =============
int hook_getaddrinfo(const char* node, const char* service,
                    const struct addrinfo* hints, struct addrinfo** res) {
    
    if (node) {
        std::string domain(node);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            int port = service ? atoi(service) : 0;
            NET_LOGI("🚫 getaddrinfo BLOCKED: %s:%d", node, port);
            return EAI_NONAME;
        }
        
        NET_LOGI("getaddrinfo: %s:%s", node, service ? service : "N/A");
        
        if (globalScanner && globalScanner->isEnabled()) {
            int port = service ? atoi(service) : 0;
            
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s:%d ★★★", node, port);
            }
            
            globalScanner->addDomain(domain, port);
        }
    }
    
    return orig_getaddrinfo(node, service, hints, res);
}

// ============= INSTALL HOOKS =============
void installNetworkHooks() {
    void* libc = dlopen("libc.so", RTLD_NOW);
    if (!libc) {
        NET_LOGI("Failed to open libc.so");
        return;
    }
    
    orig_connect = reinterpret_cast<int (*)(int, const struct sockaddr*, socklen_t)>(
        dlsym(libc, "connect")
    );
    
    orig_gethostbyname = reinterpret_cast<struct hostent* (*)(const char*)>(
        dlsym(libc, "gethostbyname")
    );
    
    orig_getaddrinfo = reinterpret_cast<int (*)(const char*, const char*, 
                                                 const struct addrinfo*, struct addrinfo**)>(
        dlsym(libc, "getaddrinfo")
    );
    
    if (!orig_connect || !orig_gethostbyname || !orig_getaddrinfo) {
        NET_LOGI("Failed to get original functions");
        dlclose(libc);
        return;
    }
    
    NET_LOGI("Original functions obtained");
    
    int hook_count = 0;
    
    if (xhook_register(".*\\.so$", "connect", 
                      (void*)hook_connect, (void**)&orig_connect) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "gethostbyname", 
                      (void*)hook_gethostbyname, (void**)&orig_gethostbyname) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "getaddrinfo", 
                      (void*)hook_getaddrinfo, (void**)&orig_getaddrinfo) == 0) {
        hook_count++;
    }
    
    if (xhook_refresh(0) == 0) {
        NET_LOGI("========================================");
        NET_LOGI("Network Hooks: %d/3 installed", hook_count);
        NET_LOGI("Anti-Telemetry: ACTIVE");
        NET_LOGI("IP Blocking: 2 IPs blacklisted");
        NET_LOGI("Domain Blocking: 28+ domains");
        NET_LOGI("Targeting: easebar.com, netease.com");
        NET_LOGI("========================================");
    } else {
        NET_LOGI("xhook_refresh failed!");
    }
    
    dlclose(libc);
}



/*
#include "NetworkHooks.h"
#include "AntiTelemetry.h"  // TAMBAHKAN INI
#include <dlfcn.h>
#include <android/log.h>
#include <cstdlib>
#include <errno.h>

#define NET_TAG "NetworkHook"
#define NET_LOGI(...) __android_log_print(ANDROID_LOG_INFO, NET_TAG, __VA_ARGS__)

// Define global function pointers
int (*orig_connect)(int sockfd, const struct sockaddr* addr, socklen_t addrlen) = nullptr;
struct hostent* (*orig_gethostbyname)(const char* name) = nullptr;
int (*orig_getaddrinfo)(const char* node, const char* service, 
                        const struct addrinfo* hints, struct addrinfo** res) = nullptr;

// ============= DOMAIN FILTER =============
bool isTargetDomain(const std::string& domain) {
    std::vector<std::string> targetDomains = {
        "easebar.com", "netease.com", "163.com", "126.com", "127.net",
        "bloodstrike", "newspike", "qiyukf.com", "urs.netease.com",
        "game.163.com", "api.netease.com", "sdk.netease.com"
    };
    
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    for (const auto& target : targetDomains) {
        if (lowerDomain.find(target) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

// ============= HOOK CONNECT =============
int hook_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    if (addr && addr->sa_family == AF_INET) {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
        int port = ntohs(addr_in->sin_port);
        
        // Try reverse DNS
        struct hostent* he = gethostbyaddr(&(addr_in->sin_addr), 
                                          sizeof(addr_in->sin_addr), AF_INET);
        if (he && he->h_name) {
            std::string hostname(he->h_name);
            
            // CHECK ANTI-TELEMETRY FIRST!
            if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(hostname)) {
                NET_LOGI("🚫 Connection BLOCKED to: %s:%d", hostname.c_str(), port);
                errno = EHOSTUNREACH;  // Set error
                return -1;  // Block connection!
            }
            
            // If not blocked, log normally
            if (globalScanner && globalScanner->isEnabled()) {
                if (isTargetDomain(hostname)) {
                    NET_LOGI("[PRIORITY] Target domain: %s", hostname.c_str());
                }
                globalScanner->addDomain(hostname, port);
            }
        }
    }
    
    return orig_connect(sockfd, addr, addrlen);
}

// ============= HOOK GETHOSTBYNAME =============
struct hostent* hook_gethostbyname(const char* name) {
    if (name) {
        std::string domain(name);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            NET_LOGI("🚫 DNS BLOCKED: %s", name);
            h_errno = HOST_NOT_FOUND;
            return nullptr;  // Block DNS resolution!
        }
        
        // Log if not blocked
        NET_LOGI("DNS Lookup: %s", name);
        
        if (globalScanner && globalScanner->isEnabled()) {
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s ★★★", name);
            }
            globalScanner->addDomain(domain, 0);
        }
    }
    
    return orig_gethostbyname(name);
}

// ============= HOOK GETADDRINFO =============
int hook_getaddrinfo(const char* node, const char* service,
                    const struct addrinfo* hints, struct addrinfo** res) {
    
    if (node) {
        std::string domain(node);
        
        // CHECK ANTI-TELEMETRY FIRST!
        if (globalAntiTelemetry && globalAntiTelemetry->checkAndBlock(domain)) {
            int port = service ? atoi(service) : 0;
            NET_LOGI("🚫 getaddrinfo BLOCKED: %s:%d", node, port);
            return EAI_NONAME;  // Block resolution!
        }
        
        NET_LOGI("getaddrinfo: %s:%s", node, service ? service : "N/A");
        
        if (globalScanner && globalScanner->isEnabled()) {
            int port = service ? atoi(service) : 0;
            
            if (isTargetDomain(domain)) {
                NET_LOGI("[PRIORITY] ★★★ Target domain: %s:%d ★★★", node, port);
            }
            
            globalScanner->addDomain(domain, port);
        }
    }
    
    return orig_getaddrinfo(node, service, hints, res);
}

// ============= INSTALL HOOKS =============
void installNetworkHooks() {
    void* libc = dlopen("libc.so", RTLD_NOW);
    if (!libc) {
        NET_LOGI("Failed to open libc.so");
        return;
    }
    
    orig_connect = reinterpret_cast<int (*)(int, const struct sockaddr*, socklen_t)>(
        dlsym(libc, "connect")
    );
    
    orig_gethostbyname = reinterpret_cast<struct hostent* (*)(const char*)>(
        dlsym(libc, "gethostbyname")
    );
    
    orig_getaddrinfo = reinterpret_cast<int (*)(const char*, const char*, 
                                                 const struct addrinfo*, struct addrinfo**)>(
        dlsym(libc, "getaddrinfo")
    );
    
    if (!orig_connect || !orig_gethostbyname || !orig_getaddrinfo) {
        NET_LOGI("Failed to get original functions");
        dlclose(libc);
        return;
    }
    
    NET_LOGI("Original functions obtained");
    
    int hook_count = 0;
    
    if (xhook_register(".*\\.so$", "connect", 
                      (void*)hook_connect, (void**)&orig_connect) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "gethostbyname", 
                      (void*)hook_gethostbyname, (void**)&orig_gethostbyname) == 0) {
        hook_count++;
    }
    
    if (xhook_register(".*\\.so$", "getaddrinfo", 
                      (void*)hook_getaddrinfo, (void**)&orig_getaddrinfo) == 0) {
        hook_count++;
    }
    
    if (xhook_refresh(0) == 0) {
        NET_LOGI("========================================");
        NET_LOGI("Network Hooks: %d/3 installed", hook_count);
        NET_LOGI("Anti-Telemetry: ACTIVE");
        NET_LOGI("Targeting: easebar.com, netease.com");
        NET_LOGI("========================================");
    } else {
        NET_LOGI("xhook_refresh failed!");
    }
    
    dlclose(libc);
}
*/
