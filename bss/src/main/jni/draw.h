

#include "include/hpp/imgui.h"
#include <EGL/egl.h>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <curl/curl.h>
#include <sys/system_properties.h>
int Width, Height;
using namespace ImGui;
using namespace std;
#include "xorstr.hpp"
const char* INI_PATH = xorstr_("/data/data/" PACKAGE_NAME "/imgui/.ini");
#include <vector>
#include <jni.h>
#include <android/native_activity.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android/input.h>
#include "curl/json.hpp"
#include "Jni_Stuff.h"
#include <GLES2/gl2.h>
#include "roominfofont.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include "Imgui/DreiFont.h"
#include "include/obfuscate.h"
#include "include/hpp/dlfcn.hpp"
#include "include/hpp/imgui/persistence.h"
#include "include/hpp/imgui/touch.h"
#include "include/sync_with_py.h"
#include "include/sync_entities.h"
#include "include/helpers.h"
#include "variables.h"
#include "RGB.h"
#include "Toggle.h"
#include "imgui/stb_image.h"
#include "Starcool.h"
#include "Chams.h"
#include "imgui/imgui_additional.h"
#include "Oxorany/oxorany.h"
#include "Imgui/Icon.h"
#include "xorstr.hpp"
#include "Imgui/Iconcpp.h"

#include "CustomImGui.h"      // ✅ TAMBAHKAN INI
#include "WaveModsConfig.h"   // ✅ TAMBAHKAN INI
#include "WaveMods/Icon.h"
#include "WaveMods/CN.h"

#include <GLES3/gl3.h>

#include "AntiTelemetry.h"     // KEDUA - bisa pakai RealTimeMonitor
#include "DomainScanner.h"     // KETIGA
#include "NetworkHooks.h"

#include <fcntl.h>
#include <dirent.h>

#define PERM_EXEC "r-xp"
#define targetaLibName "libGame.so"
char packageName[] = "com.netease.newspike";
int game_pid = -1;
uint64_t lib_base = 0;

void applyBypassPatch() {
    // Pastikan patch hanya dijalankan sekali atau sesuai kebutuhan
    static bool patched = false;
    if (patched) return;

    // Contoh patch
    MemoryPatch::createWithHex("libz.so", 0x12E5D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E5E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E628, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E668, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E6A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E6E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E728, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E768, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E7A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E7E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E828, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E868, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E880, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E898, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E8D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E908, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E940, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E958, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E998, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12E9D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EA10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EA28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EA90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EAC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EAE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EAF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EB88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EBC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EC98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12ECD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12ECE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12ED00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12ED58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12ED98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EF40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EF78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EF90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12EFA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F000, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F040, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F080, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F098, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F0B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F0C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F100, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F118, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F1C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F208, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F248, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F260, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F278, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F2B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F2F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F350, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F390, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F3A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F408, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F420, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F438, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F478, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F5C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F5D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F5F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F608, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F6E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F6F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F738, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F750, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F920, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F938, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F950, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F968, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F980, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F998, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12F9B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FA08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FA60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FAF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FB10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FB28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FB60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FBE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FC98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FCB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FD98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FDB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FDE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FF30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FF48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FF60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FFB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FFD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x12FFE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x130028, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x130040, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x130058, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1300B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1300D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1302E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x132A90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x13BD08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x13DD88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x141D40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1421F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144120, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1449F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x1449F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144A98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144AF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144B98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144BF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144C98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144CF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144D98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144DF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144E98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144ED0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144ED8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144EF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144F98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x144FF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x21F0B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x21FCD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2526F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252700, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252708, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252710, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252718, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252720, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252728, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252730, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252738, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252740, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252748, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252750, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252758, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252760, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252768, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252770, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252778, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252780, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252788, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252790, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252798, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2527F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252800, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252808, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252810, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252818, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252820, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252828, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252830, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252838, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252840, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252848, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252850, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252858, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252860, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252868, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252870, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252878, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252880, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252888, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252890, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252898, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2528F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252900, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252908, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252910, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252918, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252920, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252928, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252930, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252938, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252940, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252948, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252950, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252958, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252960, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252968, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252970, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252978, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252980, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252988, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252990, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252998, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2529F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252A98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252AF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252B98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252BF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252C98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252CF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252D98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252DF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252E98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252ED0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252ED8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252EF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F20, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F28, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F30, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F38, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F40, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F48, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F50, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F58, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F60, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F68, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F70, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F78, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F80, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F88, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F90, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252F98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FC0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FC8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FD0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FD8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FE0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FE8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FF0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x252FF8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x278010, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x278068, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x278070, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x278078, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E548, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E550, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E558, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E560, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E568, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E570, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x29E578, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2C28B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5068, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5070, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5078, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5A00, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5A08, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5A10, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2E5A18, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA5E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA5F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA5F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA600, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA608, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA610, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA618, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA620, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA628, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA630, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FA638, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FB678, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FCF98, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FCFA0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FCFA8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FCFB0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FCFB8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FF668, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FF670, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x2FF678, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x300780, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3912F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391300, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391308, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391310, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391318, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391320, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391328, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391330, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391338, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391340, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391348, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391350, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391358, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391360, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391368, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391370, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391378, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391380, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391388, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391390, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391398, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3913F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391400, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391408, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391410, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391418, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391420, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391428, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391430, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391438, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391440, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391448, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391450, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391458, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391460, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391468, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391470, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391478, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391480, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391488, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391490, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391498, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3914F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391500, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391508, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391510, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391518, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391520, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391528, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391530, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391538, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391540, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391548, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391550, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391558, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391560, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391568, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391570, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391578, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391580, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391588, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391590, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391598, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3915F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391600, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391608, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391610, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391618, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391620, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391628, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391630, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391638, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391640, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391648, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391650, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391658, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391660, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391668, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391670, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391678, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391680, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391688, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391690, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391698, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3916F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391700, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391708, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391710, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391718, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391720, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391728, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391730, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391738, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391740, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391748, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391750, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391758, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391760, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391768, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391770, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391778, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391780, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391788, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391790, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391798, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3917F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391800, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391808, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391810, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391818, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391820, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391828, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391830, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391838, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391840, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391848, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391850, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391858, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391860, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391868, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391870, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391878, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391880, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391888, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391890, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391898, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918B0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918B8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918C0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918C8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918D0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918D8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918E0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918E8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918F0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3918F8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391900, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391908, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391910, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391918, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391920, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391928, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391930, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391938, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391940, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391948, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391950, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391958, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391960, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391968, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391970, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391978, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391980, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391988, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391990, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x391998, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3919A0, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3919A8, "00 00 00 00 00 00 00 00").Modify();
MemoryPatch::createWithHex("libz.so", 0x3919B0, "00 00 00 00 00 00 00 00").Modify();
            

    patched = true;
}



#include <chrono>  // TAMBAHKAN INI untuk time tracking
//#include "gui_helpers.h"
// Lalu deklarasi global instances

#include <ctime>     // Added for expiration check
#include <cstdlib>   // Added for exit(0)
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#include <cstdlib>
#endif

// GL Function Pointers
extern void (*p_glGetIntegerv)(unsigned int, int*);
extern void (*p_glGetBooleanv)(unsigned int, unsigned char*);
extern void (*p_glDepthRangef)(float, float);
extern void (*p_glBlendColor)(float, float, float, float);
extern void (*p_glEnable)(unsigned int);
extern void (*p_glLineWidth)(float);
extern void (*p_glBlendFunc)(unsigned int, unsigned int);
extern void (*p_glDisable)(unsigned int);
extern void (*p_glCullFace)(unsigned int);
/*
// Global license system instance
LicenseSystem* g_licenseSystem = nullptr;
*/


        

#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24
#define ReadPointer(type, address) (*(type*)(address))
#define GetFunction(type, address, ...) ((type(*)(__VA_ARGS__))(address))
bool Zaijan = true;
#define CREATE_COLOR(r, g, b, a) new float[4]{(float)r, (float)g, (float)b, (float)a};
static int touchFingerID = -1;


static ImFont* F50 = nullptr;
static ImFont* Subtab = nullptr;
static ImFont* TELEGRAM = nullptr;
static ImFont* logoicon = nullptr;
static ImFont* F48 = nullptr;

static bool showMainMenu = false;


static GLuint menuIconTexture = 0;
static int iconW = 0, iconH = 0;

bool showMenu = false;
static bool bAimTouch = false;

static int   bss_vip_days_remaining = 0;
static bool  bss_vip_days_received  = false;
static char  bss_vip_username_str[64] = "";

static JNIEnv* g_env = nullptr;
static jobject g_context = nullptr;

    // PERBAIKI INI - pindahkan static ke luar function atau buat global
    static bool showBlocked = true;
    static bool showAllowed = true;
    static bool autoScroll = true;  // TAMBAHKAN INI
        
        /*
        // UI State
static char keyInput[256] = "";
static std::string statusMessage = "";
static bool showMainMenu = false;

*/

static int inject_pid = -1;
static uint64_t inject_base = 0;
static bool inject_done = false;



// ========== GET PROCESS ID ==========
int GetGamePID(const char *packageName) {
    int id = -1;
    DIR *dir = opendir("/proc");
    if (!dir) return -1;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            char filename[64], cmdline[64];
            sprintf(filename, "/proc/%d/cmdline", id);
            FILE *fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0) {
                    closedir(dir);
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

// ========== GET MODULE BASE ==========
uint64_t GetModuleBase(int pid, const char* module_name) {
    char path[64], line[1024];
    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    FILE* maps = fopen(path, "rt");
    if (!maps) return 0;

    uint64_t base = 0;
    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, module_name) && strstr(line, "r-xp")) {
            uint64_t start, end;
            sscanf(line, "%lx-%lx", &start, &end);
            if ((end - start) < 0x5100000) {
                base = start;
                break;
            }
        }
    }
    fclose(maps);
    return base;
}




// ========== BAGIAN 4: FUNGSI INJECT TOUCH (Baris 104+) ==========
void inject_touch_move(float deltaX, float deltaY) {
    sync_float["fAIM_DeltaX"] = deltaX;
    sync_float["fAIM_DeltaY"] = deltaY;
    sync_bool["bAIM_DoSnap"] = true;
}

/*
void InitLicenseSystem() {
    g_licenseSystem = new LicenseSystem("https://uciha.gamer.gd/api/verify_key.php");
}
*/



 // Tambahkan fungsi untuk draw bendera sederhana
void DrawFlag(ImDrawList* draw, ImVec2 pos, ImU32 color, float size = 12.0f) {
    // Tiang bendera
    draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 1.5f, pos.y + size), IM_COL32(150, 150, 150, 255));
    
    // Bendera persegi
    draw->AddRectFilled(
        ImVec2(pos.x + 1.5f, pos.y), 
        ImVec2(pos.x + 1.5f + size * 0.8f, pos.y + size * 0.6f), 
        color, 
        1.5f
    );
}


void BeginGroupPanel(const std::string& title, int flags = 0, const ImVec2& size = ImVec2(0,0));
// Tambahkan juga helper function untuk BeginGroupPanel jika belum ada
//namespace ImGui {
    void BeginGroupPanel(const char* name, const ImVec2& size) {
        ImGui::BeginGroup();
        
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        
        auto frameHeight = ImGui::GetFrameHeight();
        ImGui::BeginGroup();
        
        ImVec2 effectiveSize = size;
        if (size.x < 0.0f)
            effectiveSize.x = ImGui::GetContentRegionAvail().x;
        else
            effectiveSize.x = size.x;
        
        ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));
        
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(name);
        auto labelMin = ImGui::GetItemRectMin();
        auto labelMax = ImGui::GetItemRectMax();
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
        ImGui::BeginGroup();
        
        ImGui::PopStyleVar(2);
        
        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x -= frameHeight;
        
        auto itemWidth = ImGui::CalcItemWidth();
        ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));
    }
    
    void EndGroupPanel() {
        ImGui::PopItemWidth();
        
        auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        
        auto frameHeight = ImGui::GetFrameHeight();
        
        ImGui::EndGroup();
        
        ImGui::EndGroup();
        
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));
        
        ImGui::EndGroup();
        
        auto itemMin = ImGui::GetItemRectMin();
        auto itemMax = ImGui::GetItemRectMax();
        
        ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
        ImGui::GetWindowDrawList()->AddRect(
            itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f),
            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
            halfFrame.x);
        
        ImGui::PopStyleVar(2);
        
        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x += frameHeight;
        
        ImGui::Dummy(ImVec2(0.0f, 0.0f));
        
        ImGui::EndGroup();
    }
                
        
        
        
