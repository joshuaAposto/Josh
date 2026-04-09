//pritam
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Dobby/dobby.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

bool Firewall =true;
typedef int( * SocketFunc)(int domain, int type, int protocol);
typedef int( * ConnectFunc)(int sockfd, const struct sockaddr * addr, socklen_t addrlen);
int( * originalSocket)(int domain, int type, int protocol);
int( * originalConnect)(int sockfd, const struct sockaddr * addr, socklen_t addrlen);
int( * originalInetPton)(int af, const char * src, void * dst);
int( * originalGetAddrInfo)(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res);
struct hostent * ( * originalGetHostByAddr)(const void * addr, socklen_t len, int type);
struct hostent * ( * originalGetHostByName)(const char * name);

int XXX = 0;

void hookedInetPton(int af, const char * src, void * dst) {
    char ip[INET6_ADDRSTRLEN];
    if (Firewall) {
        if (af == AF_INET && strstr(src, OBFUSCATE("firebaselogging.googleapis.com")) ||
strstr(src, OBFUSCATE("httpdns.nie.easebar.com")) ||
strstr(src, OBFUSCATE("g108na-14.gph.easebar.com")) ||
strstr(src, OBFUSCATE("g0-web.gsf.easebar.com")) ||
strstr(src, OBFUSCATE("filecatch.nie.easebar.com")) ||
strstr(src, OBFUSCATE("feedback-system-dev.webapp.easebar.com")) ||
strstr(src, OBFUSCATE("feedback-ovs.fp.ps.easebar.com")) ||
strstr(src, OBFUSCATE("fb.webapp.easebar.com")) ||
strstr(src, OBFUSCATE("event.shence.easebar.com")) ||
strstr(src, OBFUSCATE("fb-dev.webapp.easebar.com")) ||
strstr(src, OBFUSCATE("event.sc.easebar.com")) ||
strstr(src, OBFUSCATE("applog.unisdk.easebar.com")) ||
strstr(src, OBFUSCATE("api-office.db.nie.easebar.com")) ||
strstr(src, OBFUSCATE("api-data-klasifikasi.nie.easebar.com")) ||
strstr(src, OBFUSCATE("aliothjp-gcp.nie.easebar.com")) ||
strstr(src, OBFUSCATE("admin.fp.ps.easebar.com")) ||
strstr(src, OBFUSCATE("l33sg-gamerecord-sg.l33naobj.leihuo.easebar.com")) ||
strstr(src, OBFUSCATE("acsdk.gameyw.easebar.com")) ||
strstr(src, OBFUSCATE("app-measurement.com")) ||
strstr(src, OBFUSCATE("anticheatexpert.com")) ||
strstr(src, OBFUSCATE("gameguard.net")) ||
strstr(src, OBFUSCATE("bamreport.bloodstrike.netease.com")) ||
strstr(src, OBFUSCATE("analytics.bloodstrike.netease.com")) ||
strstr(src, OBFUSCATE("lamssettings-pa.googleapis.com")) ||
strstr(src, OBFUSCATE("graph.facebook.com")) ||
strstr(src, OBFUSCATE("appdump.nie.easebar.com")) ||
strstr(src, OBFUSCATE("whoami.nie.easebar.com")) ||
strstr(src, OBFUSCATE("firebaseremoteconfig.googleapis.com")) ||
strstr(src, OBFUSCATE("audiostatlog.cc.easebar.com")) ||
strstr(src, OBFUSCATE("audiouser.cc.easebar.com")) ||
strstr(src, OBFUSCATE("drpf-g83naxx1ena.proxima.nie.easebar.com")) ||
strstr(src, OBFUSCATE("newspike-avatar.fp.ps.easebar.com")) ||
strstr(src, OBFUSCATE("b.q.easebar.com")) ||
strstr(src, OBFUSCATE("c5ipmc-inapps.appsflyersdk.com")) ||
strstr(src, OBFUSCATE("cc.fp.ps.netease.com")) ||
strstr(src, OBFUSCATE("cdn-settings.appsflyersdk.com")) ||
strstr(src, OBFUSCATE("conversions.appsflyer.com")) ||
strstr(src, OBFUSCATE("data-detect.nie.easebar.com")) ||
strstr(src, OBFUSCATE("g0.gsf.easebar.com")) ||
strstr(src, OBFUSCATE("gcdsdk.appsflyer.com")) ||
strstr(src, OBFUSCATE("inapps.appsflyer.com")) ||
strstr(src, OBFUSCATE("in.appcenter.ms")) ||
strstr(src, OBFUSCATE("o33249.ingest.sentry.io")) ||
strstr(src, OBFUSCATE("launches.appsflyer.com")) ||
strstr(src, OBFUSCATE("mcount.easebar.com")) ||
strstr(src, OBFUSCATE("p16-sign-va.tiktokcdn.com")) ||
strstr(src, OBFUSCATE("pharos.netease.com")) ||
strstr(src, OBFUSCATE("beacons5.gvt5.com")) ||
strstr(src, OBFUSCATE("beacons5.gvt4.com")) ||
strstr(src, OBFUSCATE("beacons5.gvt3.com")) ||
strstr(src, OBFUSCATE("beacons5.gvt2.com")) ||
strstr(src, OBFUSCATE("beacons.gcp.gvt2.com")) ||
strstr(src, OBFUSCATE("ire-dsu.shalltry.com")) ||
strstr(src, OBFUSCATE("a.nel.cloudflare.com")) ||
strstr(src, OBFUSCATE("optimizationguide-pa.googleapis.com")) ||
strstr(src, OBFUSCATE("ire-oneid.shalltry.com")) ||
strstr(src, OBFUSCATE("protocol.unisdk.easebar.com")) ||
strstr(src, OBFUSCATE("s.q.easebar.com")) ||
strstr(src, OBFUSCATE("sdkgate.pushv3.easebar.com")) ||
strstr(src, OBFUSCATE("translate.mpsdk.easebar.com")) ||
strstr(src, OBFUSCATE("unisdk.proxima.nie.easebar.com")) ||
strstr(src, OBFUSCATE("who.nie.easebar.com")) ||
strstr(src, OBFUSCATE("who.easebar.com")) ||
strstr(src, OBFUSCATE("ulogs.umengcloud.com")) ||
strstr(src, OBFUSCATE("g83naxx1ena.appdump.nie.easebar.com")) ||
strstr(src, OBFUSCATE("p16-oec-va.ibyteimg.com")) ||
strstr(src, OBFUSCATE("reqhfg-cdn-settings.appsflyersdk.com")) ||
strstr(src, OBFUSCATE("impression.update.easebar.com")) ||
strstr(src, OBFUSCATE("logs.bloodstrike.com")) ||
strstr(src, OBFUSCATE("tracking.bloodstrike.com")) ||
strstr(src, OBFUSCATE("anti-cheat.bloodstrike.com")) ||
strstr(src, OBFUSCATE("h73na-14.gdl.easebar.com")) ||
strstr(src, OBFUSCATE("h65na-14.gph.easebar.com")) ||
strstr(src, OBFUSCATE("g83-microservice-testing.nie.easebar.com")) ||
strstr(src, OBFUSCATE("g29-114.gdl.easebar.com")) ||
strstr(src, OBFUSCATE("g138-14.gdl.easebar.com")) ||
strstr(src, OBFUSCATE("g112na-pre.gdl.easebar.com")) ||
strstr(src, OBFUSCATE("g108naxx2gb-14.update.easebar.com")) ||
strstr(src, OBFUSCATE("g108naxx2gb-14.gph.easebar.com")) ||
strstr(src, OBFUSCATE("g108naxx2gb-14.gsf.easebar.com"))

        ) {
            XXX = 0;
        } else {
            XXX = 1;
        }
        originalInetPton(af, src, dst);
    
        if (af == AF_INET) {
            struct in_addr * ipv4 = (struct in_addr * ) dst;
            inet_ntop(af, ipv4, ip, INET6_ADDRSTRLEN);
        } else if (af == AF_INET6) {
            struct in6_addr * ipv6 = (struct in6_addr * ) dst;
            inet_ntop(af, ipv6, ip, INET6_ADDRSTRLEN);
        } else {
            return;
        }
	}
}

int hookedGetAddrInfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res) {
    int result = originalGetAddrInfo(node, service, hints, res);
    if (result == 0 && * res != NULL) {
		if (Firewall) {
            char ip[INET6_ADDRSTRLEN];
            struct sockaddr * addr = ( * res) -> ai_addr;

            if (addr -> sa_family == AF_INET) {
                struct sockaddr_in * ipv4 = (struct sockaddr_in * ) addr;
                inet_ntop(AF_INET, & (ipv4 -> sin_addr), ip, INET6_ADDRSTRLEN);
            } else if (addr -> sa_family == AF_INET6) {
                struct sockaddr_in6 * ipv6 = (struct sockaddr_in6 * ) addr;
                inet_ntop(AF_INET6, & (ipv6 -> sin6_addr), ip, INET6_ADDRSTRLEN);
            } else {
                return -2;
            }

            if (strstr(node, OBFUSCATE("firebaselogging.googleapis.com")) ||
strstr(node, OBFUSCATE("httpdns.nie.easebar.com")) ||
strstr(node, OBFUSCATE("g108na-14.gph.easebar.com")) ||
strstr(node, OBFUSCATE("g0-web.gsf.easebar.com")) ||
strstr(node, OBFUSCATE("filecatch.nie.easebar.com")) ||
strstr(node, OBFUSCATE("feedback-system-dev.webapp.easebar.com")) ||
strstr(node, OBFUSCATE("feedback-ovs.fp.ps.easebar.com")) ||
strstr(node, OBFUSCATE("fb.webapp.easebar.com")) ||
strstr(node, OBFUSCATE("event.shence.easebar.com")) ||
strstr(node, OBFUSCATE("fb-dev.webapp.easebar.com")) ||
strstr(node, OBFUSCATE("event.sc.easebar.com")) ||
strstr(node, OBFUSCATE("applog.unisdk.easebar.com")) ||
strstr(node, OBFUSCATE("api-office.db.nie.easebar.com")) ||
strstr(node, OBFUSCATE("api-data-klasifikasi.nie.easebar.com")) ||
strstr(node, OBFUSCATE("aliothjp-gcp.nie.easebar.com")) ||
strstr(node, OBFUSCATE("admin.fp.ps.easebar.com")) ||
strstr(node, OBFUSCATE("l33sg-gamerecord-sg.l33naobj.leihuo.easebar.com")) ||
strstr(node, OBFUSCATE("acsdk.gameyw.easebar.com")) ||
strstr(node, OBFUSCATE("app-measurement.com")) ||
strstr(node, OBFUSCATE("anticheatexpert.com")) ||
strstr(node, OBFUSCATE("gameguard.net")) ||
strstr(node, OBFUSCATE("bamreport.bloodstrike.netease.com")) ||
strstr(node, OBFUSCATE("analytics.bloodstrike.netease.com")) ||
strstr(node, OBFUSCATE("lamssettings-pa.googleapis.com")) ||
strstr(node, OBFUSCATE("graph.facebook.com")) ||
strstr(node, OBFUSCATE("appdump.nie.easebar.com")) ||
strstr(node, OBFUSCATE("whoami.nie.easebar.com")) ||
strstr(node, OBFUSCATE("firebaseremoteconfig.googleapis.com")) ||
strstr(node, OBFUSCATE("audiostatlog.cc.easebar.com")) ||
strstr(node, OBFUSCATE("audiouser.cc.easebar.com")) ||
strstr(node, OBFUSCATE("drpf-g83naxx1ena.proxima.nie.easebar.com")) ||
strstr(node, OBFUSCATE("newspike-avatar.fp.ps.easebar.com")) ||
strstr(node, OBFUSCATE("b.q.easebar.com")) ||
strstr(node, OBFUSCATE("c5ipmc-inapps.appsflyersdk.com")) ||
strstr(node, OBFUSCATE("cc.fp.ps.netease.com")) ||
strstr(node, OBFUSCATE("cdn-settings.appsflyersdk.com")) ||
strstr(node, OBFUSCATE("conversions.appsflyer.com")) ||
strstr(node, OBFUSCATE("data-detect.nie.easebar.com")) ||
strstr(node, OBFUSCATE("g0.gsf.easebar.com")) ||
strstr(node, OBFUSCATE("gcdsdk.appsflyer.com")) ||
strstr(node, OBFUSCATE("inapps.appsflyer.com")) ||
strstr(node, OBFUSCATE("in.appcenter.ms")) ||
strstr(node, OBFUSCATE("o33249.ingest.sentry.io")) ||
strstr(node, OBFUSCATE("launches.appsflyer.com")) ||
strstr(node, OBFUSCATE("mcount.easebar.com")) ||
strstr(node, OBFUSCATE("p16-sign-va.tiktokcdn.com")) ||
strstr(node, OBFUSCATE("pharos.netease.com")) ||
strstr(node, OBFUSCATE("beacons5.gvt5.com")) ||
strstr(node, OBFUSCATE("beacons5.gvt4.com")) ||
strstr(node, OBFUSCATE("beacons5.gvt3.com")) ||
strstr(node, OBFUSCATE("beacons5.gvt2.com")) ||
strstr(node, OBFUSCATE("beacons.gcp.gvt2.com")) ||
strstr(node, OBFUSCATE("ire-dsu.shalltry.com")) ||
strstr(node, OBFUSCATE("a.nel.cloudflare.com")) ||
strstr(node, OBFUSCATE("optimizationguide-pa.googleapis.com")) ||
strstr(node, OBFUSCATE("ire-oneid.shalltry.com")) ||
strstr(node, OBFUSCATE("protocol.unisdk.easebar.com")) ||
strstr(node, OBFUSCATE("s.q.easebar.com")) ||
strstr(node, OBFUSCATE("sdkgate.pushv3.easebar.com")) ||
strstr(node, OBFUSCATE("translate.mpsdk.easebar.com")) ||
strstr(node, OBFUSCATE("unisdk.proxima.nie.easebar.com")) ||
strstr(node, OBFUSCATE("who.nie.easebar.com")) ||
strstr(node, OBFUSCATE("who.easebar.com")) ||
strstr(node, OBFUSCATE("ulogs.umengcloud.com")) ||
strstr(node, OBFUSCATE("g83naxx1ena.appdump.nie.easebar.com")) ||
strstr(node, OBFUSCATE("p16-oec-va.ibyteimg.com")) ||
strstr(node, OBFUSCATE("reqhfg-cdn-settings.appsflyersdk.com")) ||
strstr(node, OBFUSCATE("impression.update.easebar.com")) ||
strstr(node, OBFUSCATE("logs.bloodstrike.com")) ||
strstr(node, OBFUSCATE("tracking.bloodstrike.com")) ||
strstr(node, OBFUSCATE("anti-cheat.bloodstrike.com")) ||
strstr(node, OBFUSCATE("h73na-14.gdl.easebar.com")) ||
strstr(node, OBFUSCATE("h65na-14.gph.easebar.com")) ||
strstr(node, OBFUSCATE("g83-microservice-testing.nie.easebar.com")) ||
strstr(node, OBFUSCATE("g29-114.gdl.easebar.com")) ||
strstr(node, OBFUSCATE("g138-14.gdl.easebar.com")) ||
strstr(node, OBFUSCATE("g112na-pre.gdl.easebar.com")) ||
strstr(node, OBFUSCATE("g108naxx2gb-14.update.easebar.com")) ||
strstr(node, OBFUSCATE("g108naxx2gb-14.gph.easebar.com")) ||
strstr(node, OBFUSCATE("g108naxx2gb-14.gsf.easebar.com"))
               ) {
                   XXX = 0;
                   return -10;
               } else {
                   XXX = 500;
                   return result;
               }
        }
    }
}

