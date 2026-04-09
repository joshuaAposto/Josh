#define PACKAGE_NAME "com.netease.newspike"

#include "include/obfuscate.h"
#include "include/includes.h"
#include "And64InlineHook/And64InlineHook.hpp"
#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyScanner.h"
#include "KittyMemory/KittyUtils.h"

#include "xhook/xhook.h"

#include "include/hpp/srcpy.h"

// ========== ANTI-TELEMETRY / ANTI-DETECTION ==========
#include "DomainScanner.h"
#include "NetworkHooks.h"
#include "AntiDetection.h"
#include "AntiTelemetry.h"
#include "block.h"
#include "FireWall.h"


DEFINES(void*, _v07, const char *filename, int flags) { return nullptr; }
DEFINES(void*, _xv07, const char *filename, int flags) { return nullptr; }

static int iLoaded = 0, iLoadAt = 2;
DEFINES(void*, o44ee9f4, const char* v0, int v1, void* v2, void* v3, void* v4) {
    static bool loaded = false;
    if (v1 == 257) { iLoaded ++;
        if (iLoaded >= iLoadAt && !loaded) {
            loaded = true;
            _o44ee9f4(srcpy, 257, v2, v2, NULL);
        }
    }
    
    return _o44ee9f4(v0, v1, v2, v3, v4);
}

#include "draw.h"
#include "include/input.h"


void *__1__(void *) {
    sleep(3);
    
    // ========== PROTECTION SYSTEM INIT ==========
    initAntiTelemetry();
    initDomainScanner();
    installNetworkHooks();
    install_domain_blocker();
    sleep(1);
    initAntiDetection();
    hookFunctions();
    // ============================================
    
    if (!fdlopen("libGame.so", 0)) return 0;
    
    auto libGame = absoluteAddress("libGame.so");
    if (!libGame) return 0;
    
    create_directory_recursive("/data/data/" PACKAGE_NAME "/imgui");
    
    HOOKI((ptr) libGame + 0x471A918, o44ee9f4);
     
 //   var.Feature1 = MemoryPatch::createWithHex(libGame + 0x16B28C4, "00 00 C6 42");
    __IMGUI__();
    __INPUT__();
    
    LOGI("RETURNING NOW MY GODDY GOD");
    return 0;
}

__attribute__((constructor))
void __0__() { pthread(__1__); }
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;    
    jvm = vm;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}