void checkProtectionStatus();
bool verifyProtectionActive();

void checkProtectionStatus() {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "🔍 System Status Check:");
    ImGui::Spacing();
    
    bool allGood = true;
    
    bool antiTelemetryOk = globalAntiTelemetry && globalAntiTelemetry->getEnabled();
    bool status1 = antiTelemetryOk;
    ImGui::Checkbox("##AntiTelemetry", &status1);
    ImGui::SameLine();
    if (antiTelemetryOk) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Anti-Telemetry: ACTIVE");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "✗ Anti-Telemetry: FAILED");
        allGood = false;
    }
    
    bool hooksOk = orig_connect && orig_gethostbyname && orig_getaddrinfo;
    bool status2 = hooksOk;
    ImGui::Checkbox("##NetworkHooks", &status2);
    ImGui::SameLine();
    if (hooksOk) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Network Hooks: INSTALLED");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "✗ Network Hooks: FAILED");
        allGood = false;
    }
    
    int totalBlocked = globalAntiTelemetry ? globalAntiTelemetry->getTotalBlocked() : 0;
    bool blockingActive = totalBlocked > 0;
    bool status3 = blockingActive;
    ImGui::Checkbox("##BlockingActive", &status3);
    ImGui::SameLine();
    if (totalBlocked > 0) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                          "✓ Blocking Active: %d requests blocked", totalBlocked);
    } else {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), 
                          "⚠ No blocks yet (waiting for telemetry)");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    if (allGood) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                          "✓✓✓ PROTECTION WORKING! ✓✓✓");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
                          "❌ PROTECTION FAILED - CHECK LOGCAT!");
    }
}





////////////T//////////
void drawAntiTelemetryMenu() {
    if (!globalAntiTelemetry) return;
        }


        /////////////
        
   void drawDomainScannerMenu() {
    if (!globalScanner) {
        LOGI("globalScanner is NULL!");
        return;
    }
    }
        
        
        
        
        // ========== AUTO INJECT (HANYA AMBIL PID & BASE) ==========
void AutoInject() {
    if (inject_done) return;
    
    // Get PID
    inject_pid = GetGamePID("com.netease.newspike");
    if (inject_pid <= 0) {
        LOGI("[Inject] Game not found");
        return;
    }
    LOGI("[Inject] ✅ PID: %d", inject_pid);
    
    // Get Base
    inject_base = GetModuleBase(inject_pid, "libGame.so");
    if (inject_base == 0) {
        LOGI("[Inject] ❌ Base not found");
        return;
    }
    LOGI("[Inject] ✅ Base: 0x%lx", inject_base);
    
    // Update global variables yang sudah ada
    game_pid = inject_pid;
    lib_base = inject_base;
    
    inject_done = true;
    LOGI("[Inject] ✅ INJECT SUCCESS - Ready to use!");
}

// ========== RESET INJECT ==========
void ResetInject() {
    inject_done = false;
    inject_pid = -1;
    inject_base = 0;
    game_pid = -1;
    lib_base = 0;
    LOGI("[Inject] Reset - ready for new game session");
}

// ========== CHECK IF GAME RUNNING ==========
bool IsGameRunning() {
    if (inject_pid <= 0) return false;
    
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d", inject_pid);
    return (access(path, F_OK) == 0);
}

// ========== SMART AUTO-INJECT ==========
void SmartAutoInject() {
    // Reset jika game mati
    if (inject_done && !IsGameRunning()) {
        ResetInject();
    }
    
    // Auto inject jika belum
    if (!inject_done) {
        AutoInject();
    }
}
        
        
        
        
// ========== GANTI DENGAN KODE INI ==========

// Variabel global
struct My_Patches {
    MemoryPatch Recoil, Magic, Assist, Smooth;
} hexPatches;

static bool recoil = false;
static bool assist = false;
static bool smooth = false;
static bool isPlayerBotEnabled = false;
static float magicBulletRadius = 100.0f;
static bool magicBulletDrawFov = false;


// ========== LOCK-ON AIMBOT FUNCTION ==========
static bool lockOnEnabled = false;
static Entity* lockedTarget = nullptr;
static float lockSmoothness = 100.0f; // Dikurangi dari 0.3f ke 0.15f = lebih sticky
static bool lockPrediction = true; // Prediksi gerakan target
static float predictionStrength = 100.0f; // Strength prediksi



void applyLockOnAim(Entity& target) {
    if (!lockOnEnabled) return;
    if (!sync_bool["bAIM"]) return;
    
    // Validate target
    if (!target.is_on_screen) {
        lockedTarget = nullptr;
        return;
    }
    
    if (target.is_knocked && sync_bool["bAIM_IgnoreKnocked"]) {
        lockedTarget = nullptr;
        return;
    }
    
    // Check visibility if required
    if (sync_bool["bAIM_CheckVisibility"] && !target.is_visible) {
        lockedTarget = nullptr;
        return;
    }
    
    // Get target bone
    std::string targetBone = "head";
    int boneIndex = sync_int["iAIM_TargetBone"];
    
    switch(boneIndex) {
        case 0: targetBone = "head"; break;
        case 1: targetBone = "spine"; break;
        case 2: targetBone = "spine1"; break;
        default: targetBone = "head"; break;
    }
    
    if (target.bones.find(targetBone) == target.bones.end()) {
        targetBone = "head";
    }
    
    // Get bone position
    auto bonePos = target.bones.at(targetBone);
    ImVec2 screenPos = ToImVec2(bonePos);
    
    // ========== TAMBAHKAN MOVEMENT PREDICTION ==========
    static ImVec2 lastTargetPos = screenPos;
    static auto lastUpdateTime = std::chrono::steady_clock::now();
    
    auto currentTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastUpdateTime).count();
    
    // Calculate velocity (pixels per second)
    ImVec2 velocity;
    velocity.x = (screenPos.x - lastTargetPos.x) / deltaTime;
    velocity.y = (screenPos.y - lastTargetPos.y) / deltaTime;
    
    // Predict future position
    ImVec2 predictedPos = screenPos;
    if (lockPrediction && deltaTime > 0.001f) {
        // Predict 0.1 seconds ahead (adjustable)
        float predictionTime = 0.1f * predictionStrength;
        predictedPos.x += velocity.x * predictionTime;
        predictedPos.y += velocity.y * predictionTime;
    }
    
    // Update tracking data
    lastTargetPos = screenPos;
    lastUpdateTime = currentTime;
    
    // Calculate delta from center (using predicted position)
    float centerX = sync_float["fWidth"] * 0.5f;
    float centerY = sync_float["fHeight"] * 0.5f;
    
    float deltaX = predictedPos.x - centerX;
    float deltaY = predictedPos.y - centerY;
    
    // Apply smoothness (LOWER = MORE STICKY)
    // 0.05 = super sticky, 0.5 = smooth
    deltaX *= lockSmoothness;
    deltaY *= lockSmoothness;
    
    // Apply aim adjustment
    sync_float["fAIM_DeltaX"] = deltaX;
    sync_float["fAIM_DeltaY"] = deltaY;
    sync_bool["bAIM_DoSnap"] = true;
    
    LOGI("[LOCK-ON] Locked to %s | Vel: %.1f,%.1f | Delta: %.1f,%.1f", 
         target.entity_name.c_str(), velocity.x, velocity.y, deltaX, deltaY);
}


// ========== FIND LOCK-ON TARGET ==========
Entity* findLockOnTarget(std::vector<Entity>& entities) {
    if (!lockOnEnabled) return nullptr;
    
    ImVec2 screenCenter(Width / 2, Height / 2);
    float closestDistance = sync_float["fAIM_Fov"];
    Entity* bestTarget = nullptr;
    
    for (auto& e : entities) {
        if (!e.is_on_screen) continue;
        if (e.bones.empty() || e.bones.find("head") == e.bones.end()) continue;
        
        // Check knocked
        if (sync_bool["bAIM_IgnoreKnocked"] && e.is_knocked) continue;
        
        // Check bots
        if (sync_bool["bAIM_IgnoreBots"] && e.is_bot) continue;
        
        // Check visibility
        if (sync_bool["bAIM_CheckVisibility"] && !e.is_visible) continue;
        
        // Calculate distance to crosshair
        ImVec2 headPos = ToImVec2(e.bones.at("head"));
        float dx = headPos.x - screenCenter.x;
        float dy = headPos.y - screenCenter.y;
        float distance = sqrt(dx * dx + dy * dy);
        
        if (distance < closestDistance) {
            closestDistance = distance;
            bestTarget = &e;
        }
    }
    
    return bestTarget;
}



// ========== FUNGSI noMore() YANG SUDAH DIPERBAIKI ==========

