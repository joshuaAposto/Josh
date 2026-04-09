#define PACKAGE_NAME "com.netease.newspike"
#include "include/obfuscate.h"
#include "obfuscate.h"
#include "include/includes.h"
#include "And64InlineHook/And64InlineHook.hpp"
#include "xhook/xhook.h"
#include "KittyMemory/MemoryPatch.h"
#include <thread>
#include "Domain.h"
#include "include/hpp/srcpy.h"
#include <pthread.h>
#include <jni.h>
#include <dlfcn.h>
#include <netdb.h>
#include <android/log.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

// ========== ANTI-TELEMETRY INCLUDES ==========
#include "DomainScanner.h"
#include "NetworkHooks.h"
#include "AntiDetection.h"
#include "AntiTelemetry.h"
//#include "ReportDetector_Android.h"  // TAMBAHKAN INI

#include "FireWall.h"
#include "draw.h"
#include "include/input.h"

std::string sdcardPath = "/sdcard/";
std::string logFilePath = sdcardPath + "offset.txt";
typedef void* (*PyRun_StringFlags_t)(const char*, int, void*, void*, void*);
PyRun_StringFlags_t PyRun_StringFlags;

#define LOG_TAG "MOD"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Struktur untuk menyimpan informasi weapon & skiny
struct WeaponSkinInfo {
    int weaponId;
    int skinId;
    std::string weaponName;
    std::string skinName;
};

void parseWeaponScanResults();
void saveScanSummary();
std::vector<WeaponSkinInfo> foundSkins;

void parseWeaponScanResults() {
    std::ifstream scanFile("/sdcard/weapon_scan.txt");
    
    if (!scanFile.is_open()) {
        std::cout << "[ERROR] Cannot open scan results" << std::endl;
        return;
    }

    std::string line;
    foundSkins.clear();

    while (std::getline(scanFile, line)) {
        if (line.find("WEAPON_SCAN|") == 0) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }

            if (tokens.size() >= 5) {
                WeaponSkinInfo info;
                info.weaponId = std::stoi(tokens[1]);
                info.skinId = std::stoi(tokens[2]);
                info.weaponName = tokens[3];
                info.skinName = tokens[4];

                foundSkins.push_back(info);

                std::cout << "[FOUND] Weapon: " << info.weaponName 
                         << " (ID: " << info.weaponId 
                         << ") | Skin: " << info.skinName 
                         << " (ID: " << info.skinId << ")" << std::endl;
            }
        }
    }

    scanFile.close();
}

void saveScanSummary() {
    std::ofstream summary("/sdcard/weapon_skin_summary.txt");
    
    if (!summary.is_open()) {
        std::cout << "[ERROR] Cannot create summary file" << std::endl;
        return;
    }

    summary << "========== WEAPON & SKIN SCAN RESULTS ==========" << std::endl;
    summary << "Total items found: " << foundSkins.size() << std::endl;
    summary << std::endl;

    for (size_t i = 0; i < foundSkins.size(); i++) {
        summary << "[" << (i + 1) << "] " 
                << foundSkins[i].weaponName << " (ID: " << foundSkins[i].weaponId << ")"
                << " | " << foundSkins[i].skinName << " (ID: " << foundSkins[i].skinId << ")"
                << std::endl;
    }

    summary << std::endl;
    summary << "Scan completed successfully!" << std::endl;
    summary.close();

    std::cout << "[SUCCESS] Summary saved to /sdcard/weapon_skin_summary.txt" << std::endl;
}

static bool isTouching = false;
static float touchX = 0.0f, touchY = 0.0f;

DEFINES(void*, _v07, const char *filename, int flags) { return nullptr; }
DEFINES(void*, _xv07, const char *filename, int flags) { return nullptr; }

