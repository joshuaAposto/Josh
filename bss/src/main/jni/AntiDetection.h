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
// Silenced: writing to logcat reveals cheat presence to anti-cheat logcat scanners
#define ANTI_LOGI(...) ((void)0)

// Hide from memory scanning
void hideMemoryRegions();

// Hook anti-cheat functions
void hookAntiCheatFunctions();

// Hide root detection
void hideRootAccess();

// Hide Xposed/Magisk
void hideFrameworks();

// Hide from package scan
void hideFromPackageList();

// Obfuscate strings in memory
void obfuscateMemoryStrings();

// Initialize all anti-detection
void initAntiDetection();

// Anti-debugging
void antiDebugProtection();

// Hide modules from /proc/self/maps
void hideMapsEntries();

// Spoof system properties
void spoofSystemProperties();

#endif