void noMore(ImDrawList* draw) {
    if (!sync_bool["bESP"]) return;

    auto all_entities = get_entities();
        
    std::vector<Entity> valid_entities = all_entities;
    
    std::sort(valid_entities.begin(), valid_entities.end(),
        [](const Entity& a, const Entity& b) {
            float za = a.bones.at("head")[2];
            float zb = b.bones.at("head")[2];
            return za < zb;
        }
    );

    int max_entities = 20;
    if (valid_entities.size() > max_entities) {
        valid_entities.resize(max_entities);
    }

    int players = 0, bots = 0;
        
    for (const Entity& e : valid_entities) {
        if (e.is_bot) bots++;
        else players++;
                
                        
        if (!e.is_on_screen) continue;
        
        ImVec2 From;
        if (isPlayerBotEnabled) {
            float centerX = sync_float["fWidth"] * 0.5f;
            float topY = sync_float["fHeight"] * 0.08f;
            float iconHeight = 35.f;
            From = ImVec2(centerX, topY + iconHeight);
        } else {
            switch (sync_int["iESP_Point"]) {
                case 0: From = ImVec2(Width / 2, Height); break;
                case 1: From = ImVec2(Width / 2, Height / 2); break;
                case 2: From = ImVec2(Width / 2, 0); break;
            }
        }

        if (sync_bool["bESP_Skeleton"]) {
            auto head = ToImVec2(e.bones.at("head"));
            auto spine = ToImVec2(e.bones.at("spine"));
            auto spine1 = ToImVec2(e.bones.at("spine1"));
            auto limbs_r_upperarm = ToImVec2(e.bones.at("limbs_r_upperarm"));
            auto limbs_r_forearm = ToImVec2(e.bones.at("limbs_r_forearm"));
            auto limbs_r_hand = ToImVec2(e.bones.at("limbs_r_hand"));
            auto limbs_r_thigh = ToImVec2(e.bones.at("limbs_r_thigh"));
            auto limbs_r_calf = ToImVec2(e.bones.at("limbs_r_calf"));
            auto limbs_l_upperarm = ToImVec2(e.bones.at("limbs_l_upperarm"));
            auto limbs_l_forearm = ToImVec2(e.bones.at("limbs_l_forearm"));
            auto limbs_l_hand = ToImVec2(e.bones.at("limbs_l_hand"));
            auto limbs_l_thigh = ToImVec2(e.bones.at("limbs_l_thigh"));
            auto limbs_l_calf = ToImVec2(e.bones.at("limbs_l_calf"));
            auto limbs_r_foot = ToImVec2(e.bones.at("limbs_r_foot"));
            auto limbs_l_foot = ToImVec2(e.bones.at("limbs_l_foot"));
            auto limbs_r_toe = ToImVec2(e.bones.at("limbs_r_toe"));
            auto limbs_l_toe = ToImVec2(e.bones.at("limbs_l_toe"));

            ImVec2 chest((limbs_r_upperarm.x + limbs_l_upperarm.x) / 2, 
                        (limbs_r_upperarm.y + limbs_l_upperarm.y) / 2);
            
            auto linecolor = e.is_visible ? colors["cESP_Skeleton"] : colors["cESP_SkeletonHidden"];
            auto thickness = sync_float["fESP_LineThickness"];
            
            draw->AddLine(head, chest, linecolor, thickness);
            draw->AddLine(chest, spine, linecolor, thickness);
            draw->AddLine(chest, limbs_r_upperarm, linecolor, thickness);
            draw->AddLine(chest, limbs_l_upperarm, linecolor, thickness);
            draw->AddLine(limbs_r_upperarm, limbs_r_forearm, linecolor, thickness);
            draw->AddLine(limbs_l_upperarm, limbs_l_forearm, linecolor, thickness);
            draw->AddLine(limbs_r_forearm, limbs_r_hand, linecolor, thickness);
            draw->AddLine(limbs_l_forearm, limbs_l_hand, linecolor, thickness);
            draw->AddLine(spine, limbs_l_thigh, linecolor, thickness);
            draw->AddLine(spine, limbs_r_thigh, linecolor, thickness);
            draw->AddLine(limbs_l_thigh, limbs_l_calf, linecolor, thickness);
            draw->AddLine(limbs_r_thigh, limbs_r_calf, linecolor, thickness);
            draw->AddLine(limbs_r_calf, limbs_r_foot, linecolor, thickness);
            draw->AddLine(limbs_l_calf, limbs_l_foot, linecolor, thickness);
            draw->AddLine(limbs_r_foot, limbs_r_toe, linecolor, thickness);
            draw->AddLine(limbs_l_foot, limbs_l_toe, linecolor, thickness);
        }
        
        
          ImVec2 head = ToImVec2(e.bones.at("head"));
          ImVec2 To = ImVec2(head.x, head.y - 50.0f);
    //      ImVec2 To(ToImVec2(e.bones.at("head")));
        
if (e.is_bot) {
    if (sync_bool["bESP_LineBots"]) {
        draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
    }
} else {
    if (sync_bool["bESP_Line"]) {
        draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
    }
}

// Magic Bullet Indicator
        if (sync_bool["bMagic"] && e.is_visible) {
            ImVec2 screenCenter(Width / 2, Height / 2);
            ImVec2 entityHead = ToImVec2(e.bones.at("head"));
            
            float dx = entityHead.x - screenCenter.x;
            float dy = entityHead.y - screenCenter.y;
            float distance = sqrt(dx * dx + dy * dy);
            
            if (distance <= magicBulletRadius) {
                draw->AddCircle(entityHead, 15.0f, IM_COL32(255, 100, 0, 200), 16, 2.0f);
                draw->AddText(ImVec2(entityHead.x + 20, entityHead.y - 10), 
                             IM_COL32(255, 100, 0, 255), "MAGIC");
            }
        }


  if ((sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"] || sync_bool["bESP_Distance"] || sync_bool["bESP_Health"]) && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float distance = 1000.0f / height;
    if (distance > 300) distance = 300;

    if (sync_bool["bESP_Distance"]) {
        std::string distanceText = std::to_string((int)distance) + "m";
        ImVec2 textSize = ImGui::CalcTextSize(distanceText.c_str());
        ImVec2 textPos = ImVec2(feet.x - textSize.x / 2, feet.y + 5.0f);
        draw->AddText(textPos, IM_COL32(0, 255, 0, 255), distanceText.c_str());
    }
    
    float cursorX = head.x;
    float currentY = head.y - 45.0f;

    float bar_width = 170.0f; 
    float bar_height = 35.0f;
    float startX = cursorX - bar_width / 2;

    ImVec2 barTopLeft = {startX, currentY};
    ImVec2 barBottomRight = {startX + bar_width, currentY + bar_height};
    draw->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 180), 3.0f);

    if (sync_bool["bESP_Health"]) {
        float armor_percent = e.max_armor > 0.0f ? std::min(e.is_armor / e.max_armor, 1.0f) : 0.0f;
        float health_percent = 0.0f;
        if (armor_percent < 1.0f) {
            float max_health = e.max_health > 0.0f ? e.max_health : 125.0f;
            health_percent = std::min(e.is_health / max_health, 1.0f);
        }

        float armor_width = bar_width * armor_percent;
        float health_width = bar_width * (1.0f - armor_percent) * health_percent;

        float hpBarHeight = 5.0f;
        float hpBarY = currentY + 2.0f;

        draw->AddRectFilled(
            {startX, hpBarY},
            {startX + armor_width, hpBarY + hpBarHeight},
            IM_COL32(138, 43, 226, 255), 2.0f);
        
        if (armor_percent < 1.0f && health_percent > 0.0f) {
            ImU32 hp_color = e.is_knocked ? IM_COL32(255, 0, 0, 255) : (health_percent > 0.5f ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 255, 0, 255));
            draw->AddRectFilled(
                {startX + armor_width, hpBarY},
                {startX + armor_width + health_width, hpBarY + hpBarHeight},
                hp_color, 2.0f);
        }
    }

    // BAGIAN BARU: Bendera + TeamID + Name
    if (sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"]) {
        float contentStartX = startX + 5.0f; // Padding kiri
        float contentY = currentY + (bar_height - 12.0f) / 2.0f; // Center vertical
        
        // Tentukan warna bendera berdasarkan team
        ImU32 flagColor;
        switch(e.team_id) {
            case 0: flagColor = IM_COL32(255, 50, 50, 255); break;   // Merah
            case 1: flagColor = IM_COL32(50, 150, 255, 255); break;  // Biru
            case 2: flagColor = IM_COL32(50, 255, 50, 255); break;   // Hijau
            case 3: flagColor = IM_COL32(255, 255, 50, 255); break;  // Kuning
            default: flagColor = IM_COL32(200, 200, 200, 255); break; // Abu-abu
        }
        
        // Draw bendera di kiri
        DrawFlag(draw, ImVec2(contentStartX, contentY), flagColor, 12.0f);
        
        // Offset untuk text setelah bendera
        float textStartX = contentStartX + 18.0f;
        
        // Build text
        std::string fullText;
        if (sync_bool["bESP_TeamID"])
            fullText += std::to_string(e.team_id);
        if (sync_bool["bESP_TeamID"] && sync_bool["bESP_Name"])
            fullText += " | ";
        if (sync_bool["bESP_Name"])
            fullText += e.entity_name;
        if (e.is_knocked)
            fullText += " [DOWN]";

        ImVec2 textSize = ImGui::CalcTextSize(fullText.c_str());
        ImVec2 textPos = ImVec2(textStartX, currentY + (bar_height - textSize.y) / 2.0f);
        ImU32 textColor = e.is_knocked ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
        draw->AddText(textPos, textColor, fullText.c_str());
    }

    currentY += bar_height + 4.0f;
}
   


if (sync_bool["bESP_Box"]) {
    ImVec2 head = ToImVec2(e.bones.at("head"));

    ImVec2 foot;
    bool hasFoot = false;

    if (e.bones.count("limbs_l_calf") && e.bones.count("limbs_r_calf")) {
        ImVec2 l = ToImVec2(e.bones.at("limbs_l_calf"));
        ImVec2 r = ToImVec2(e.bones.at("limbs_r_calf"));
        foot = ImVec2((l.x + r.x) / 2, (l.y + r.y) / 2);
        hasFoot = true;
    }

    if (hasFoot) {
        float height = abs(foot.y - head.y) * 2.0f;
        float width = height / 2.0f;

        ImVec2 topLeft(head.x - width / 2, head.y - height * 0.2f);
        ImVec2 bottomRight(head.x + width / 2, head.y - height * 0.2f + height);

        ImU32 color = e.is_visible ? colors["cESP_Box"] : colors["cESP_BoxHidden"];
        float thickness = sync_float["fESP_LineThickness"];

        float lineLen = width * 0.2f;

        ImVec2 tl = topLeft;
        ImVec2 br = bottomRight;
        ImVec2 tr = ImVec2(br.x, tl.y);
        ImVec2 bl = ImVec2(tl.x, br.y);

        // Top-left corner
        draw->AddLine(tl, ImVec2(tl.x + lineLen, tl.y), color, thickness); // ─
        draw->AddLine(tl, ImVec2(tl.x, tl.y + lineLen), color, thickness); // │

        // Top-right corner
        draw->AddLine(tr, ImVec2(tr.x - lineLen, tr.y), color, thickness); // ─
        draw->AddLine(tr, ImVec2(tr.x, tr.y + lineLen), color, thickness); // │

        // Bottom-left corner
        draw->AddLine(bl, ImVec2(bl.x + lineLen, bl.y), color, thickness); // ─
        draw->AddLine(bl, ImVec2(bl.x, bl.y - lineLen), color, thickness); // │

        // Bottom-right corner
        draw->AddLine(br, ImVec2(br.x - lineLen, br.y), color, thickness); // ─
        draw->AddLine(br, ImVec2(br.x, br.y - lineLen), color, thickness); // │
    }
}

    update_aim_target();

if (sync_bool["bAIM"] && aim_target_valid && aim_target_visible) {
    draw->AddLine(
        ImVec2(sync_float["fWidth"] * 0.5f, sync_float["fHeight"] * 0.5f),
        aim_target_screen,
        IM_COL32(255, 0, 0, 255),
        2.0f
    );
}


if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
    draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
    draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
}




//========== LOCK-ON VISUAL INDICATOR ==========

        // Lock-On Indicator