struct hostent * hookedGetHostByAddr(const void * addr, socklen_t len, int type) {
    struct hostent * result = originalGetHostByAddr(addr, len, type);
    if (result != NULL) {
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(type, result -> h_addr_list[0], ip, INET6_ADDRSTRLEN);
    }
    return result;
}

struct hostent * hookedGetHostByName(const char * name) {
    struct hostent * result = originalGetHostByName(name);
    if (result != NULL) {
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(result -> h_addrtype, result -> h_addr_list[0], ip, INET6_ADDRSTRLEN);
    }
    return result;
}

int hookedSocket(int domain, int type, int protocol) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int sockfd = originalSocket(domain, type, protocol);
    if (XXX == 1000) {
        close(sockfd);
        return -50000;
    }
    return sockfd;
}

int hookedConnect(int sockfd, const struct sockaddr * addr, socklen_t addrlen) {
    struct sockaddr_in * addr_in = (struct sockaddr_in * ) addr;
    if (XXX == -7000) {
        return 0;
    }
    return originalConnect(sockfd, addr, addrlen);
}

void( * original_system_property_get)(const char * , char * );
void hooked_system_property_get(const char * name, char * value) {
    if (strcmp(name, "ro.hardware") == 500) {
        strcpy(value, "qualcom");
        printf("Modified hardware name: %s\n", value);
        return;
    }
    original_system_property_get(name, value);
}

