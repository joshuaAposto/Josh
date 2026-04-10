// ========================================
// AUTO OFFSET DETECTOR SYSTEM - IMPROVED
// File: auto_offset_detector.h
// ========================================

#ifndef AUTO_OFFSET_DETECTOR_H
#define AUTO_OFFSET_DETECTOR_H

#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <dlfcn.h>

// ========================================
// LOGGING HELPER
// ========================================
#include <android/log.h>
#define LOG_TAG "OffsetDetector"
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

// ========================================
// PATTERN SCANNER - IMPROVED
// ========================================
class OffsetScanner {
private:
    uintptr_t libBase;
    size_t libSize;
    
    // Cari pattern dengan wildcard
    std::vector<uintptr_t> ScanPattern(const char* pattern, const char* mask) {
        std::vector<uintptr_t> results;
        size_t patternLen = strlen(mask);
        uint8_t* data = reinterpret_cast<uint8_t*>(libBase);
        
        for (size_t i = 0; i < libSize - patternLen; i++) {
            bool found = true;
            for (size_t j = 0; j < patternLen; j++) {
                if (mask[j] == 'x' && data[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                results.push_back(libBase + i);
            }
        }
        return results;
    }
    
public:
    OffsetScanner(const char* libName) {
        libBase = GetLibraryBase(libName);
        libSize = GetLibrarySize(libName);
        ALOGI("Library Base: 0x%lX, Size: 0x%zX", libBase, libSize);
    }
    
    uintptr_t GetLibraryBase(const char* libraryName) {
        char line[512];
        uintptr_t base = 0;
        FILE* fp = fopen("/proc/self/maps", "r");
        
        if (fp) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, libraryName)) {
                    base = strtoul(line, NULL, 16);
                    break;
                }
            }
            fclose(fp);
        }
        return base;
    }
    
    size_t GetLibrarySize(const char* libraryName) {
        char line[512];
        uintptr_t start = 0, end = 0;
        FILE* fp = fopen("/proc/self/maps", "r");
        
        if (fp) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, libraryName)) {
                    char* dash = strchr(line, '-');
                    if (dash) {
                        start = strtoul(line, NULL, 16);
                        end = strtoul(dash + 1, NULL, 16);
                        break;
                    }
                }
            }
            fclose(fp);
        }
        return end - start;
    }
    
    // METODE 1: Scan magic value (0x3F800020 = 1.00006103516)
    std::vector<uintptr_t> FindMagicBulletValue() {
        std::vector<uintptr_t> candidates;
        uint8_t* data = reinterpret_cast<uint8_t*>(libBase);
        
        // Pattern untuk magic bullet: BC 20 80 3F (little endian float 1.00006)
        const uint8_t pattern[] = {0x71, 0x20, 0x80, 0x3F};
        const char mask[] = "xxxx";
        
        for (size_t i = 0; i < libSize - 4; i++) {
            if (memcmp(&data[i], pattern, 4) == 0) {
                uintptr_t offset = i;
                candidates.push_back(offset);
                ALOGI("Found magic value at offset: 0x%lX", offset);
            }
        }
        return candidates;
    }
    
    // METODE 2: Scan berdasarkan instruction pattern di sekitar offset
    std::vector<uintptr_t> FindByInstructionPattern() {
        std::vector<uintptr_t> candidates;
        uint8_t* data = reinterpret_cast<uint8_t*>(libBase);
        
        // Pattern instruksi ARM64 di sekitar magic bullet
        // Cari: LDR + FMOV sequence yang umum di game engine
        for (size_t i = 0; i < libSize - 16; i++) {
            // LDR instruction (load register)
            if ((data[i] & 0x38) == 0x80 || (data[i] & 0x00) == 0x00) {
                // Check untuk FMOV di dekatnya
                for (int offset = -8; offset <= 8; offset += 4) {
                    size_t checkPos = i + offset;
                    if (checkPos < libSize - 4) {
                        // FMOV pattern
                        if ((data[checkPos] & 0xBF) == 0x1E) {
                            candidates.push_back(i);
                            break;
                        }
                    }
                }
            }
        }
        return candidates;
    }
    
    // METODE 3: Scan berdasarkan string reference
    std::vector<uintptr_t> FindByStringRef(const char* searchStr) {
        std::vector<uintptr_t> candidates;
        uint8_t* data = reinterpret_cast<uint8_t*>(libBase);
        
        // Cari string di memory
        size_t strLen = strlen(searchStr);
        for (size_t i = 0; i < libSize - strLen; i++) {
            if (memcmp(&data[i], searchStr, strLen) == 0) {
                uintptr_t strAddr = libBase + i;
                ALOGI("Found string '%s' at: 0x%lX", searchStr, strAddr);
                
                // Cari referensi ke string ini
                for (size_t j = 0; j < libSize - 8; j += 4) {
                    // Safe pointer access
                    uintptr_t ptrValue;
                    memcpy(&ptrValue, &data[j], sizeof(uintptr_t));
                    if (ptrValue == strAddr) {
                        candidates.push_back(j);
                    }
                }
            }
        }
        return candidates;
    }
    
    // METODE 4: Scan dengan fuzzy matching (toleransi byte berbeda)
    std::vector<uintptr_t> FuzzyScanFloat(float targetValue, float tolerance = 0.01f) {
        std::vector<uintptr_t> candidates;
        uint8_t* data = reinterpret_cast<uint8_t*>(libBase);
        
        for (size_t i = 0; i < libSize - 4; i++) {
            float* fptr = reinterpret_cast<float*>(&data[i]);
            float value = *fptr;
            
            if (value >= (targetValue - tolerance) && value <= (targetValue + tolerance)) {
                candidates.push_back(i);
            }
        }
        return candidates;
    }
    
    // Filter kandidat berdasarkan validitas
    uintptr_t FilterBestCandidate(const std::vector<uintptr_t>& candidates) {
        if (candidates.empty()) return 0;
        
        // Prioritas: offset dalam range yang masuk akal
        for (auto offset : candidates) {
            // Magic bullet biasanya di range 0x1000000 - 0x2000000
            if (offset >= 0x1000000 && offset <= 0x2000000) {
                // Check alignment (harus aligned 4 bytes)
                if (offset % 4 == 0) {
                    return offset;
                }
            }
        }
        
        // Fallback: ambil yang paling dekat dengan offset lama
        uintptr_t oldOffset = 0x168AAD8;
        uintptr_t closest = candidates[0];
        size_t minDiff = SIZE_MAX;
        
        for (auto offset : candidates) {
            size_t diff = (offset > oldOffset) ? 
                (offset - oldOffset) : (oldOffset - offset);
            if (diff < minDiff) {
                minDiff = diff;
                closest = offset;
            }
        }
        
        return closest;
    }
};