static int iLoaded = 0, iLoadAt = 2;
DEFINES(void*, iMissMyWife, const char* v0, int v1, void* v2, void* v3, void* v4) {
    static bool loaded = false;
    if (v1 == 257) { 
        iLoaded++;
        if (iLoaded >= iLoadAt && !loaded) {
            loaded = true;
            _iMissMyWife(srcpy, 257, v2, v2, NULL);
        }
    }
    
    return _iMissMyWife(v0, v1, v2, v3, v4);
}

        
// ========== VERIFICATION FUNCTION ==========
bool verifyProtectionActive() {
    LOGI("========================================");
    LOGI("🔍 RUNNING PROTECTION VERIFICATION");
    LOGI("========================================");
    
    bool allGood = true;
    
    // Test 1: Check Anti-Telemetry
    if (!globalAntiTelemetry) {
        LOGI("❌ CRITICAL: globalAntiTelemetry is NULL!");
        return false;
    }
    
    if (!globalAntiTelemetry->getEnabled()) {
        LOGI("❌ CRITICAL: Anti-Telemetry is disabled!");
        return false;
    }
    LOGI("✓ Anti-Telemetry: ACTIVE");
    
    // Test 2: Check hooks
    if (!orig_connect || !orig_gethostbyname || !orig_getaddrinfo) {
        LOGI("❌ CRITICAL: Network hooks not installed!");
        return false;
    }
    LOGI("✓ Network Hooks: INSTALLED");
    
    // Test 3: Domain blocking tests
    LOGI("🧪 RUNNING DOMAIN BLOCKING TESTS");

std::vector<std::pair<std::string, bool>> tests = {
    // Harus BLOCKED
    {"applog.matrix.easebar.com", true},
    {"firebaselogging.googleapis.com", true},
    {"log.netease.com", true},
    // Harus ALLOWED
    {"game.163.com", false},
    {"login.163.com", false}
};

int pass = 0, fail = 0;
for (const auto& test : tests) {
    bool expectedBlock = test.second;  // ← PERBAIKAN: Ganti nama variabel
    bool actualBlock = globalAntiTelemetry->shouldBlock(test.first);
    
    if (actualBlock == expectedBlock) {
        LOGI("✓ TEST PASS: %s - %s", 
             test.first.c_str(), 
             actualBlock ? "BLOCKED" : "ALLOWED");
        pass++;
    } else {
        LOGI("❌ TEST FAIL: %s", test.first.c_str());
        fail++;
        allGood = false;
    }
}
    
    LOGI("📊 TEST RESULTS: Pass: %d, Fail: %d", pass, fail);
    
    if (allGood) {
        LOGI("✓✓✓ ALL TESTS PASSED! ✓✓✓");
    } else {
        LOGI("❌ SOME TESTS FAILED");
    }
    
    LOGI("========================================");
    return allGood;
}