// Lock-On Indicator
if (lockOnEnabled && lockedTarget && &e == lockedTarget) {
    ImVec2 targetHead = ToImVec2(e.bones.at("head"));
    
    // ✅ PERBAIKAN 1: Define screenCenter
    ImVec2 screenCenter(Width / 2, Height / 2);
    
    // Pulsing lock-on brackets
    static float lockPulse = 0.0f;
    lockPulse += 0.08f;
    float pulseSize = 20.0f + 5.0f * sin(lockPulse);
    
    // Draw lock-on brackets (4 corners)
    float bracketSize = pulseSize;
    float thickness = 3.0f;
    
    // Top-left
    draw->AddLine(ImVec2(targetHead.x - bracketSize, targetHead.y - bracketSize), 
                 ImVec2(targetHead.x - bracketSize/2, targetHead.y - bracketSize),
                 IM_COL32(0, 255, 0, 255), thickness);
    draw->AddLine(ImVec2(targetHead.x - bracketSize, targetHead.y - bracketSize), 
                 ImVec2(targetHead.x - bracketSize, targetHead.y - bracketSize/2),
                 IM_COL32(0, 255, 0, 255), thickness);
    
    // Top-right
    draw->AddLine(ImVec2(targetHead.x + bracketSize, targetHead.y - bracketSize), 
                 ImVec2(targetHead.x + bracketSize/2, targetHead.y - bracketSize),
                 IM_COL32(0, 255, 0, 255), thickness);
    draw->AddLine(ImVec2(targetHead.x + bracketSize, targetHead.y - bracketSize), 
                 ImVec2(targetHead.x + bracketSize, targetHead.y - bracketSize/2),
                 IM_COL32(0, 255, 0, 255), thickness);
    
    // Bottom-left
    draw->AddLine(ImVec2(targetHead.x - bracketSize, targetHead.y + bracketSize), 
                 ImVec2(targetHead.x - bracketSize/2, targetHead.y + bracketSize),
                 IM_COL32(0, 255, 0, 255), thickness);
    draw->AddLine(ImVec2(targetHead.x - bracketSize, targetHead.y + bracketSize), 
                 ImVec2(targetHead.x - bracketSize, targetHead.y + bracketSize/2),
                 IM_COL32(0, 255, 0, 255), thickness);
    
    // Bottom-right
    draw->AddLine(ImVec2(targetHead.x + bracketSize, targetHead.y + bracketSize), 
                 ImVec2(targetHead.x + bracketSize/2, targetHead.y + bracketSize),
                 IM_COL32(0, 255, 0, 255), thickness);
    draw->AddLine(ImVec2(targetHead.x + bracketSize, targetHead.y + bracketSize), 
                 ImVec2(targetHead.x + bracketSize, targetHead.y + bracketSize/2),
                 IM_COL32(0, 255, 0, 255), thickness);
    
    // Center dot (lebih besar untuk visibility)
    draw->AddCircleFilled(targetHead, 5.0f, IM_COL32(0, 255, 0, 255));
    draw->AddCircle(targetHead, 6.0f, IM_COL32(255, 255, 255, 255), 12, 2.0f);
    
    // ✅ PERBAIKAN 2: Lock line (screenCenter sudah didefinisikan)
    draw->AddLine(screenCenter, targetHead, IM_COL32(0, 255, 0, 200), 2.5f);
    
    // "LOCKED" text
    ImVec2 textPos = ImVec2(targetHead.x - 25, targetHead.y - 45);
    draw->AddText(textPos, IM_COL32(0, 255, 0, 255), "LOCKED");
    
    // ✅ PERBAIKAN 3: Movement tracking dengan time delta
    static ImVec2 prevHeadPos = targetHead;
    static auto lastFrameTime = std::chrono::steady_clock::now();
    
    auto currentTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
    
    // Calculate velocity (pixels per second)
    float moveSpeed = 0.0f;
    if (deltaTime > 0.001f) { // Avoid division by zero
        float dx = targetHead.x - prevHeadPos.x;
        float dy = targetHead.y - prevHeadPos.y;
        moveSpeed = sqrt(dx * dx + dy * dy) / deltaTime;
    }
    
    // Update tracking data
    prevHeadPos = targetHead;
    lastFrameTime = currentTime;
    
    // Show "TRACKING" if target is moving
    if (moveSpeed > 50.0f) { // Target is moving (adjusted threshold)
        ImVec2 trackText = ImVec2(targetHead.x - 30, targetHead.y - 30);
        draw->AddText(trackText, IM_COL32(255, 255, 0, 255), "TRACKING");
    }
    
    // Calculate distance
    ImVec2 lockHead = ToImVec2(e.bones.at("head"));
    ImVec2 lockFoot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    ImVec2 lockFoot_r = ToImVec2(e.bones.at("limbs_r_calf"));
    ImVec2 lockFeet = (lockFoot_l.y > lockFoot_r.y) ? lockFoot_l : lockFoot_r;
    
    float lockHeight = lockFeet.y - lockHead.y;
    float lockDistance = (lockHeight > 0) ? (1000.0f / lockHeight) : 300.0f;
    if (lockDistance > 300) lockDistance = 300;
    
    // Distance text
    char distText[32];
    sprintf(distText, "%.0fm", lockDistance);
    ImVec2 distTextPos = ImVec2(targetHead.x - 15, targetHead.y + 35);
    draw->AddText(distTextPos, IM_COL32(255, 255, 255, 255), distText);
    
    // Movement speed indicator
    if (moveSpeed > 50.0f) {
        char speedText[32];
        sprintf(speedText, "%.0f px/s", moveSpeed);
        ImVec2 speedTextPos = ImVec2(targetHead.x - 25, targetHead.y + 50);
        draw->AddText(speedTextPos, IM_COL32(255, 165, 0, 255), speedText);
    }
}




    // Player/Bot Counter
    if (isPlayerBotEnabled) {
    int totalPlayers = players;
    int totalBots = bots;
    
    float iconSize = 40.f;
    float totalWidth = iconSize * 2;
    float iconHeight = 35.f;
    float gap = 0.f;
    
    float centerX = sync_float["fWidth"] * 0.5f;
    float topY = sync_float["fHeight"] * 0.08f;
    
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    
    // Background gabungan
    ImVec2 bgMin(centerX - totalWidth * 0.5f, topY);
    ImVec2 bgMax(centerX + totalWidth * 0.5f, topY + iconHeight);
    
    // Player box (kiri - merah)
    ImVec2 playerBoxMin(bgMin.x, bgMin.y);
    ImVec2 playerBoxMax(centerX, bgMax.y);
    draw->AddRectFilled(playerBoxMin, playerBoxMax, IM_COL32(220, 40, 40, 255), 6.f, ImDrawFlags_RoundCornersLeft);
    
    // Bot box (kanan - hijau)
    ImVec2 botBoxMin(centerX, bgMin.y);
    ImVec2 botBoxMax(bgMax.x, bgMax.y);
    draw->AddRectFilled(botBoxMin, botBoxMax, IM_COL32(80, 200, 80, 255), 6.f, ImDrawFlags_RoundCornersRight);
    
    // Player icon
    ImVec2 playerCenter((playerBoxMin.x + playerBoxMax.x) * 0.5f - 10, (playerBoxMin.y + playerBoxMax.y) * 0.5f);
    draw->AddCircleFilled(ImVec2(playerCenter.x, playerCenter.y - 4), 5.f, IM_COL32(255, 255, 255, 255));
    draw->AddCircleFilled(ImVec2(playerCenter.x, playerCenter.y + 6), 7.f, IM_COL32(255, 255, 255, 255));
    
    // Player number
    char bufPlayer[8];
    sprintf(bufPlayer, "%d", totalPlayers);
    ImVec2 textPlayerSize = ImGui::CalcTextSize(bufPlayer);
    ImVec2 textPlayerPos(playerCenter.x + 12, playerCenter.y - textPlayerSize.y * 0.5f);
    draw->AddText(textPlayerPos, IM_COL32(255, 255, 255, 255), bufPlayer);
    
    // Bot icon
    ImVec2 botCenter((botBoxMin.x + botBoxMax.x) * 0.5f - 10, (botBoxMin.y + botBoxMax.y) * 0.5f);
    draw->AddRectFilled(ImVec2(botCenter.x - 6, botCenter.y - 6), 
                       ImVec2(botCenter.x + 6, botCenter.y + 6), 
                       IM_COL32(255, 255, 255, 255), 2.f);
    draw->AddCircleFilled(ImVec2(botCenter.x - 3, botCenter.y - 2), 1.5f, IM_COL32(80, 200, 80, 255));
    draw->AddCircleFilled(ImVec2(botCenter.x + 3, botCenter.y - 2), 1.5f, IM_COL32(80, 200, 80, 255));
    draw->AddRectFilled(ImVec2(botCenter.x - 4, botCenter.y + 2), 
                       ImVec2(botCenter.x + 4, botCenter.y + 3), 
                       IM_COL32(80, 200, 80, 255));
    
    // Bot number
    char bufBot[8];
    sprintf(bufBot, "%d", totalBots);
    ImVec2 textBotSize = ImGui::CalcTextSize(bufBot);
    ImVec2 textBotPos(botCenter.x + 12, botCenter.y - textBotSize.y * 0.5f);
    draw->AddText(textBotPos, IM_COL32(255, 255, 255, 255), bufBot);
}



if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
    if (sync_bool["bAIM_DrawFov"]) {
    draw->AddCircle(
        ImVec2(Width / 2, Height / 2),
        sync_float["fAIM_Fov"],
        IM_COL32(255, 0, 0, 255), // Merah
        0,
        sync_float["fESP_LineThickness"]
    );
    }
    }
    }
    
        
    // Magic Bullet FOV
    if (sync_bool["bMagic"] && magicBulletDrawFov) {
        draw->AddCircle(ImVec2(Width / 2, Height / 2), magicBulletRadius,
                       IM_COL32(255, 100, 0, 255), 64, 2.0f);
        
        float crossSize = 10.0f;
        ImVec2 center(Width / 2, Height / 2);
        draw->AddLine(ImVec2(center.x - crossSize, center.y), 
                     ImVec2(center.x + crossSize, center.y),
                     IM_COL32(255, 100, 0, 255), 2.0f);
        draw->AddLine(ImVec2(center.x, center.y - crossSize), 
                     ImVec2(center.x, center.y + crossSize),
                     IM_COL32(255, 100, 0, 255), 2.0f);
    }

        }
        
        
// ============================================================
// KEY SYSTEM LOGIC
// ============================================================
static std::atomic<uint32_t> auth_state(0x8A8A8A8A);
const uint32_t AUTH_SUCCESS = 0x1337BABE;
static std::atomic<bool> is_authenticating(false);
static char user_input_key[64] = "";
static char auth_status[256] = "Waiting for key...";
static std::string cached_uuid = "";

std::string getDeviceName() {
    char model[PROP_VALUE_MAX];
    __system_property_get(xorstr_("ro.product.model"), model);
    return std::string(model);
}

std::string GetMD5Hash(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.size(), digest);
    std::ostringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    return ss.str();
}

std::string GetCleanUUID_RBS() {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    jclass atClass = env->FindClass(xorstr_("android/app/ActivityThread"));
    jmethodID currentAT = env->GetStaticMethodID(atClass, xorstr_("currentActivityThread"), xorstr_("()Landroid/app/ActivityThread;"));
    jobject at = env->CallStaticObjectMethod(atClass, currentAT);
    jmethodID getApp = env->GetMethodID(atClass, xorstr_("getApplication"), xorstr_("()Landroid/app/Application;"));
    jobject app = env->CallObjectMethod(at, getApp);
    std::string raw_id = GetAndroidID(env, app);
    raw_id += GetDeviceModel(env);
    raw_id += GetDeviceBrand(env);
    std::string final_uuid = GetDeviceUniqueIdentifier(env, raw_id.c_str());
    jvm->DetachCurrentThread();
    return final_uuid;
}

std::string GenerateSecuritySignature_RBS(const std::string& hwid) {
    std::string secret = xorstr_("SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR");
    std::string str_to_hash = "hwid=" + hwid + secret;
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str_to_hash.c_str(), str_to_hash.size());
    MD5_Final(hash, &md5);
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::string GetKeyGeneratorLink_RBS(const std::string& uuid) {
    std::string sig = GenerateSecuritySignature_RBS(uuid);
    return std::string(xorstr_("https://bloodstrikes.replit.app/?hwid=")) + uuid + xorstr_("&signature=") + sig;
}

bool VerifyKeyWithServer_RBS(const std::string& key, const std::string& uuid) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        snprintf(auth_status, sizeof(auth_status), xorstr_("Error: CURL failed to initialize"));
        return false;
    }
    char errbuf[CURL_ERROR_SIZE]; errbuf[0] = 0;
    char* ek = curl_easy_escape(curl, key.c_str(), key.length());
    char* eu = curl_easy_escape(curl, uuid.c_str(), uuid.length());
    if (!ek || !eu) {
        if (ek) curl_free(ek);
        if (eu) curl_free(eu);
        curl_easy_cleanup(curl);
        snprintf(auth_status, sizeof(auth_status), xorstr_("Error: URL Encoding failed"));
        return false;
    }
    std::string url = xorstr_("https://bloodstrikes.replit.app/verify?key=");
    url += ek; url += xorstr_("&hwid="); url += eu;
    curl_free(ek); curl_free(eu);
    struct MemoryStruct chunk{}; chunk.memory = (char*)malloc(1); chunk.size = 0;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, xorstr_("libcurl-agent/1.0"));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    CURLcode res = curl_easy_perform(curl);
    bool isSuccess = false;
    if (res == CURLE_OK) {
        try {
            using json = nlohmann::json;
            auto data = json::parse(chunk.memory);
            if (data.contains(xorstr_("sig")) && data.contains(xorstr_("ts"))) {
                bool sv = data[xorstr_("success")].get<bool>();
                std::string sm = data.value(xorstr_("message"), "");
                std::string sts = data[xorstr_("ts")].get<std::string>();
                std::string ssig = data[xorstr_("sig")].get<std::string>();
                std::string SALT = xorstr_("SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR");
                std::string raw = SALT + (sv ? xorstr_("true") : xorstr_("false")) + sts + uuid;
                std::string csig = GetMD5Hash(raw);
                if (csig == ssig) {
                    long long ts_val = std::stoll(sts);
                    long long now = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::system_clock::now().time_since_epoch()).count();
                    if (std::abs(now - ts_val) < 300000) {
                        if (sv) {
                            if (data.contains(xorstr_("days_remaining")) && data[xorstr_("days_remaining")].is_number()) {
                                bss_vip_days_remaining = data[xorstr_("days_remaining")].get<int>();
                                bss_vip_days_received  = true;
                            }
                            if (data.contains(xorstr_("username")) && data[xorstr_("username")].is_string()) {
                                std::string uname = data[xorstr_("username")].get<std::string>();
                                strncpy(bss_vip_username_str, uname.c_str(), sizeof(bss_vip_username_str) - 1);
                            }
                            snprintf(auth_status, sizeof(auth_status), "%s", sm.empty() ? xorstr_("Login Successful!") : sm.c_str());
                            auth_state.store(AUTH_SUCCESS);
                            isSuccess = true;
                        } else {
                            snprintf(auth_status, sizeof(auth_status), "%s", sm.empty() ? xorstr_("Login Failed") : sm.c_str());
                        }
                    } else { snprintf(auth_status, sizeof(auth_status), xorstr_("Security: Response Expired")); }
                } else { snprintf(auth_status, sizeof(auth_status), xorstr_("Security: Hash Mismatch Detected")); }
            } else { snprintf(auth_status, sizeof(auth_status), xorstr_("Server Error: Invalid Protocol")); }
        } catch (...) { snprintf(auth_status, sizeof(auth_status), xorstr_("Bad Server Response!")); }
    } else {
        size_t len = strlen(errbuf);
        if (len > 0) snprintf(auth_status, sizeof(auth_status), xorstr_("CURL %d: %s"), res, errbuf);
        else snprintf(auth_status, sizeof(auth_status), xorstr_("CURL Error Code: %d"), res);
    }
    free(chunk.memory); curl_easy_cleanup(curl);
    return isSuccess;
}

