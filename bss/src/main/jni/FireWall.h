//pritam
#pragma once
#include <arpa/inet.h>
#include <dlfcn.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include "Dobby/dobby.h"

#ifdef __cplusplus
extern "C" {
#endif

// Simple OBFUSCATE macro
#define OBFUSCATE(str) str

/* === Server bypass helpers ===
   To enable bypass, define BYPASS_SERVER=1 at compile time or before including this file.
*/
#ifndef BYPASS_SERVER
  #define BYPASS_SERVER 0
#endif

#define IF_SERVER_BYPASS_RETURN(val) do { if (BYPASS_SERVER != 0) return (val); } while(0)
#define IF_SERVER_BYPASS_CONTINUE() do { if (BYPASS_SERVER != 0) return; } while(0)
#define IF_SERVER_BYPASS_BREAK() do { if (BYPASS_SERVER != 0) break; } while(0)

// Global variables
int Firewall = 1;
int XXX = 0;

// Function pointer typedefs
typedef int (*SocketFunc)(int domain, int type, int protocol);
typedef int (*ConnectFunc)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
typedef int (*InetPtonFunc)(int af, const char *src, void *dst);
typedef int (*GetAddrInfoFunc)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
typedef struct hostent *(*GetHostByAddrFunc)(const void *addr, socklen_t len, int type);
typedef struct hostent *(*GetHostByNameFunc)(const char *name);

// Function pointers
SocketFunc originalSocket = NULL;
ConnectFunc originalConnect = NULL;
InetPtonFunc originalInetPton = NULL;
GetAddrInfoFunc originalGetAddrInfo = NULL;
GetHostByAddrFunc originalGetHostByAddr = NULL;
GetHostByNameFunc originalGetHostByName = NULL;

// Helper function to check blocked domains
int isBlockedDomain(const char *domain) {
    if (!domain) return 0;
    
    // Check each blocked domain individually
    if (strstr(domain, OBFUSCATE("firebaselogging.googleapis.com")) ||
        strstr(domain, OBFUSCATE("httpdns.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("g108na-14.gph.easebar.com")) ||
        strstr(domain, OBFUSCATE("g0-web.gsf.easebar.com")) ||
        strstr(domain, OBFUSCATE("filecatch.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("feedback-system-dev.webapp.easebar.com")) ||
        strstr(domain, OBFUSCATE("feedback-ovs.fp.ps.easebar.com")) ||
        strstr(domain, OBFUSCATE("fb.webapp.easebar.com")) ||
        strstr(domain, OBFUSCATE("event.shence.easebar.com")) ||
        strstr(domain, OBFUSCATE("fb-dev.webapp.easebar.com")) ||
        strstr(domain, OBFUSCATE("event.sc.easebar.com")) ||
        strstr(domain, OBFUSCATE("applog.unisdk.easebar.com")) ||
        strstr(domain, OBFUSCATE("api-office.db.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("api-data-klasifikasi.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("aliothjp-gcp.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("admin.fp.ps.easebar.com")) ||
        strstr(domain, OBFUSCATE("l33sg-gamerecord-sg.l33naobj.leihuo.easebar.com")) ||
        strstr(domain, OBFUSCATE("acsdk.gameyw.easebar.com")) ||
        strstr(domain, OBFUSCATE("app-measurement.com")) ||
        strstr(domain, OBFUSCATE("anticheatexpert.com")) ||
        strstr(domain, OBFUSCATE("gameguard.net")) ||
        strstr(domain, OBFUSCATE("bamreport.bloodstrike.netease.com")) ||
        strstr(domain, OBFUSCATE("analytics.bloodstrike.netease.com")) ||
        strstr(domain, OBFUSCATE("lamssettings-pa.googleapis.com")) ||
        strstr(domain, OBFUSCATE("graph.facebook.com")) ||
        strstr(domain, OBFUSCATE("appdump.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("whoami.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("firebaseremoteconfig.googleapis.com")) ||
        strstr(domain, OBFUSCATE("audiostatlog.cc.easebar.com")) ||
        strstr(domain, OBFUSCATE("audiouser.cc.easebar.com")) ||
        strstr(domain, OBFUSCATE("drpf-g83naxx1ena.proxima.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("newspike-avatar.fp.ps.easebar.com")) ||
        strstr(domain, OBFUSCATE("b.q.easebar.com")) ||
        strstr(domain, OBFUSCATE("c5ipmc-inapps.appsflyersdk.com")) ||
        strstr(domain, OBFUSCATE("cc.fp.ps.netease.com")) ||
        strstr(domain, OBFUSCATE("cdn-settings.appsflyersdk.com")) ||
        strstr(domain, OBFUSCATE("conversions.appsflyer.com")) ||
        strstr(domain, OBFUSCATE("data-detect.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("g0.gsf.easebar.com")) ||
        strstr(domain, OBFUSCATE("gcdsdk.appsflyer.com")) ||
        strstr(domain, OBFUSCATE("inapps.appsflyer.com")) ||
        strstr(domain, OBFUSCATE("in.appcenter.ms")) ||
        strstr(domain, OBFUSCATE("o33249.ingest.sentry.io")) ||
        strstr(domain, OBFUSCATE("launches.appsflyer.com")) ||
        strstr(domain, OBFUSCATE("mcount.easebar.com")) ||
        strstr(domain, OBFUSCATE("p16-sign-va.tiktokcdn.com")) ||
        strstr(domain, OBFUSCATE("pharos.netease.com")) ||
        strstr(domain, OBFUSCATE("beacons5.gvt5.com")) ||
        strstr(domain, OBFUSCATE("beacons5.gvt4.com")) ||
        strstr(domain, OBFUSCATE("beacons5.gvt3.com")) ||
        strstr(domain, OBFUSCATE("beacons5.gvt2.com")) ||
        strstr(domain, OBFUSCATE("beacons.gcp.gvt2.com")) ||
        strstr(domain, OBFUSCATE("ire-dsu.shalltry.com")) ||
        strstr(domain, OBFUSCATE("a.nel.cloudflare.com")) ||
        strstr(domain, OBFUSCATE("optimizationguide-pa.googleapis.com")) ||
        strstr(domain, OBFUSCATE("ire-oneid.shalltry.com")) ||
        strstr(domain, OBFUSCATE("protocol.unisdk.easebar.com")) ||
        strstr(domain, OBFUSCATE("s.q.easebar.com")) ||
        strstr(domain, OBFUSCATE("sdkgate.pushv3.easebar.com")) ||
        strstr(domain, OBFUSCATE("translate.mpsdk.easebar.com")) ||
        strstr(domain, OBFUSCATE("unisdk.proxima.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("who.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("who.easebar.com")) ||
        strstr(domain, OBFUSCATE("ulogs.umengcloud.com")) ||
        strstr(domain, OBFUSCATE("g83naxx1ena.appdump.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("p16-oec-va.ibyteimg.com")) ||
        strstr(domain, OBFUSCATE("reqhfg-cdn-settings.appsflyersdk.com")) ||
        strstr(domain, OBFUSCATE("impression.update.easebar.com")) ||
        strstr(domain, OBFUSCATE("logs.bloodstrike.com")) ||
        strstr(domain, OBFUSCATE("tracking.bloodstrike.com")) ||
        strstr(domain, OBFUSCATE("anti-cheat.bloodstrike.com")) ||
        strstr(domain, OBFUSCATE("h73na-14.gdl.easebar.com")) ||
        strstr(domain, OBFUSCATE("h65na-14.gph.easebar.com")) ||
        strstr(domain, OBFUSCATE("g83-microservice-testing.nie.easebar.com")) ||
        strstr(domain, OBFUSCATE("g29-114.gdl.easebar.com")) ||
        strstr(domain, OBFUSCATE("g138-14.gdl.easebar.com")) ||
        strstr(domain, OBFUSCATE("g112na-pre.gdl.easebar.com")) ||
        strstr(domain, OBFUSCATE("g108naxx2gb-14.update.easebar.com")) ||
        strstr(domain, OBFUSCATE("g108naxx2gb-14.gph.easebar.com")) ||
        strstr(domain, OBFUSCATE("g108naxx2gb-14.gsf.easebar.com")) ||
        // Critical anti-cheat & reporting domains
        strstr(domain, OBFUSCATE("reportban.netease.com")) ||
        strstr(domain, OBFUSCATE("matchreport.bloodstrike.netease.com")) ||
        strstr(domain, OBFUSCATE("securitycheck.netease.com")) ||
        strstr(domain, OBFUSCATE("accountsecurity.netease.com")) ||
        strstr(domain, OBFUSCATE("userlog.bloodstrike.netease.com")) ||
        strstr(domain, OBFUSCATE("qsec.netease.com")) ||
        strstr(domain, OBFUSCATE("tinfo.netease.com")) ||
        strstr(domain, OBFUSCATE("log.qos.netease.com")) ||
        strstr(domain, OBFUSCATE("tracking.netease.com")) ||
        strstr(domain, OBFUSCATE("anticheat.bloodstrike.net")) ||
        strstr(domain, OBFUSCATE("anticheat-api.bloodstrike.com")) ||
        strstr(domain, OBFUSCATE("report.bloodstrike.net")) ||
        strstr(domain, OBFUSCATE("report-api.bloodstrike.io")) ||
        strstr(domain, OBFUSCATE("report-api.bloodstrike.net")) ||
        strstr(domain, OBFUSCATE("appeal-api.bloodstrike.io")) ||
        strstr(domain, OBFUSCATE("log.bloodstrike.net")) ||
        strstr(domain, OBFUSCATE("track.bloodstrike.io")) ||
        strstr(domain, OBFUSCATE("log-collector.bloodstrike.com")) ||
        strstr(domain, OBFUSCATE("stat-api.bloodstrike.net")) ||
        strstr(domain, OBFUSCATE("update-api.bloodstrike.io")) ||
        strstr(domain, OBFUSCATE("config.bloodstrike.io")) ||
        strstr(domain, OBFUSCATE("analytics.bloodstrike.netease.com"))) {
        return 1;
    }
    return 0;
}

int hookedInetPton(int af, const char *src, void *dst) {
    if (!Firewall || !src) {
        return originalInetPton(af, src, dst);
    }
    
    char ip[INET6_ADDRSTRLEN];
    
    if (isBlockedDomain(src)) {
        XXX = 0;
    } else {
        XXX = 1;
    }
    
    int result = originalInetPton(af, src, dst);
    
    if (result > 0) {
        if (af == AF_INET) {
            struct in_addr *ipv4 = (struct in_addr *)dst;
            inet_ntop(af, ipv4, ip, INET6_ADDRSTRLEN);
        } else if (af == AF_INET6) {
            struct in6_addr *ipv6 = (struct in6_addr *)dst;
            inet_ntop(af, ipv6, ip, INET6_ADDRSTRLEN);
        }
    }
    
    return result;
}

int hookedGetAddrInfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    IF_SERVER_BYPASS_RETURN(originalGetAddrInfo(node, service, hints, res));
    
    if (!Firewall || !node) {
        return originalGetAddrInfo(node, service, hints, res);
    }
    
    if (isBlockedDomain(node)) {
        XXX = 0;
        return -10;
    }
    
    int result = originalGetAddrInfo(node, service, hints, res);
    
    if (result == 0 && *res != NULL) {
        char ip[INET6_ADDRSTRLEN];
        struct sockaddr *addr = (*res)->ai_addr;
        
        if (addr->sa_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET6_ADDRSTRLEN);
        } else if (addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
        } else {
            return -2;
        }
        
        XXX = 500;
    }
    
    return result;
}

struct hostent *hookedGetHostByAddr(const void *addr, socklen_t len, int type) {
    IF_SERVER_BYPASS_RETURN(originalGetHostByAddr(addr, len, type));
    
    struct hostent *result = originalGetHostByAddr(addr, len, type);
    if (result != NULL && result->h_addr_list[0] != NULL) {
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(type, result->h_addr_list[0], ip, INET6_ADDRSTRLEN);
    }
    return result;
}

struct hostent *hookedGetHostByName(const char *name) {
    IF_SERVER_BYPASS_RETURN(originalGetHostByName(name));
    
    if (!name) {
        return originalGetHostByName(name);
    }
    
    struct hostent *result = originalGetHostByName(name);
    if (result != NULL && result->h_addr_list[0] != NULL) {
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(result->h_addrtype, result->h_addr_list[0], ip, INET6_ADDRSTRLEN);
    }
    return result;
}

int hookedSocket(int domain, int type, int protocol) {
    int sockfd = originalSocket(domain, type, protocol);
    if (XXX == 1000) {
        close(sockfd);
        return -50000;
    }
    return sockfd;
}

int hookedConnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (XXX == -7000) {
        return 0;
    }
    return originalConnect(sockfd, addr, addrlen);
}

// Additional hook functions
void (*original_system_property_get)(const char *, char *) = NULL;

void hooked_system_property_get(const char *name, char *value) {
    if (name && strcmp(name, "ro.hardware") == 0) {
        strcpy(value, "qualcom");
        printf("Modified hardware name: %s\n", value);
        return;
    }
    if (original_system_property_get) {
        original_system_property_get(name, value);
    }
}

char *(*originalStrstr)(const char *haystack, const char *needle) = NULL;

char *hookedStrstr(const char *haystack, const char *needle) {
    if (haystack && strstr(haystack, "vphonegaga") != NULL) {
        char *modifiedHaystack = strdup(haystack);
        if (modifiedHaystack) {
            char *occurrence = strstr(modifiedHaystack, "crash6777777");
            if (occurrence != NULL) {
                strncpy(occurrence, "blocked", strlen("blocked"));
            }
            free(modifiedHaystack);
        }
    }
    return originalStrstr(haystack, needle);
}

// Network I/O hook functions
ssize_t (*original_send)(int sockfd, const void *buf, size_t len, int flags) = NULL;
ssize_t (*original_recv)(int sockfd, void *buf, size_t len, int flags) = NULL;
ssize_t (*original_recvfrom)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) = NULL;
ssize_t (*original_sendto)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) = NULL;

ssize_t hooked_send(int sockfd, const void *buf, size_t len, int flags) {
    return original_send(sockfd, buf, len, flags);
}

ssize_t hooked_recv(int sockfd, void *buf, size_t len, int flags) {
    return original_recv(sockfd, buf, len, flags);
}

ssize_t hooked_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    return original_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

ssize_t hooked_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    return original_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

void hookFunctions() {
    void *handle = dlopen("libGame.so", RTLD_LAZY);
    if (handle == NULL) {
        handle = dlopen("libc.so", RTLD_LAZY);
        if (handle == NULL) {
            return;
        }
    }
    
    // Get original function pointers
    originalSocket = (SocketFunc)dlsym(handle, "socket");
    originalConnect = (ConnectFunc)dlsym(handle, "connect");
    originalInetPton = (InetPtonFunc)dlsym(handle, "inet_pton");
    originalGetAddrInfo = (GetAddrInfoFunc)dlsym(handle, "getaddrinfo");
    originalGetHostByAddr = (GetHostByAddrFunc)dlsym(handle, "gethostbyaddr");
    originalGetHostByName = (GetHostByNameFunc)dlsym(handle, "gethostbyname");
    original_system_property_get = (void(*)(const char *, char *))dlsym(handle, "__system_property_get");
    originalStrstr = (char *(*)(const char *, const char *))dlsym(handle, "strstr");
    original_send = (ssize_t(*)(int, const void *, size_t, int))dlsym(handle, "send");
    original_recv = (ssize_t(*)(int, void *, size_t, int))dlsym(handle, "recv");
    original_recvfrom = (ssize_t(*)(int, void *, size_t, int, struct sockaddr *, socklen_t *))dlsym(handle, "recvfrom");
    original_sendto = (ssize_t(*)(int, const void *, size_t, int, const struct sockaddr *, socklen_t))dlsym(handle, "sendto");
    
    // Hook the functions using Dobby
    if (originalSocket != NULL) {
        DobbyHook((void *)originalSocket, (void *)hookedSocket, (void **)&originalSocket);
    }
    if (originalConnect != NULL) {
        DobbyHook((void *)originalConnect, (void *)hookedConnect, (void **)&originalConnect);
    }
    if (originalInetPton != NULL) {
        DobbyHook((void *)originalInetPton, (void *)hookedInetPton, (void **)&originalInetPton);
    }
    if (originalGetAddrInfo != NULL) {
        DobbyHook((void *)originalGetAddrInfo, (void *)hookedGetAddrInfo, (void **)&originalGetAddrInfo);
    }
    if (originalGetHostByAddr != NULL) {
        DobbyHook((void *)originalGetHostByAddr, (void *)hookedGetHostByAddr, (void **)&originalGetHostByAddr);
    }
    if (originalGetHostByName != NULL) {
        DobbyHook((void *)originalGetHostByName, (void *)hookedGetHostByName, (void **)&originalGetHostByName);
    }
    
    dlclose(handle);
}

#ifdef __cplusplus
}
#endif
