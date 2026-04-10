#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <thread>
#include <set>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>

#include <chrono>
#include <deque>

using namespace std;

#define MY_LOG_TAG "angousana"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, MY_LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MY_LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, MY_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, MY_LOG_TAG, __VA_ARGS__)

#define ptr uintptr_t
#define DWORD uintptr_t
#define ulong uint64_t
#define dword uint32_t
#define qword uint64_t

#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24

#define WHITE              ImColor(255, 255, 255)
#define RED                ImColor(255, 0, 0)
#define GREEN              ImColor(0, 255, 0)
#define LIME               ImColor(0, 255, 0)
#define BLUE               ImColor(0, 0, 255)
#define BLACK              ImColor(0, 0, 0)
#define PURPLE             ImColor(128, 0, 128)
#define GREY               ImColor(128, 128, 128)
#define YELLOW             ImColor(255, 255, 0)
#define ORANGE             ImColor(255, 165, 0)
#define DARKGREEN          ImColor(0, 100, 0)
#define PINK               ImColor(255, 192, 203)
#define BROWN              ImColor(165, 42, 42)
#define CYAN               ImColor(0, 255, 255)

#define floop(s, e) for (int fi=s; fi<e; fi++)

void pthread(void* (*func)(void*)) {pthread_t x; pthread_create(&x, 0, func, 0);}
void sleepm(int milliseconds) {std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));}

#include <sys/mman.h>
bool EditPerm(void* addr, size_t len, int PERMISSION) {return (mprotect((void*)((uintptr_t)addr&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1)),((((uintptr_t)(uintptr_t)addr+len-1&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1)))-((uintptr_t)addr&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1))+(sysconf(_SC_PAGE_SIZE))),PERMISSION)!=-1);}
bool EditMemory(void* addr, size_t len, const void *data) {EditPerm(addr, len, (PROT_READ | PROT_WRITE | PROT_EXEC)); bool x = (memcpy(addr, data, len)!=NULL); EditPerm(addr, len, (PROT_READ | PROT_EXEC)); return x;}

uint SVOID() {return 0;}
uint VOID() {static int _I_ = 0;LOGI("VOID: %i", _I_++);return 0;}
bool TRUE() {static int _I_ = 0;LOGI("TRUE: %i", _I_++);return true;}
bool FALSE() {static int _I_ = 0;LOGI("FALSE: %i", _I_++);return false;}
int IMAX() {static int _I_ = 0;LOGI("IMAX: %i", _I_++);return 100000;}
int I0() {static int _I_ = 0;LOGI("I0: %i", _I_++);return 0;}
int I10() {static int _I_ = 0;LOGI("I10: %i", _I_++);return 10;}
int I1000() {static int _I_ = 0;LOGI("I1000: %i", _I_++);return 1000;}
float FMAX() {static int _I_ = 0;LOGI("FMAX: %i", _I_++);return 100000.0f;}
float F1000() {static int _I_ = 0;LOGI("F1000: %i", _I_++);return 1000.0f;}
float F100() {static int _I_ = 0;LOGI("F100: %i", _I_++);return 100.0f;}
float F10() {static int _I_ = 0;LOGI("F10: %i", _I_++);return 10.0f;}
float F0() {static int _I_ = 0;LOGI("F0: %i", _I_++);return 0.0f;}
float SF0() { return 0.0f; }
float SF10() { return 10.0f; }
float SF100() { return 100.0f; }
double DMAX() {static int _I_ = 0;LOGI("DMAX: %i", _I_++);return 100000.0f;}
vector<int> V3() {static int _I_ = 0;LOGI("V3: %i", _I_++);return {0,0,0};}
vector<int> V2() {static int _I_ = 0;LOGI("V2: %i", _I_++);return {0,0};}

#define BT(addr) HOOKN(addr, TRUE);
#define BF(addr) HOOKN(addr, FALSE);
#define V(addr) HOOKN(addr, VOID);

#define IM(addr) HOOKN(addr, IMAX);

#define SV(addr) HOOKN(addr, SVOID);
#define ST(addr) HOOKN(addr, STRUE);
#define SF(addr) HOOKN(addr, SFALSE);

u_long htol(string strx) {const char* str = strx.c_str(); u_long res = 0; char c; while ((c = *str++)) {char v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8); res = (res << 4) | (u_long) v;} return res;}
ptr absoluteAddress(const char *libraryName, uintptr_t offset=0) {
    bool found=false;
    LOGI("Searching %s until found...", libraryName);
    while (!found) {
        ifstream maps("/proc/self/maps");
        string map;
        while (getline(maps, map)) {
            if (strstr(map.c_str(), libraryName)) {
                LOGI("Library found! Exiting loop...");
                istringstream line(map);
                string start;
                getline(line, start, '-');
                return htol(start)+offset;
            }
        }
    }
    return 0;
}
template<typename... Args> void absoluteAddress(Args... args) {
    const char* arr[] = {args...};
    for (size_t i = 0; i < sizeof...(args); i++) {
        absoluteAddress(arr[i]);
    }
}

#define FOR(item, list) if (auto* __l = (list)) \
    if (auto __s = __l->getSize()) if (auto* __items = __l->getItems()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define FORA(item, array) if (auto* __l = (array)) \
    if (auto __s = __l->getLength()) if (auto* __items = __l->getPointer()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define F(type, offset) (*(type*)(offset))
#define M(type, offset, ...) ((type(*)(__VA_ARGS__))(offset))

#define A(x) void* x() { LOGI(#x); return 0; }
#define B(x) HOOKNI(fdlsym(libc, #x), x);

#include <sys/stat.h>

static bool create_directory_recursive(const char* path) {
    char tmp[256];
    char* p = nullptr;
    size_t len;
    
    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
        
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            *p = '/';
        }
    }
    
    return mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST;
}