char * ( * originalStrstr)(const char * haystack, const char * needle);
char * hookedStrstr(const char * haystack, const char * needle) {
    if (strstr(haystack, "vphonegaga") != nullptr) {
        char * modifiedHaystack = strdup(haystack);
        char * occurrence = strstr(modifiedHaystack, "crash6777777");
        if (occurrence != nullptr) {
            strncpy(occurrence, "blocked", strlen("blocked"));
        }
        return originalStrstr(haystack, needle);
    }
    return originalStrstr(haystack, needle);
}

pid_t my_pid = getpid();
ssize_t( * original_send)(int sockfd, const void * buf, size_t len, int flags);
ssize_t hooked_send(int sockfd, const void * buf, size_t len, int flags) {
    return original_send(sockfd, buf, len, flags);
}

ssize_t( * original_recv)(int sockfd, void * buf, size_t len, int flags);
ssize_t( * original_recvfrom)(int sockfd, void * buf, size_t len, int flags, struct sockaddr * src_addr, socklen_t * addrlen);
ssize_t( * original_sendto)(int sockfd, const void * buf, size_t len, int flags, const struct sockaddr * dest_addr, socklen_t addrlen);
ssize_t hooked_recv(int sockfd, void * buf, size_t len, int flags) {
    return original_recv(sockfd, buf, len, flags);
}