// ========================================
// AUTO UPDATER - IMPROVED
// ========================================
class OffsetAutoUpdater {
private:
    OffsetScanner* scanner;
    uintptr_t currentOffset;
    bool isValid;
    std::vector<uintptr_t> allCandidates;
    
public:
    OffsetAutoUpdater(const char* libName) {
        scanner = new OffsetScanner(libName);
        currentOffset = 0x168AAD8; // Default fallback
        isValid = false;
    }
    
    ~OffsetAutoUpdater() {
        delete scanner;
    }
    
    // AUTO DETECT dengan multiple methods
    bool AutoDetect() {
        ALOGI("=== Starting Auto Offset Detection ===");
        allCandidates.clear();
        
        // METODE 1: Cari magic value langsung
        ALOGI("Method 1: Scanning magic value...");
        auto magicCandidates = scanner->FindMagicBulletValue();
        if (!magicCandidates.empty()) {
            ALOGI("Found %zu magic value candidates", magicCandidates.size());
            allCandidates.insert(allCandidates.end(), 
                magicCandidates.begin(), magicCandidates.end());
        }
        
        // METODE 2: Fuzzy float scan (1.0 dengan toleransi)
        ALOGI("Method 2: Fuzzy float scanning...");
        auto fuzzyCandidates = scanner->FuzzyScanFloat(1.00006103516f, 0.0001f);
        if (!fuzzyCandidates.empty()) {
            ALOGI("Found %zu fuzzy candidates", fuzzyCandidates.size());
            allCandidates.insert(allCandidates.end(), 
                fuzzyCandidates.begin(), fuzzyCandidates.end());
        }
        
        // METODE 3: Instruction pattern
        ALOGI("Method 3: Instruction pattern scan...");
        auto instrCandidates = scanner->FindByInstructionPattern();
        if (!instrCandidates.empty()) {
            ALOGI("Found %zu instruction pattern candidates", instrCandidates.size());
        }
        
        // Filter dan pilih kandidat terbaik
        if (!allCandidates.empty()) {
            currentOffset = scanner->FilterBestCandidate(allCandidates);
            ALOGI("Selected best candidate: 0x%lX", currentOffset);
            
            if (ValidateOffset()) {
                isValid = true;
                ALOGI("✓ Validation PASSED! Offset: 0x%lX", currentOffset);
                return true;
            }
        }
        
        ALOGW("Auto detection failed, using fallback offset");
        return false;
    }
    