bool CheckVersion_RBS(const std::string& current_version) {
    CURL *curl = curl_easy_init();
    if (!curl) return true;
    struct MemoryStruct chunk{}; chunk.memory = (char*)malloc(1); chunk.size = 0;
    std::string url = xorstr_("https://bloodstrikes.replit.app/version?v=") + current_version;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    CURLcode res = curl_easy_perform(curl);
    bool isValid = true;
    if (res == CURLE_OK) {
        try {
            using json = nlohmann::json;
            auto result = json::parse(chunk.memory);
            if (result.contains(xorstr_("sig")) && result.contains(xorstr_("ts"))) {
                bool sv = result[xorstr_("valid")].get<bool>();
                std::string sm = result.value(xorstr_("message"), "");
                std::string sts = result[xorstr_("ts")].get<std::string>();
                std::string ssig = result[xorstr_("sig")].get<std::string>();
                std::string SALT = xorstr_("SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR");
                std::string raw = SALT + (sv ? xorstr_("true") : xorstr_("false")) + sts + current_version;
                std::string csig = GetMD5Hash(raw);
                if (csig == ssig) {
                    isValid = sv;
                    if (!isValid && !sm.empty()) snprintf(auth_status, sizeof(auth_status), "%s", sm.c_str());
                } else {
                    isValid = false;
                    snprintf(auth_status, sizeof(auth_status), xorstr_("Security: Version check tampered!"));
                }
            }
        } catch (...) { isValid = false; }
    }
    free(chunk.memory); curl_easy_cleanup(curl);
    return isValid;
}

