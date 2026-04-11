
#ifndef NETWORKHOOKS_H
#define NETWORKHOOKS_H

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <algorithm>
#include "DomainScanner.h"
#include "xhook/xhook.h"

// Original function pointers
extern int (*orig_connect)(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
extern struct hostent* (*orig_gethostbyname)(const char* name);
extern int (*orig_getaddrinfo)(const char* node, const char* service, 
                               const struct addrinfo* hints, struct addrinfo** res);

// Function declarations
bool isTargetDomain(const std::string& domain);
int hook_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
struct hostent* hook_gethostbyname(const char* name);
int hook_getaddrinfo(const char* node, const char* service,
                    const struct addrinfo* hints, struct addrinfo** res);
void installNetworkHooks();

#endif








/*

#ifndef NETWORKHOOKS_H
#define NETWORKHOOKS_H

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include "DomainScanner.h"
#include "xhook/xhook.h"  // TAMBAHKAN INI

// Original function pointers
extern int (*orig_connect)(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
extern struct hostent* (*orig_gethostbyname)(const char* name);
extern int (*orig_getaddrinfo)(const char* node, const char* service, 
                               const struct addrinfo* hints, struct addrinfo** res);

// Hook implementations
int hook_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
struct hostent* hook_gethostbyname(const char* name);
int hook_getaddrinfo(const char* node, const char* service,
                    const struct addrinfo* hints, struct addrinfo** res);

void installNetworkHooks();

#endif

*/