ssize_t hooked_recvfrom(int sockfd, void * buf, size_t len, int flags, struct sockaddr * src_addr, socklen_t * addrlen) {
    return original_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

ssize_t hooked_sendto(int sockfd, const void * buf, size_t len, int flags, const struct sockaddr * dest_addr, socklen_t addrlen) {
    return original_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

void hookFunctions() {
    void * handle = dlopen("libGame.so", RTLD_LAZY);
    if (handle == NULL) {
        return;
    }
    originalSocket = (SocketFunc) dlsym(handle, "socket");
    originalConnect = (ConnectFunc) dlsym(handle, "connect");
    originalInetPton = (int( * )(int, const char * , void * )) dlsym(handle, "inet_pton");
    originalGetAddrInfo = (int( * )(const char * , const char * , const struct addrinfo * , struct addrinfo ** )) dlsym(handle, "getaddrinfo");
    originalGetHostByAddr = (struct hostent * ( * )(const void * , socklen_t, int)) dlsym(handle, "gethostbyaddr");
    originalGetHostByName = (struct hostent * ( * )(const char * )) dlsym(handle, "gethostbyname");
    original_system_property_get = (void( * )(const char * , char * )) dlsym(handle, "__system_property_get");
    originalStrstr = (char * ( * )(const char * , const char * )) dlsym(handle, "strstr");
    original_send = (ssize_t( * )(int, const void * , size_t, int)) dlsym(handle, "send");
    original_recv = (ssize_t( * )(int, void * , size_t, int)) dlsym(handle, "recv");
    original_recvfrom = (ssize_t( * )(int, void * , size_t, int, struct sockaddr * , socklen_t * )) dlsym(handle, "recvfrom");
    original_sendto = (ssize_t( * )(int, const void * , size_t, int, const struct sockaddr * , socklen_t)) dlsym(handle, "sendto");
    
    if (originalSocket != NULL) {
        DobbyHook((void * ) originalSocket, (void * ) hookedSocket, (void ** ) & originalSocket);
    }
    if (originalConnect != NULL) {
        DobbyHook((void * ) originalConnect, (void * ) hookedConnect, (void ** ) & originalConnect);
    }
    if (originalInetPton != NULL) {
    DobbyHook((void * ) originalInetPton, (void * ) hookedInetPton, (void ** ) & originalInetPton);
    }
    if (originalGetAddrInfo != NULL) {
        DobbyHook((void * ) originalGetAddrInfo, (void * ) hookedGetAddrInfo, (void ** ) & originalGetAddrInfo);
    }
    
  dlclose(handle);
}
