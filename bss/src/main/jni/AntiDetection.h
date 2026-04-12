#ifndef ANTIDETECTION_H
#define ANTIDETECTION_H

#include <string>
#include <vector>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <android/log.h>
#include <fcntl.h>
#include <dirent.h>

#define ANTI_TAG "AntiDetect"
#define ANTI_LOGI(...) __android_log_print(ANDROID_LOG_INFO, ANTI_TAG, __VA_ARGS__)

void hideMemoryRegions();
void hookAntiCheatFunctions();
void hideRootAccess();
void hideFrameworks();
void hideFromPackageList();
void obfuscateMemoryStrings();
void initAntiDetection();
void antiDebugProtection();
void hideMapsEntries();
void spoofSystemProperties();
void hideEmulatorTraces();
void hideGameGuardianTraces();
void blockSuspiciousFileAccess();

#endif