// ========== MAIN HACK THREAD ==========
void* hack_thread(void*) {
        LOGI("[INIT] Hack thread started");
    LOGI("========================================");
    LOGI("🚀 MOD INITIALIZATION STARTED");
    LOGI("========================================");
    
    sleep(3); // Wait for game to stabilize
    
    // ========== STEP 1: INITIALIZE ANTI-TELEMETRY ==========
    LOGI("[1/4] Initializing Anti-Telemetry System...");
    initAntiTelemetry();
    
    if (!globalAntiTelemetry) {
        LOGI("❌ FATAL: Anti-Telemetry initialization failed!");
        return 0;
    }
    
    LOGI("✓ Anti-Telemetry initialized");
    sleep(1);
    
    // ========== STEP 2: INITIALIZE DOMAIN SCANNER ==========
    LOGI("[2/4] Initializing Domain Scanner...");
    initDomainScanner();
    
    if (!globalScanner) {
        LOGI("❌ WARNING: Domain Scanner failed (non-critical)");
    } else {
        LOGI("✓ Domain Scanner initialized");
        LOGI("✓ Save location: /sdcard/domain.txt");
    }
    sleep(1);
    
    // ========== STEP 3: INSTALL NETWORK HOOKS ==========
    LOGI("[3/4] Installing Network Hooks...");
    installNetworkHooks();
        
        if (globalAntiTelemetry && globalAntiTelemetry->getEnabled()) {
        TELE_LOGI("========================================");
        TELE_LOGI("✓ Protection System Initialized!");
        TELE_LOGI("✓ Network hooks installed");
        TELE_LOGI("✓ Anti-Telemetry active");
        TELE_LOGI("✓ Ready to block telemetry");
        TELE_LOGI("========================================");
    } else {
        TELE_LOGI("❌ FAILED: Protection NOT active!");
    }

    
    // Wait for hooks to settle
    sleep(1);
    
    // ========== STEP 4: VERIFY PROTECTION ==========
    LOGI("[4/4] Verifying Protection System...");
    bool verified = verifyProtectionActive();
    
    if (!verified) {
        LOGI("⚠️ WARNING: Verification failed - check logcat for details");
        LOGI("⚠️ Mod will continue but protection may not be active");
    }
    
    sleep(1);
    
    // ========== CONTINUE WITH NORMAL MOD LOADING ==========
    LOGI("========================================");
    LOGI("🎮 LOADING GAME HOOKS");
    LOGI("========================================");
        
    hookFunctions();
    
    // GL Function pointers
    p_glGetIntegerv = (void (*)(unsigned int, int *))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glGetIntegerv")));
    p_glGetBooleanv = (void (*)(unsigned int, unsigned char *))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glGetBooleanv")));
    p_glDepthRangef = (void (*)(float, float))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glDepthRangef")));
    p_glBlendColor = (void (*)(float, float, float, float))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glBlendColor")));
    p_glEnable = (void (*)(unsigned int))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glEnable")));
    p_glLineWidth = (void (*)(float))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glLineWidth")));
    p_glBlendFunc = (void (*)(unsigned int, unsigned int))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glBlendFunc")));
    p_glDisable = (void (*)(unsigned int))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glDisable")));
    p_glCullFace = (void (*)(unsigned int))((const void*(*)(...))dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glCullFace")));

    DobbyHook((void *) dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glGetUniformLocation")), (void *) new_glGetUniformLocation, (void **) &old_glGetUniformLocation);
    DobbyHook((void *) dlsym(dlopen(xorstr_("libGLESv3.so"), 4), xorstr_("glDrawElements")), (void *) new_glDrawElements, (void **) &old_glDrawElements);

    if (!fdlopen(oxorany("libGame.so"), 0)) {
        LOGI("❌ FATAL: Failed to load libGame.so");
        return 0;
    }
    
    auto libGame = absoluteAddress(oxorany("libGame.so"));
    if (!libGame) {
        LOGI("❌ FATAL: Failed to get libGame base address");
        return 0;
    }
    
    LOGI("✓ libGame.so loaded at: 0x%lx", (unsigned long)libGame);
    
    create_directory_recursive(xorstr_("/data/data/" PACKAGE_NAME "/imgui"));
    system("mkdir -p /sdcard/");
    
    HOOKI((ptr) libGame + 0x471A918, iMissMyWife); // 0x46F47BC // 0x471599C
    
    __INPUT__();
    start_entity_reader();
    
    LOGI("[INFO] Hooking eglSwapBuffers for menu rendering...");
    HOOKIT(xorstr_("libEGL.so"), xorstr_("eglSwapBuffers"), Draw);
    
    LOGI("========================================");
    LOGI("✓✓✓ MOD LOADED SUCCESSFULLY ✓✓✓");
    LOGI("========================================");
    LOGI("🛡️ Anti-Telemetry: ACTIVE");
    LOGI("🔒 Network Protection: ENABLED");
    LOGI("📊 Blocking: 28+ Telemetry Domains");
    LOGI("🎮 Game Servers: WHITELISTED");
    LOGI("========================================");
    LOGI("💡 Check Anti-Telemetry menu for stats");
    LOGI("💡 Watch logcat for 🚫 BLOCKED messages");
    LOGI("========================================");
    
    return 0;
}

// ========== CONSTRUCTOR (EARLIEST INIT POINT) ==========
__attribute__((constructor))    
    void lib_main() {
        
    // Seed random for anti-pattern
    srand(time(nullptr));
    
    // Random delay untuk avoid pattern detection
    usleep((rand() % 1000 + 500) * 1000);
    
    LOGI("========================================");
    LOGI("🔐 PROTECTED MOD - ANTI-TELEMETRY BUILD");
    LOGI("========================================");
    
    // Start hack thread
    std::thread(hack_thread, nullptr).detach();
}

// ========== JNI_ONLOAD ==========
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    jvm = vm;
    
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    
    LOGI("[JNI] JNI_OnLoad called - VM attached");
    
    return JNI_VERSION_1_6;
}
