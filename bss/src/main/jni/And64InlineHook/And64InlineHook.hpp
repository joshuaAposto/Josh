#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void A64HookFunction(void *const symbol, void *const replace, void **result);
void *A64HookFunctionV(void *const symbol, void *const replace, void *const rwx, const uintptr_t rwx_size);

#ifdef __cplusplus
}
#endif


struct Backup3 {
    void* x, *y, *z;
};

map<string, Backup3> _CACHEDB;
void Backup(ptr pointer) {
    void* x, *y, *z;
    
    memcpy( &x, (void*)(pointer), 8);
    memcpy( &y, (void*)(pointer + 0x4 * 2), 8);
    memcpy( &z, (void*)(pointer + 0x4 * 3), 8);
    
    string o = to_string(pointer);
    
    _CACHEDB[o] = {x, y, z};
}
void Restore(ptr pointer) {
    string o = to_string(pointer);
    if (_CACHEDB.count(o)<1)
        return;
    
    auto B = _CACHEDB[o];
    Backup(pointer);
    EditMemory( (void*)(pointer), 8, &B.x );
    EditMemory( (void*)(pointer + 0x4 * 2), 8, &B.y );
    EditMemory( (void*)(pointer + 0x4 * 3), 8, &B.z );
}

void _A64HookF(void *const symbol, void *const replace, void **result) {
    //Backup((ptr)symbol);
    A64HookFunction(symbol, replace, result);
}

#define HOOKI(addr, func) _A64HookF((void*)(addr), (void *)func, (void **)&_##func)
#define HOOKNI(addr, func) _A64HookF((void*)(addr), (void *)func, NULL)

#define HOOK(addr, func) _A64HookF((void*)(il2cpp+addr), (void *)func, (void **)&_##func)
#define HOOKN(addr, func) _A64HookF((void*)(il2cpp+addr), (void *)func, NULL)

#define HOOKM(a, b, c, func) _A64HookF((void*)(Method(a, b, c, __##func)), (void *)func, (void **)&_##func); __##func=-1446
#define HOOKNM(a, b, c, d, func) _A64HookF((void*)(Method(a, b, c, d)), (void *)func, NULL)
#define H(a, b, c, d, func) _A64HookF((void*)(Method(#a, #b, #c, d)), (void *)func, NULL)
#define I(a, b, c, func) _A64HookF((void*)(Method(#a, #b, #c, __##func)), (void *)func, (void **)&_##func); __##func=-1446

#define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, count, ...) count

#define DEFINES(type, func, ...) int __##func = GET_ARG_COUNT(__VA_ARGS__); static type (*_##func)(__VA_ARGS__); static type func(__VA_ARGS__)
#define DEFINE(type, func, ...) int __##func = GET_ARG_COUNT(__VA_ARGS__); type (*_##func)(ptr, ##__VA_ARGS__); type func(ptr inst, ##__VA_ARGS__)


#define HOOKIT(library, symbol, func) do { \
    void* lib = fdlopen(library, 4); \
    LOGI("lib %s: %p", library, lib); \
    if (!lib) break; \
    void* p = fdlsym(lib, symbol); \
    LOGI("sym %s: %p", symbol, p); \
    fdlclose(lib); \
    if (p) HOOKI(p, func); \
} while(0)

#define HOOKIT2(library, sym1, func1, sym2, func2) do { \
    void* lib = fdlopen(library, 4); \
    LOGI("lib %s: %p", library, lib); \
    if (!lib) break; \
    void* p1 = fdlsym(lib, sym1); \
    LOGI("sym %s: %p", sym1, p1); \
    void* p2 = fdlsym(lib, sym2); \
    LOGI("sym %s: %p", sym2, p2); \
    fdlclose(lib); \
    if (p1 && p2) { HOOKI(p1, func1); HOOKI(p2, func2); } \
} while(0)

#define HOOKIT3(library, sym1, func1, sym2, func2, sym3, func3) do { \
    void* lib = fdlopen(library, 4); \
    LOGI("lib %s: %p", library, lib); \
    if (!lib) break; \
    void* p1 = fdlsym(lib, sym1); \
    LOGI("sym %s: %p", sym1, p1); \
    void* p2 = fdlsym(lib, sym2); \
    LOGI("sym %s: %p", sym2, p2); \
    void* p3 = fdlsym(lib, sym3); \
    LOGI("sym %s: %p", sym3, p3); \
    fdlclose(lib); \
    if (p1 && p2 && p3) { HOOKI(p1, func1); HOOKI(p2, func2); HOOKI(p3, func3); } \
} while(0)

#define HOOKIT4(library, sym1, func1, sym2, func2, sym3, func3, sym4, func4) do { \
    void* lib = fdlopen(library, 4); \
    LOGI("lib %s: %p", library, lib); \
    if (!lib) break; \
    void* p1 = fdlsym(lib, sym1); \
    LOGI("sym %s: %p", sym1, p1); \
    void* p2 = fdlsym(lib, sym2); \
    LOGI("sym %s: %p", sym2, p2); \
    void* p3 = fdlsym(lib, sym3); \
    LOGI("sym %s: %p", sym3, p3); \
    void* p4 = fdlsym(lib, sym4); \
    LOGI("sym %s: %p", sym4, p4); \
    fdlclose(lib); \
    if (p1 && p2 && p3 && p4) { HOOKI(p1, func1); HOOKI(p2, func2); HOOKI(p3, func3); HOOKI(p4, func4); } \
} while(0)