// ============================================================

        void DrawWaveModsMenu() {
    static bool showMenu = false;
    bool need_sync = false;
    sync_float[xorstr_("fHeight")] = Height;
    sync_float[xorstr_("fWidth")] = Width;
    sync_with_py();

    ImGuiIO& io = ImGui::GetIO();

    // ── Dark Mode Palette ──────────────────────────────────────────────────
    ImVec4 themeColor = ImVec4(0.72f, 0.32f, 0.95f, 1.0f);
    ImVec4 themeDim   = ImVec4(0.72f, 0.32f, 0.95f, 0.28f);
    ImVec4 bgDark     = ImVec4(0.05f, 0.05f, 0.07f, 0.97f);
    ImVec4 bgPanel    = ImVec4(0.08f, 0.08f, 0.11f, 1.0f);
    ImVec4 bgFrame    = ImVec4(0.12f, 0.12f, 0.16f, 1.0f);
    ImVec4 textMain   = ImVec4(0.90f, 0.90f, 0.94f, 1.0f);
    ImVec4 textDim    = ImVec4(0.42f, 0.42f, 0.52f, 1.0f);
    ImVec4 borderCol  = ImVec4(0.20f, 0.20f, 0.26f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg,             bgDark);
    ImGui::PushStyleColor(ImGuiCol_ChildBg,              bgPanel);
    ImGui::PushStyleColor(ImGuiCol_FrameBg,              bgFrame);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,       ImVec4(0.17f, 0.17f, 0.23f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,        ImVec4(0.21f, 0.21f, 0.29f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button,               bgFrame);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,        ImVec4(0.18f, 0.18f, 0.26f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,         themeDim);
    ImGui::PushStyleColor(ImGuiCol_Header,               themeDim);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,        ImVec4(0.72f, 0.32f, 0.95f, 0.42f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,         ImVec4(0.72f, 0.32f, 0.95f, 0.58f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab,           themeColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive,     ImVec4(0.85f, 0.48f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark,            themeColor);
    ImGui::PushStyleColor(ImGuiCol_Separator,            borderCol);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg,          bgDark);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab,        ImVec4(0.22f, 0.22f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.32f, 0.32f, 0.42f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_PopupBg,              bgPanel);
    ImGui::PushStyleColor(ImGuiCol_Border,               borderCol);
    ImGui::PushStyleColor(ImGuiCol_Text,                 textMain);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled,         textDim);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,   8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,    6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,    5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding,     5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding,    6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,      ImVec2(6.0f, 5.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,     ImVec2(6.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

    // ── Shield Logo Helper (same as Sacred) ──────────────────────────────
    auto DrawShieldLogo = [](ImDrawList* ld, ImVec2 center, float sz, float t) {
        float hw = sz * 0.48f, hh = sz * 0.54f;
        ImVec2 shield[5] = {
            ImVec2(center.x - hw, center.y - hh),
            ImVec2(center.x + hw, center.y - hh),
            ImVec2(center.x + hw, center.y + hh * 0.25f),
            ImVec2(center.x,      center.y + hh),
            ImVec2(center.x - hw, center.y + hh * 0.25f),
        };
        ld->AddConvexPolyFilled(shield, 5, IM_COL32(140, 8, 8, 255));
        float shx = hw * 0.85f, shy = hh * 0.85f;
        ImVec2 inner[5] = {
            ImVec2(center.x - shx, center.y - shy),
            ImVec2(center.x + shx, center.y - shy),
            ImVec2(center.x + shx, center.y + shy * 0.25f),
            ImVec2(center.x,       center.y + shy),
            ImVec2(center.x - shx, center.y + shy * 0.25f),
        };
        ld->AddConvexPolyFilled(inner, 5, IM_COL32(180, 12, 12, 180));
        int ga = (int)(180 + 75 * fabsf(sinf(t * 2.2f)));
        ld->AddPolyline(shield, 5, IM_COL32(255, 200, 0, ga), ImDrawFlags_Closed, 2.2f);
        ld->AddRectFilled(
            ImVec2(center.x - hw * 0.7f, center.y - hh * 0.88f),
            ImVec2(center.x + hw * 0.7f, center.y - hh * 0.60f),
            IM_COL32(255, 255, 255, 18), 2.0f);
        float bw = sz * 0.32f, bh = sz * 0.07f, bc = sz * 0.08f;
        float sx = center.x - bw * 0.5f, sy = center.y - (bh*3 + bc*2) * 0.5f - sz * 0.04f;
        ImU32 gc = IM_COL32(255, 215, 0, 255);
        ld->AddRectFilled(ImVec2(sx, sy),               ImVec2(sx+bw, sy+bh),           gc, 2.0f);
        ld->AddRectFilled(ImVec2(sx, sy+bh),            ImVec2(sx+bh, sy+bh+bc),        gc);
        ld->AddRectFilled(ImVec2(sx, sy+bh+bc),         ImVec2(sx+bw, sy+bh*2+bc),      gc, 2.0f);
        ld->AddRectFilled(ImVec2(sx+bw-bh, sy+bh*2+bc),ImVec2(sx+bw, sy+bh*2+bc+bc),   gc);
        ld->AddRectFilled(ImVec2(sx, sy+bh*2+bc*2),    ImVec2(sx+bw, sy+bh*3+bc*2),     gc, 2.0f);
        ld->AddCircleFilled(ImVec2(center.x - hw + 5, center.y - hh + 5), 3.0f, IM_COL32(255,200,0,160));
        ld->AddCircleFilled(ImVec2(center.x + hw - 5, center.y - hh + 5), 3.0f, IM_COL32(255,200,0,160));
    };

    // ── Floating Shield Button (same design as Sacred) ────────────────────
    if (!showMenu) {
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.90f, io.DisplaySize.y * 0.15f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(80, 88));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Border,   ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        if (ImGui::Begin("##OpenMenuBtn", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground)) {

            float t = ImGui::GetTime();
            ImDrawList* bdl = ImGui::GetWindowDrawList();
            ImVec2 bp = ImGui::GetWindowPos();
            float cx = bp.x + 40, cy = bp.y + 44;

            int ga = (int)(140 + 80 * fabsf(sinf(t * 2.0f)));
            bdl->AddCircle(ImVec2(cx, cy), 42.0f, IM_COL32(200, 20, 20, ga), 32, 3.0f);
            bdl->AddCircleFilled(ImVec2(cx, cy), 38.0f, IM_COL32(12, 4, 4, 245));

            DrawShieldLogo(bdl, ImVec2(cx, cy - 2), 52.0f, t);

            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::InvisibleButton("##btn", ImVec2(80, 88));
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                ImVec2 delta = ImGui::GetIO().MouseDelta;
                ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x + delta.x, ImGui::GetWindowPos().y + delta.y));
            }
            if (ImGui::IsItemDeactivated() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left, 4.0f)) {
                showMenu = true;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);

        ImGui::PopStyleVar(8);
        ImGui::PopStyleColor(22);
        return;
    }

    // ── Key System ──────────────────────────────────────────────────────────
    if (auth_state.load() != AUTH_SUCCESS) {
        if (cached_uuid.empty()) cached_uuid = GetCleanUUID_RBS();
        std::string link = GetKeyGeneratorLink_RBS(cached_uuid);

        float scrW = io.DisplaySize.x;
        float scrH = io.DisplaySize.y;

        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 420), ImVec2(scrW * 0.95f, scrH * 0.90f));
        ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(scrW * 0.5f, scrH * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,    ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,   8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.06f, 0.04f, 0.04f, 0.98f));
        ImGui::PushStyleColor(ImGuiCol_Border,        ImVec4(0.70f, 0.06f, 0.06f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,       ImVec4(0.12f, 0.07f, 0.07f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.08f, 0.08f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.05f, 0.05f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark,     ImVec4(0.95f, 0.20f, 0.20f, 1.0f));

        if (ImGui::Begin("##RBSKeySystem", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

            float ww = ImGui::GetWindowSize().x;
            ImVec4 loginAccent = ImVec4(0.95f, 0.20f, 0.20f, 1.0f);

            // Title
            ImGui::SetWindowFontScale(1.1f);
            const char* title = xorstr_("SACREDBS VIP - LOGIN");
            ImGui::SetCursorPosX((ww - ImGui::CalcTextSize(title).x) * 0.5f);
            ImGui::TextColored(loginAccent, "%s", title);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Separator(); ImGui::Spacing();

            // Device info
            ImGui::TextDisabled(xorstr_("DEVICE:"));
            ImGui::SameLine();
            ImGui::Text("%s", getDeviceName().c_str());
            ImGui::TextDisabled(xorstr_("HWID:"));
            ImGui::SameLine();
            ImGui::Text("%s", cached_uuid.c_str());
            ImGui::Spacing();
            ImGui::TextDisabled(xorstr_("Telegram: t.me/SACREDBS_VIP"));
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            ImGui::TextColored(loginAccent, xorstr_("STEP 1: GET YOUR FREE KEY"));
            float halfW = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.70f, 0.06f, 0.06f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
            if (ImGui::Button(xorstr_("OPEN IN BROWSER"), ImVec2(halfW, 45))) {
                OpenBrowserWithUrl(link);
                snprintf(auth_status, sizeof(auth_status), xorstr_("Opening browser..."));
            }
            ImGui::SameLine();
            if (ImGui::Button(xorstr_("COPY LINK"), ImVec2(halfW, 45))) {
                CopyLinkToClipboard(link);
                snprintf(auth_status, sizeof(auth_status), xorstr_("Link copied! Paste in browser."));
            }
            ImGui::PopStyleColor(2);
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            ImGui::TextColored(loginAccent, xorstr_("STEP 2: LOGIN"));
            ImGui::Spacing();
            ImGui::PushItemWidth(-1);
            if (ImGui::InputTextWithHint("##rbs_key", xorstr_("click PASTE..."),
                user_input_key, IM_ARRAYSIZE(user_input_key)))
                ShowSoftKeyboardInput();
            ImGui::PopItemWidth();
            ImGui::Spacing();
            if (ImGui::Button(xorstr_("PASTE KEY FROM CLIPBOARD"), ImVec2(-1, 40))) {
                std::string clip = getClipboard();
                if (!clip.empty()) {
                    strncpy(user_input_key, clip.c_str(), IM_ARRAYSIZE(user_input_key) - 1);
                    snprintf(auth_status, sizeof(auth_status), xorstr_("Key pasted!"));
                } else {
                    snprintf(auth_status, sizeof(auth_status), xorstr_("Clipboard is empty!"));
                }
            }
            ImGui::Spacing();

            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.70f, 0.06f, 0.06f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1.0f,  1.0f,  1.0f,  1.0f));
            if (is_authenticating.load()) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                ImGui::Button(xorstr_("VERIFYING..."), ImVec2(-1, 48));
                ImGui::PopStyleVar();
            } else {
                if (ImGui::Button(xorstr_("LOGIN"), ImVec2(-1, 48))) {
                    is_authenticating = true;
                    snprintf(auth_status, sizeof(auth_status), xorstr_("Checking App Version..."));
                    std::string temp_key(user_input_key);
                    std::string temp_hwid(cached_uuid);
                    std::thread([temp_key, temp_hwid]() {
                        JNIEnv *env;
                        bool did_attach = false;
                        int stat = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
                        if (stat == JNI_EDETACHED) {
                            if (jvm->AttachCurrentThread(&env, NULL) == JNI_OK) did_attach = true;
                        }
                        if (CheckVersion_RBS(xorstr_("1.3"))) {
                            snprintf(auth_status, sizeof(auth_status), xorstr_("Connecting to server..."));
                            VerifyKeyWithServer_RBS(temp_key, temp_hwid);
                        }
                        is_authenticating = false;
                        if (did_attach) jvm->DetachCurrentThread();
                    }).detach();
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::Spacing();
            float sw = ImGui::CalcTextSize(auth_status).x;
            ImGui::SetCursorPosX((ww - sw) * 0.5f);
            ImGui::TextColored(ImVec4(1.0f, 0.28f, 0.28f, 1.0f), "%s", auth_status);
        }
        ImGui::End();
        ImGui::PopStyleColor(6);
        ImGui::PopStyleVar(3);
        ImGui::PopStyleVar(8); ImGui::PopStyleColor(22);
        return;
    }

    // ── Main Menu ──────────────────────────────────────────────────────────
    {
    
        float scrW = io.DisplaySize.x;
        float scrH = io.DisplaySize.y;
        float defW = fminf(scrW * 0.96f, 660.0f);
        float defH = fminf(scrH * 0.95f, 440.0f);

        ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 200.0f), ImVec2(scrW * 0.98f, scrH * 0.98f));
        ImGui::SetNextWindowSize(ImVec2(defW, defH), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

        if (ImGui::Begin("##RyukoBS_MainMenu", &showMenu,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse)) {

            float ww    = ImGui::GetWindowSize().x;
            float scale = fmaxf(ww / 600.0f, 0.48f);
            float t_now = ImGui::GetTime();
            ImVec2 winPos = ImGui::GetWindowPos();
            ImDrawList* dl = ImGui::GetWindowDrawList();

            // ── HEADER BACKGROUND ────────────────────────────────────────
            float HDR_H  = 56.0f;
            float logoSz = 44.0f;
            ImVec2 hdrMin = ImVec2(winPos.x + 1, winPos.y + 1);
            ImVec2 hdrMax = ImVec2(winPos.x + ww - 1, winPos.y + HDR_H);
            dl->AddRectFilled(hdrMin, hdrMax, IM_COL32(12, 4, 4, 255), 8.0f, ImDrawFlags_RoundCornersTop);
            dl->AddLine(ImVec2(hdrMin.x, hdrMax.y), ImVec2(hdrMax.x, hdrMax.y), IM_COL32(180, 120, 0, 180), 1.5f);
            dl->AddRect(hdrMin, hdrMax, IM_COL32(150, 12, 12, 160), 8.0f, ImDrawFlags_RoundCornersTop, 1.2f);

            // ── SHIELD LOGO left ─────────────────────────────────────────
            DrawShieldLogo(dl, ImVec2(winPos.x + 8 + logoSz * 0.5f, winPos.y + HDR_H * 0.5f), logoSz, t_now);

            // ── DRAG HANDLE ──────────────────────────────────────────────
            ImGui::SetWindowFontScale(0.90f);
            {
                float dragX  = 8 + logoSz + 6;
                float closeW = 68.0f;
                float dragW  = ww - dragX - closeW - ImGui::GetStyle().ItemSpacing.x;
                ImGui::SetCursorPos(ImVec2(dragX, 0));
                ImGui::InvisibleButton("##hdr_drag", ImVec2(dragW, HDR_H));
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                    ImVec2 delta = ImGui::GetIO().MouseDelta;
                    ImGui::SetWindowPos(ImVec2(winPos.x + delta.x, winPos.y + delta.y));
                }
                ImGui::SetWindowFontScale(0.90f);
                float fh = ImGui::GetFontSize();
                int pr = (int)(220 + 35 * sinf(t_now * 2.5f));
                int pg = (int)(150 * fabsf(sinf(t_now * 1.3f)));
                dl->AddText(ImVec2(winPos.x + dragX, winPos.y + (HDR_H - fh) * 0.5f),
                            IM_COL32(pr, pg, 10, 255), xorstr_("SACREDBS VIP  :::  drag to move"));
            }

            // ── CLOSE BUTTON ─────────────────────────────────────────────
            ImGui::SetWindowFontScale(0.70f);
            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.60f, 0.04f, 0.04f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.35f, 0.02f, 0.02f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1.0f,  1.0f,  1.0f,  1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);
            float btnH = HDR_H - 16.0f;
            ImGui::SetCursorPos(ImVec2(ww - 68, (HDR_H - btnH) * 0.5f));
            if (ImGui::Button(xorstr_(" X "), ImVec2(56, btnH))) showMenu = false;
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(4);

            ImGui::SetWindowFontScale(1.0f);
            ImGui::SetCursorPosY(HDR_H + 4);
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            // ── Tab navigation ───────────────────────────────────────────
            static int ActiveTab    = 1;
            static int ActiveSubTab = 1;

            ImGui::SetWindowFontScale(scale * 0.70f);
            ImGuiStyle& st = ImGui::GetStyle();
            float totalW  = ImGui::GetContentRegionAvail().x;
            float btnW    = (totalW - st.ItemSpacing.x * 5.0f) / 6.0f;

            auto NavBtn = [&](const char* label, int id) {
                bool active = (ActiveTab == id);
                ImGui::PushStyleColor(ImGuiCol_Button,
                    active ? ImVec4(0.72f, 0.32f, 0.95f, 0.38f) : bgFrame);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                    active ? ImVec4(0.72f, 0.32f, 0.95f, 0.52f) : ImVec4(0.18f, 0.18f, 0.26f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text,
                    active ? themeColor : textMain);
                if (ImGui::Button(label, ImVec2(btnW, fmaxf(24.0f * scale, 20.0f))))
                    { ActiveTab = id; ActiveSubTab = 1; }
                ImGui::PopStyleColor(3);
            };

            NavBtn(xorstr_("ESP"),    1); ImGui::SameLine();
            NavBtn(xorstr_("COMBAT"), 2); ImGui::SameLine();
            NavBtn(xorstr_("MOVE"),   3); ImGui::SameLine();
            NavBtn(xorstr_("OTHER"),  4); ImGui::SameLine();
            NavBtn(xorstr_("INFO"),   5); ImGui::SameLine();
            NavBtn(xorstr_("SHIELD"), 6);

            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
            ImGui::SetWindowFontScale(scale * 0.72f);

            // ── Scrollable content area ──────────────────────────────────
            ImGui::BeginChild(xorstr_("##RBS_Content"), ImVec2(0, 0), false,
                ImGuiWindowFlags_AlwaysVerticalScrollbar);

            // Sub-tab helper (lambda)
            auto SubBtn = [&](const char* label, int id) {
                bool active = (ActiveSubTab == id);
                ImGui::PushStyleColor(ImGuiCol_Button,
                    active ? ImVec4(0.72f, 0.32f, 0.95f, 0.28f) : bgFrame);
                ImGui::PushStyleColor(ImGuiCol_Text,
                    active ? themeColor : textMain);
                if (ImGui::Button(label, ImVec2(0, 0))) ActiveSubTab = id;
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
            };

            // ── TAB 1: ESP ───────────────────────────────────────────────
            if (ActiveTab == 1) {
                SubBtn(xorstr_("Main"), 1);
                SubBtn(xorstr_("Extra"), 2);
                SubBtn(xorstr_("Colors"), 3);
                ImGui::NewLine();
                ImGui::Separator(); ImGui::Spacing();

                if (ActiveSubTab == 1) {
                    ImGui::TextColored(themeColor, xorstr_("ESP SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("ENABLE ESP"),    &sync_bool[xorstr_("bESP")]);
                    Toggle(xorstr_("ESP LINE"),      &sync_bool[xorstr_("bESP_Line")]);
                    Toggle(xorstr_("ESP BOT LINE"),  &sync_bool[xorstr_("bESP_LineBots")]);
                    Toggle(xorstr_("ESP BOX"),       &sync_bool[xorstr_("bESP_Box")]);
                    Toggle(xorstr_("ESP SKELETON"),  &sync_bool[xorstr_("bESP_Skeleton")]);
                    ImGui::Spacing();
                    ImGui::TextDisabled(xorstr_("LINE POSITION"));
                    need_sync |= Combo(xorstr_("##LinePos"), &sync_int[xorstr_("iESP_Point")], espPoint, IM_ARRAYSIZE(espPoint));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                    need_sync |= SliderFloat(xorstr_("LINE SIZE"), &sync_float[xorstr_("fESP_LineThickness")], 1.0f, 10.0f, xorstr_("%.2f"));
                    ImGui::PopItemWidth();
                }
                else if (ActiveSubTab == 2) {
                    ImGui::TextColored(themeColor, xorstr_("ESP EXTRA"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("ESP NAME"),     &sync_bool[xorstr_("bESP_Name")]);
                    Toggle(xorstr_("ESP HEALTH"),   &sync_bool[xorstr_("bESP_Health")]);
                    Toggle(xorstr_("ESP DISTANCE"), &sync_bool[xorstr_("bESP_Distance")]);
                    Toggle(xorstr_("ESP TEAM ID"),  &sync_bool[xorstr_("bESP_TeamID")]);
                    Toggle(xorstr_("ESP COUNT"),    &isPlayerBotEnabled);
                }
                else if (ActiveSubTab == 3) {
                    ImGui::TextColored(themeColor, xorstr_("COLOR SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    ImGui::ColorEdit3(xorstr_("ESP LINE COLOR"), (float*)&colors["cESP_Line"], ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit3(xorstr_("ESP BOX COLOR"),  (float*)&colors["cESP_Box"],  ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit3(xorstr_("SKELETON COLOR"), (float*)&colors["cESP_Skeleton"], ImGuiColorEditFlags_NoInputs);
                }
            }

            // ── TAB 2: COMBAT ────────────────────────────────────────────
            else if (ActiveTab == 2) {
                SubBtn(xorstr_("Aimbot"), 1);
                SubBtn(xorstr_("FOV"),    2);
                SubBtn(xorstr_("Memory"), 3);
                ImGui::NewLine();
                ImGui::Separator(); ImGui::Spacing();

                if (ActiveSubTab == 1) {
                    ImGui::TextColored(themeColor, xorstr_("AIMBOT SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("ENABLE AIMBOT"),       &sync_bool[xorstr_("bAIM")]);
                    Toggle(xorstr_("CHECK VISIBILITY"),    &sync_bool[xorstr_("bAIM_CheckVisibility")]);
                    Toggle(xorstr_("IGNORE KNOCKED"),      &sync_bool[xorstr_("bAIM_IgnoreKnocked")]);
                    Toggle(xorstr_("IGNORE BOTS"),         &sync_bool[xorstr_("bAIM_IgnoreBots")]);
                    ImGui::Spacing();
                    ImGui::TextColored(themeColor, xorstr_("BULLET TRACK"));
                    ImGui::Separator();
                    Toggle(xorstr_("ENABLE BULLET"),       &sync_bool[xorstr_("bBulletTrack")]);
                    Toggle(xorstr_("IGNORE KNOCK"),        &sync_bool[xorstr_("bBulletTrack_IgnoreKnocked")]);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                    need_sync |= SliderFloat(xorstr_("PROBABILITY"), &sync_float[xorstr_("fBulletTrack_Probability")], 30.0f, 100.0f, xorstr_("%.2f"));
                    ImGui::PopItemWidth();
                    ImGui::Spacing();
                    ImGui::TextColored(themeColor, xorstr_("LOCK-ON AIMBOT"));
                    ImGui::Separator();
                    Toggle(xorstr_("ENABLE LOCK-ON"),      &lockOnEnabled);
                    ImGui::SliderFloat(xorstr_("SMOOTHNESS"), &lockSmoothness, 0.05f, 100.0f, xorstr_("%.2f"));
                    Toggle(xorstr_("PREDICTION"),          &lockPrediction);
                    if (lockPrediction)
                        ImGui::SliderFloat(xorstr_("PREDICT STR"), &predictionStrength, 0.5f, 100.0f, xorstr_("%.1f"));
                }
                else if (ActiveSubTab == 2) {
                    ImGui::TextColored(themeColor, xorstr_("FOV SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("SHOW FOV"),            &sync_bool[xorstr_("bAIM_DrawFov")]);
                    Toggle(xorstr_("HIDE FOV NO TARGET"),  &sync_bool[xorstr_("bAIM_NoTargetHideFov")]);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                    need_sync |= SliderFloat(xorstr_("FOV SIZE"), &sync_float[xorstr_("fAIM_Fov")], 30.0f, 1500.0f, xorstr_("%.2f"));
                    ImGui::PopItemWidth();
                    ImGui::Spacing();
                    ImGui::TextColored(themeColor, xorstr_("AIM SETTINGS"));
                    ImGui::Separator();
                    need_sync |= Combo(xorstr_("TRIGGER"),     &sync_int[xorstr_("iAIM_Trigger")],    aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
                    need_sync |= Combo(xorstr_("TARGET BONE"), &sync_int[xorstr_("iAIM_TargetBone")], targetBone,    IM_ARRAYSIZE(targetBone));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                    need_sync |= SliderFloat(xorstr_("STRENGTH"), &sync_float[xorstr_("fAIM_SnapStrength")], 0.001f, 3.0f, xorstr_("%.3f"));
                    ImGui::PopItemWidth();
                }
                else if (ActiveSubTab == 3) {
                    ImGui::TextColored(themeColor, xorstr_("RECOIL & SPREAD"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("NO RECOIL"), &sync_bool[xorstr_("bNoRecoil")]);
                    Toggle(xorstr_("NO SPREAD"), &sync_bool[xorstr_("bNoSpread")]);
                    Toggle(xorstr_("NO SHAKE"),  &sync_bool[xorstr_("bSwitch")]);
                    ImGui::Spacing();
                    ImGui::TextColored(themeColor, xorstr_("MAGIC BULLET"));
                    ImGui::Separator();
                    Toggle(xorstr_("ENABLE MAGIC"),   &sync_bool[xorstr_("bMagic")]);
                    Toggle(xorstr_("SHOW FOV MAGIC"), &magicBulletDrawFov);
                    ImGui::SliderFloat(xorstr_("MAGIC RADIUS"), &magicBulletRadius, 10.0f, 500.0f, xorstr_("%.1f"));
                    ImGui::Spacing();
                    ImGui::TextColored(themeColor, xorstr_("OTHER"));
                    ImGui::Separator();
                    Toggle(xorstr_("NO RELOAD"),    &sync_bool[xorstr_("reloadTime")]);
                    Toggle(xorstr_("BULLET TRACE"), &sync_bool[xorstr_("bulletTrace")]);
                }
            }

            // ── TAB 3: MOVEMENT ──────────────────────────────────────────
            else if (ActiveTab == 3) {
                ImGui::TextColored(themeColor, xorstr_("MOVEMENT SETTINGS"));
                ImGui::Separator(); ImGui::Spacing();
                Toggle(xorstr_("ENABLE SPEED"), &sync_bool[xorstr_("bSpeed")]);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                need_sync |= SliderFloat(xorstr_("SPEED VALUE"), &sync_float[xorstr_("fSpeed")], 1.0f, 15.0f, xorstr_("%.2f"));
                ImGui::PopItemWidth();
                ImGui::Spacing(); ImGui::Separator();
                Toggle(xorstr_("ENABLE JUMP"), &sync_bool[xorstr_("bjump")]);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.72f);
                need_sync |= SliderFloat(xorstr_("JUMP VALUE"), &sync_float[xorstr_("fjump")], 1.0f, 15.0f, xorstr_("%.2f"));
                ImGui::PopItemWidth();
            }

            // ── TAB 4: OTHER (Xray / Chams) ──────────────────────────────
            else if (ActiveTab == 4) {
                SubBtn(xorstr_("Xray"),  1);
                SubBtn(xorstr_("Chams"), 2);
                ImGui::NewLine();
                ImGui::Separator(); ImGui::Spacing();

                if (ActiveSubTab == 1) {
                    ImGui::TextColored(themeColor, xorstr_("XRAY SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    Toggle(xorstr_("RED WALLHACK"), &sync_bool[xorstr_("bXray")]);
                    Toggle(xorstr_("SKIN HACK"),    &sync_bool[xorstr_("bSkinHack")]);
                }
                else if (ActiveSubTab == 2) {
                    ImGui::TextColored(themeColor, xorstr_("CHAMS SETTINGS"));
                    ImGui::Separator(); ImGui::Spacing();
                    need_sync |= Combo(xorstr_("CHAMS TYPE"), &chamsint, Chams, IM_ARRAYSIZE(Chams));
                    Toggle(xorstr_("RGB VISIBLE"), &enableRainbow);
                    Toggle(xorstr_("RGB WALL"),    &enableRainbowWall);
                    ImGui::Spacing();
                    ImGui::TextDisabled(xorstr_("IN WALL COLOR"));
                    need_sync |= ImGui::ColorEdit4(xorstr_("##InWall"),  (float*)&inWallColor,  ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    ImGui::SameLine(); ImGui::TextDisabled(xorstr_("VISIBLE COLOR"));
                    need_sync |= ImGui::ColorEdit4(xorstr_("##Visible"), (float*)&visibleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                }
            }

            // ── TAB 5: INFO ──────────────────────────────────────────────
            else if (ActiveTab == 5) {
                // ── VIP STATUS ───────────────────────────────────────────
                ImGui::TextColored(ImVec4(1.0f, 0.80f, 0.10f, 1.0f), xorstr_("VIP STATUS"));
                ImGui::Separator(); ImGui::Spacing();
                if (strlen(bss_vip_username_str) > 0) {
                    ImGui::TextDisabled(xorstr_("USER:"));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.1f, 1.0f), "%s", bss_vip_username_str);
                }
                ImGui::TextDisabled(xorstr_("DAYS REMAINING:"));
                ImGui::SameLine();
                if (bss_vip_days_received) {
                    float dr = (float)bss_vip_days_remaining;
                    ImVec4 daysColor = dr > 7 ? ImVec4(0.3f,1.0f,0.3f,1.0f)
                                     : dr > 2 ? ImVec4(1.0f,0.7f,0.1f,1.0f)
                                     :          ImVec4(1.0f,0.2f,0.2f,1.0f);
                    ImGui::TextColored(daysColor, "%d DAYS", bss_vip_days_remaining);
                } else {
                    ImGui::TextDisabled(xorstr_("FREE KEY"));
                }
                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

                ImGui::TextColored(themeColor, xorstr_("MENU INFORMATION"));
                ImGui::Separator(); ImGui::Spacing();
                ImGui::Text(xorstr_("Name    : SACREDBS VIP"));
                ImGui::Text(xorstr_("Version : 1.3"));
                ImGui::Text(xorstr_("Build   : 2026"));
                ImGui::Text(xorstr_("FPS     : %.1f"), io.Framerate);
                ImGui::Spacing(); ImGui::Separator();
                ImGui::TextColored(themeColor, xorstr_("FEATURES"));
                ImGui::Separator();
                ImGui::BulletText(xorstr_("ESP System (Line, Box, Skeleton)"));
                ImGui::BulletText(xorstr_("Aimbot & Magic Bullet"));
                ImGui::BulletText(xorstr_("No Recoil & No Spread"));
                ImGui::BulletText(xorstr_("Speed & Jump Hack"));
                ImGui::BulletText(xorstr_("Xray & Chams"));
                ImGui::BulletText(xorstr_("Anti-Telemetry Protection"));
                ImGui::Spacing(); ImGui::Separator();
                ImGui::TextColored(themeColor, xorstr_("CREDITS"));
                ImGui::Separator();
                ImGui::Text(xorstr_("Developer: RYUKO"));
                ImGui::BulletText(xorstr_("Telegram : t.me/SACREDBS_VIP"));
                ImGui::BulletText(xorstr_("Game Version : BloodStrike 1.003.650015"));
                ImGui::Spacing(); ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                ImGui::TextWrapped(xorstr_("WARNING: Use at your own risk, Play safe."));
                ImGui::PopStyleColor();
            }

            // ── TAB 6: SHIELD (Anti-Telemetry) ───────────────────────────
            else if (ActiveTab == 6) {
                if (!globalAntiTelemetry) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), xorstr_("Anti-Telemetry not initialized!"));
                } else {
                    SubBtn(xorstr_("Protection"), 1);
                    SubBtn(xorstr_("History"),    2);
                    SubBtn(xorstr_("Stats"),      3);
                    SubBtn(xorstr_("Settings"),   4);
                    ImGui::NewLine();
                    ImGui::Separator(); ImGui::Spacing();

                    if (ActiveSubTab == 1) {
                        static float pulse = 0.0f; pulse += 0.05f;
                        float intensity = 0.5f + 0.5f * sinf(pulse);
                        bool enabled = globalAntiTelemetry->getEnabled();
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(intensity, 0.0f, 0.0f, 1.0f));
                        ImGui::Text(xorstr_("PROTECTION STATUS"));
                        ImGui::PopStyleColor();
                        ImGui::Separator();
                        if (ImGui::Checkbox(xorstr_("Enable Protection"), &enabled))
                            globalAntiTelemetry->setEnabled(enabled);
                        ImGui::SameLine();
                        if (enabled) ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), xorstr_("● ACTIVE"));
                        else         ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), xorstr_("● DISABLED"));
                        ImGui::Separator();
                        int tot = globalAntiTelemetry->getTotalBlocked();
                        int unq = (int)globalAntiTelemetry->getBlockHistory().size();
                        ImGui::Text("Blocked: %d  |  Unique: %d", tot, unq);
                        ImGui::Spacing();
                        if (ImGui::CollapsingHeader(xorstr_("Protected Categories"), ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::Indent(10.0f);
                            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), xorstr_("Blocking:"));
                            ImGui::BulletText(xorstr_("Easebar telemetry (28+ domains)"));
                            ImGui::BulletText(xorstr_("NetEase telemetry"));
                            ImGui::BulletText(xorstr_("Firebase logging"));
                            ImGui::BulletText(xorstr_("AppsFlyer tracking"));
                            ImGui::BulletText(xorstr_("Anti-cheat systems"));
                            ImGui::BulletText(xorstr_("Analytics & crash reporting"));
                            ImGui::Unindent(10.0f);
                        }
                        ImGui::Spacing();
                        if (ImGui::Button(xorstr_("Run System Check"), ImVec2(-1, 28))) verifyProtectionActive();
                    }
                    else if (ActiveSubTab == 2) {
                        if (ImGui::CollapsingHeader(xorstr_("Block History"), ImGuiTreeNodeFlags_DefaultOpen)) {
                            static char searchFilter[128] = "";
                            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 60);
                            ImGui::InputTextWithHint(xorstr_("##search"), xorstr_("Search..."), searchFilter, IM_ARRAYSIZE(searchFilter));
                            ImGui::SameLine();
                            if (ImGui::Button(xorstr_("Clr"))) searchFilter[0] = '\0';
                            ImGui::Spacing();
                            float avH = ImGui::GetContentRegionAvail().y - 10;
                            ImGui::BeginChild(xorstr_("ScrollableHistory"), ImVec2(0, avH), true,
                                ImGuiWindowFlags_AlwaysVerticalScrollbar);
                            auto history = globalAntiTelemetry->getBlockHistory();
                            if (history.empty()) {
                                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), xorstr_("Waiting for telemetry..."));
                            } else {
                                std::string filterStr(searchFilter);
                                std::transform(filterStr.begin(), filterStr.end(), filterStr.begin(), ::tolower);
                                int visCount = 0;
                                for (size_t i = 0; i < history.size(); i++) {
                                    const auto& block = history[i];
                                    if (!filterStr.empty()) {
                                        std::string dl = block.domain;
                                        std::transform(dl.begin(), dl.end(), dl.begin(), ::tolower);
                                        if (dl.find(filterStr) == std::string::npos) continue;
                                    }
                                    visCount++;
                                    ImVec4 c;
                                    if      (block.reason.find("Anti-Cheat") != std::string::npos) c = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                                    else if (block.reason.find("Telemetry")  != std::string::npos ||
                                             block.reason.find("Analytics")  != std::string::npos) c = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
                                    else if (block.reason.find("Firebase")   != std::string::npos) c = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
                                    else if (block.reason.find("Easebar")    != std::string::npos) c = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                                    else                                                            c = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                                    ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(c.x*0.3f, c.y*0.3f, c.z*0.3f, 0.8f));
                                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(c.x*0.5f, c.y*0.5f, c.z*0.5f, 0.8f));
                                    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(c.x*0.7f, c.y*0.7f, c.z*0.7f, 0.8f));
                                    char lbl[512];
                                    snprintf(lbl, sizeof(lbl), "[%d] %s###blk%zu", block.count, block.domain.c_str(), i);
                                    if (ImGui::CollapsingHeader(lbl)) {
                                        ImGui::Indent(16.0f);
                                        ImGui::PushStyleColor(ImGuiCol_Text, c);
                                        ImGui::Text("Category: %s", block.reason.c_str());
                                        ImGui::PopStyleColor();
                                        ImGui::Text("Attempts: %d  |  Last: %s", block.count, block.lastBlocked.c_str());
                                        ImGui::Unindent(16.0f);
                                    }
                                    ImGui::PopStyleColor(3);
                                    ImGui::Spacing();
                                }
                                if (!filterStr.empty()) {
                                    ImGui::Separator();
                                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f),
                                        "Showing %d of %d", visCount, (int)history.size());
                                }
                            }
                            ImGui::EndChild();
                        }
                        ImGui::Spacing(); checkProtectionStatus();
                    }
                    else if (ActiveSubTab == 3) {
                        ImGui::TextColored(themeColor, xorstr_("DETAILED STATISTICS"));
                        ImGui::Separator();
                        int tot = globalAntiTelemetry->getTotalBlocked();
                        int unq = (int)globalAntiTelemetry->getBlockHistory().size();
                        ImGui::Text("Total Blocked: %d", tot);
                        ImGui::Text("Unique Domains: %d", unq);
                        ImGui::Spacing(); ImGui::Separator();
                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), xorstr_("Block History:"));
                        float histH = fminf(ImGui::GetContentRegionAvail().y - 10, 200.0f);
                        ImGui::BeginChild(xorstr_("StatsHistory"), ImVec2(0, histH), true);
                        auto history = globalAntiTelemetry->getBlockHistory();
                        if (history.empty()) {
                            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), xorstr_("No blocks yet..."));
                        } else {
                            for (size_t i = 0; i < history.size(); i++) {
                                const auto& block = history[i];
                                ImGui::Text("[%d] %s | %d times", (int)i+1, block.domain.c_str(), block.count);
                                ImGui::Separator();
                            }
                        }
                        ImGui::EndChild();
                    }
                    else if (ActiveSubTab == 4) {
                        ImGui::TextColored(themeColor, xorstr_("PROTECTION SETTINGS"));
                        ImGui::Separator(); ImGui::Spacing();
                        bool enabled = globalAntiTelemetry->getEnabled();
                        if (ImGui::Checkbox(xorstr_("Enable Protection"), &enabled))
                            globalAntiTelemetry->setEnabled(enabled);
                        ImGui::Spacing(); ImGui::Separator();
                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), xorstr_("Protected Categories:"));
                        ImGui::Indent(10.0f);
                        ImGui::BulletText(xorstr_("Easebar telemetry (28+ domains)"));
                        ImGui::BulletText(xorstr_("NetEase telemetry (log, stat, bi)"));
                        ImGui::BulletText(xorstr_("Firebase (logging, remote config)"));
                        ImGui::BulletText(xorstr_("AppsFlyer tracking"));
                        ImGui::BulletText(xorstr_("Anti-cheat systems"));
                        ImGui::BulletText(xorstr_("Analytics & tracking"));
                        ImGui::BulletText(xorstr_("Crash/error reporting"));
                        ImGui::Unindent(10.0f);
                        ImGui::Spacing(); ImGui::Separator();
                        if (ImGui::Button(xorstr_("Run System Check"), ImVec2(-1, 28))) verifyProtectionActive();
                        ImGui::Spacing(); checkProtectionStatus();
                        ImGui::Spacing(); ImGui::Separator();
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                        ImGui::TextWrapped(xorstr_("Keep ENABLED for maximum protection."));
                        ImGui::PopStyleColor();
                    }
                }
            }

            ImGui::EndChild();
            ImGui::SetWindowFontScale(1.0f);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImGui::PopStyleVar(8);
    ImGui::PopStyleColor(22);
    if (need_sync) sync_with_py();
}

        

