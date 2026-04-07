#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "json.hpp"


#include "includes.h"

#define FIFO_PATH "/data/data/" PACKAGE_NAME "/imgui/sync"

static std::map<std::string, bool> sync_bool = {

{"bFlashMenu", false},

    {"bAIM", false},
    {"bAIM_IgnoreBots", false},
    {"bAIM_CheckVisibility", false},
    {"bAIM_IgnoreKnocked", false},
    {"bAIM_SnapLine", false},
    {"bAIM_DrawFov", false},
    {"bAIM_NoTargetHideFov", false},
    {"bESP_DangerCircle", false},
    {"bESP_Distance", false},
    {"bDrawBulletLine", false},
{"bDumpWeap", false},
    {"bESP", false},
    {"bESP_Line", false},
    {"bESP_LineBots", false},
    {"bESP_Skeleton", false},
    {"bESP_Box", false},
    {"bESP_Health", false},
    {"bESP_Name", false},
    {"bESP_Distance", false},
    {"bESP_TeamID", false},
    {"bESP_Knockesp", false},
    {"bESP_Loot", false},

    {"bBulletTrack", false},
    {"bBulletTrack_IgnoreKnocked", false},
    {"bBulletTrack_IgnoreBots", false},
    
    {"bXray", false},
    {"bSkinHack", false},
    {"bSpeed", false},
    {"bNoRecoil", false},
    {"bNoShake", false},  
    {"bNoSpread", false},
    {"bFeature1", false},
        {"bDrawBulletLine", false},
//          {"bESP_Grenade", false},            // toggle grenade alert
  //  {"bESP_AlertOnlyOffscreen", false}, // kalau mau tampilkan hanya saat offscreen (opsional)

};
static std::map<std::string, bool> last_sync_bool = sync_bool;
static std::map<std::string, float> sync_float = {
    {"fAIM_Fov", 250.0f},
    {"fESP_BoxRounding", 1.0f},
    {"fAimbotFov", 0.50},
    {"fSpeed", 5.0f},
    {"fESP_LineThickness", 2.0f},
    {"fAIM_SnapStrength", 0.1f},
    {"fBulletTrack_Probability", 100.0f},
    {"fHeight", 0.0f},
    {"fWidth", 0.0f}
    // --- tambahan untuk grenade alert ---
//    {"fESP_GrenadeRadius", 15.0f},  // meter
//    {"fESP_AlertRadius",   60.0f},  // px
//    {"fESP_AlertSize",      8.0f}   // px
};
static std::map<std::string, int> sync_int = {
    {"iAIM_TargetBone", 0},
    {"iAIM_Trigger", 0},
    {"iESP_Point", 2},
    {"iBulletTrack_TargetBone", 0},
        {"SelectedWeaponID", 1},
    {"SelectedSkinID", 0},
};

//#include <map>
//#include <vector>





static const char* targetBone[] = { "Head", "Neck", "Upper Chest", "Lower Chest", "Pelvis", "Left Hand", "Right Hand", "Left Thigh", "Right Thigh", "Left Foot", "Right Foot", "Random" };
static const char* aimbotTrigger[] = { "Aim+Shoot", "Aim", "Shoot", "Always" };
static const char* espPoint[] = { "Bottom", "Center", "Top" };

static void sync_with_py() {
    json j = json::object();
    
    for (const auto& pair : sync_bool) j[pair.first] = pair.second;
    for (const auto& pair : sync_float) j[pair.first] = pair.second;
    for (const auto& pair : sync_int) j[pair.first] = pair.second;
    
    std::string serialized = j.dump();
    
    int fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
    if (fd >= 0) {
        write(fd, serialized.c_str(), serialized.length());
        write(fd, "\n", 1);
        close(fd);
        std::cout << "Sent to FIFO: " << serialized << std::endl;
    } else std::cerr << "Failed to open FIFO for writing, maybe no one's listening" << std::endl;
}