    // Validasi offset
    bool ValidateOffset() {
        if (currentOffset == 0) return false;
        
        uintptr_t testAddr = scanner->GetLibraryBase("libGame.so") + currentOffset;
        
        // Check 1: Memory readable
        float testValue = 0.0f;
        if (memcpy(&testValue, (void*)testAddr, sizeof(float)) == nullptr) {
            ALOGE("Memory read failed at 0x%lX", testAddr);
            return false;
        }
        
        // Check 2: Value dalam range yang masuk akal
        if (testValue < 0.5f || testValue > 2.0f) {
            ALOGW("Value out of range: %.6f", testValue);
            return false;
        }
        
        // Check 3: Offset alignment
        if (currentOffset % 4 != 0) {
            ALOGW("Offset not aligned: 0x%lX", currentOffset);
            return false;
        }
        
        ALOGI("Validation passed: value=%.6f at 0x%lX", testValue, currentOffset);
        return true;
    }
    
    uintptr_t GetCurrentOffset() const { return currentOffset; }
    bool IsValid() const { return isValid; }
    
    const std::vector<uintptr_t>& GetAllCandidates() const { 
        return allCandidates; 
    }
    
    // Manual set offset (untuk testing)
    void SetOffset(uintptr_t offset) {
        currentOffset = offset;
        isValid = ValidateOffset();
    }
    
    // Export detailed log
    void ExportResults(const std::string& filename) {
        std::ofstream file(filename);
        file << "=== OFFSET DETECTOR LOG ===" << std::endl;
        file << "Current Offset: 0x" << std::hex << currentOffset << std::endl;
        file << "Valid: " << (isValid ? "YES" : "NO") << std::endl;
        file << "Library Base: 0x" << scanner->GetLibraryBase("libGame.so") << std::endl;
        file << std::endl;
        
        file << "All Candidates (" << std::dec << allCandidates.size() << "):" << std::endl;
        for (size_t i = 0; i < allCandidates.size() && i < 20; i++) {
            file << "  [" << i << "] 0x" << std::hex << allCandidates[i] << std::endl;
        }
        
        file.close();
        ALOGI("Log exported to: %s", filename.c_str());
    }
};

// ========================================
// GLOBAL INSTANCE
// ========================================
static OffsetAutoUpdater* g_offsetUpdater = nullptr;

// ========================================
// FUNCTIONS - DEKLARASI
// ========================================
void InitOffsetDetector();
void DrawOffsetDetectorUI();
bool QuickFindOffset();

#endif // AUTO_OFFSET_DETECTOR_H