static bool setup = false;

DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);

    if (Width <= 0 || Height <= 0)
        return _Draw(dpy, surface);

    if (!setup) {
        // ✅ HAPUS DEKLARASI static ImFont* DI SINI!
        // Sudah dipindahkan ke global scope di atas
        
        setup = true;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;

        static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
        ImFontConfig icons_config;
        ImFontConfig font_cfg;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5f;
        icons_config.OversampleV = 2.5f;
        
        io.Fonts->AddFontFromMemoryTTF((void*)roominfofont, sizeof(roominfofont), 
                                       28.5f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());
        
        // ✅ INISIALISASI FONTS (MENGGUNAKAN GLOBAL VARIABLES)
        F50 = io.Fonts->AddFontFromMemoryTTF((void*)roominfofont, sizeof(roominfofont), 
                                             50.0f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());
        Subtab = io.Fonts->AddFontFromMemoryTTF((void*)roominfofont, sizeof(roominfofont), 
                                                35.0f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());
        F48 = io.Fonts->AddFontFromMemoryTTF((void*)roominfofont, sizeof(roominfofont), 
                                            48.0f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());
        
        io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 
                                                 45.f, &icons_config, icons_ranges);
        
        TELEGRAM = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 
                                                            45.0f, &icons_config, icons_ranges);
        logoicon = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 
                                                            50.0f, &icons_config, icons_ranges);
        
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGui::StyleColorsClassic();

        ImGuiStyle& style = ImGui::GetStyle();

        style.Colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
        style.Colors[ImGuiCol_ChildBg]              = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);
        style.Colors[ImGuiCol_Border]               = ImVec4(0.15f, 0.15f, 0.15f, 0.60f);

        style.Colors[ImGuiCol_Button]               = ImVec4(0.16f, 0.16f, 0.16f, 0.60f);
        style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.00f, 0.48f, 1.00f, 0.65f);
        style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.00f, 0.48f, 1.00f, 0.85f);

        style.Colors[ImGuiCol_Header]               = ImVec4(0.00f, 0.48f, 1.00f, 0.60f);
        style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.00f, 0.48f, 1.00f, 0.80f);
        style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);

        style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
        style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.00f, 0.48f, 1.00f, 0.65f);
        style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.00f, 0.48f, 1.00f, 0.85f);

        style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);

        style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);

        style.Colors[ImGuiCol_Tab]                  = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_TabHovered]           = ImVec4(0.00f, 0.48f, 1.00f, 0.65f);
        style.Colors[ImGuiCol_TabActive]            = ImVec4(0.00f, 0.48f, 1.00f, 0.85f);

        style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.00f, 0.48f, 1.00f, 0.80f);
        style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);

        style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.00f, 0.48f, 1.00f, 0.30f);
        style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.00f, 0.48f, 1.00f, 0.50f);

        style.Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.00f, 0.48f, 1.00f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);

        style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.48f, 1.00f, 0.35f);

        style.Colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TableBorderLight]     = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);

        style.Colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        style.ItemInnerSpacing = ImVec2(8, 6);
        style.FrameBorderSize  = 1.0f;
        style.WindowRounding   = 7.0f;
        style.ChildRounding    = 5.0f;
        style.FrameRounding    = 5.0f;
        style.PopupRounding    = 5.0f;
        style.ScrollbarRounding= 5.0f;
        style.GrabRounding     = 5.0f;
        style.TabRounding      = 5.0f;
        style.WindowBorderSize = 2.0f;
        style.ChildBorderSize  = 2.0f;
        style.PopupBorderSize  = 2.0f;
        style.FrameBorderSize  = 2.0f;
        style.ItemSpacing      = ImVec2(8.0f, 6.0f);
        style.WindowPadding    = ImVec2(10.0f, 10.0f);
        style.FramePadding     = ImVec2(6.0f, 4.0f);
        style.IndentSpacing    = 20.0f;
        style.ScrollbarSize    = 1.0f;
        style.ScaleAllSizes(3.0f);

        setup = true;
    }

    // render setiap frame
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = (float)Width;
    io.DisplaySize.y = (float)Height;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    NewFrame();
    ClearImGuiWindowTracking();
        // Add domain scanner menu
    drawDomainScannerMenu();
        //parseWeaponScanResults();
        //saveScanSummary();
        drawAntiTelemetryMenu();  // TAMBAHKAN INI
        //HandleTouchInput(io); // TAMBAHKAN INI
    auto draw = GetBackgroundDrawList();
    noMore(draw);
        
    AutoInject();  // <--- INJECT OTOMATIS SETIAP FRAME
    DrawWaveModsMenu();
    //DrawMenu(io);
    EndFrame();
    Render();
    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return _Draw(dpy, surface);
}




