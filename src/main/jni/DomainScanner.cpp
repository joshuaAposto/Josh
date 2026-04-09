#include "DomainScanner.h"
#include <cstdlib>
#include <cstdio>
#include <algorithm>

DomainScanner* globalScanner = nullptr;

DomainScanner::DomainScanner() {
    savePath = "/storage/emulated/0/";
    isScanning = true;
    
    SCAN_LOGI("Domain Scanner initialized");
    SCAN_LOGI("Save path: %s", savePath.c_str());
    SCAN_LOGI("Targeting: easebar.com, netease.com, bloodstrike");
}

std::string DomainScanner::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return std::string(buffer);
}

std::string DomainScanner::resolveDomain(const std::string& domain) {
    struct hostent* he;
    struct in_addr** addr_list;
    
    he = gethostbyname(domain.c_str());
    if (he == nullptr) {
        return "UNRESOLVED";
    }
    
    addr_list = (struct in_addr**)he->h_addr_list;
    if (addr_list[0] != nullptr) {
        return std::string(inet_ntoa(*addr_list[0]));
    }
    
    return "UNRESOLVED";
}

bool DomainScanner::isValidDomain(const std::string& domain) {
    if (domain.empty() || domain.length() < 3) return false;
    
    // Allow IPs and domains
    if (domain.find("127.0.0.1") != std::string::npos) return false;
    if (domain.find("localhost") != std::string::npos) return false;
    if (domain.find("0.0.0.0") != std::string::npos) return false;
    
    return true;
}

// Check if domain is priority target
bool DomainScanner::isTargetDomain(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), lowerDomain.begin(), ::tolower);
    
    return (lowerDomain.find("easebar.com") != std::string::npos ||
            lowerDomain.find("netease.com") != std::string::npos ||
            lowerDomain.find("163.com") != std::string::npos ||
            lowerDomain.find("126.com") != std::string::npos ||
            lowerDomain.find("127.net") != std::string::npos ||
            lowerDomain.find("bloodstrike") != std::string::npos ||
            lowerDomain.find("newspike") != std::string::npos);
}

void DomainScanner::addDomain(const std::string& domain, int port) {
    if (!isScanning) {
        SCAN_LOGI("Scanner disabled, not adding domain");
        return;
    }
    
    if (!isValidDomain(domain)) {
        SCAN_LOGI("Invalid domain: %s", domain.c_str());
        return;
    }
    
    if (scannedDomains.find(domain) != scannedDomains.end()) {
        SCAN_LOGI("Domain already exists: %s", domain.c_str());
        return;
    }
    
    scannedDomains.insert(domain);
    
    DomainInfo info;
    info.domain = domain;
    info.port = port;
    info.timestamp = getCurrentTimestamp();
    info.ip = resolveDomain(domain);
    info.isResolved = (info.ip != "UNRESOLVED");
    
    domainList.push_back(info);
    
    // Log dengan highlight untuk target domains
    if (isTargetDomain(domain)) {
        SCAN_LOGI("★★★ TARGET DOMAIN [%d]: %s -> %s:%d ★★★", 
                  (int)domainList.size(), domain.c_str(), info.ip.c_str(), port);
    } else {
        SCAN_LOGI("✓ Domain added [%d]: %s -> %s:%d", 
                  (int)domainList.size(), domain.c_str(), info.ip.c_str(), port);
    }
    
    // Auto-save setiap 3 domains (lebih sering)
    if (domainList.size() % 3 == 0) {
        SCAN_LOGI("Auto-saving at %d domains...", (int)domainList.size());
        saveToDisk();
    }
}

void DomainScanner::saveToDisk() {
    if (domainList.empty()) {
        SCAN_LOGI("No domains to save");
        return;
    }
    
    SCAN_LOGI("Starting save process...");
    
    // Try multiple paths
    std::vector<std::string> testPaths = {
        "/storage/emulated/0/domain.txt",
        "/sdcard/domain.txt",
        "/storage/self/primary/domain.txt"
    };
    
    bool saved = false;
    std::string successPath;
    
    for (const auto& testPath : testPaths) {
        SCAN_LOGI("Trying path: %s", testPath.c_str());
        
        FILE* file = fopen(testPath.c_str(), "w");
        if (!file) {
            SCAN_LOGI("Failed to open: %s (errno: %d)", testPath.c_str(), errno);
            continue;
        }
        
        // Write header
        fprintf(file, "========================================\n");
        fprintf(file, "   BLOOD STRIKE DOMAIN SCANNER\n");
        fprintf(file, "   TARGET: easebar.com & netease.com\n");
        fprintf(file, "========================================\n");
        fprintf(file, "Total Domains  : %d\n", (int)domainList.size());
        fprintf(file, "Last Updated   : %s\n", getCurrentTimestamp().c_str());
        fprintf(file, "File Location  : %s\n", testPath.c_str());
        fprintf(file, "========================================\n\n");
        
        // Separate target domains from others
        fprintf(file, "=== TARGET DOMAINS (easebar.com, netease.com) ===\n\n");
        
        int targetCount = 0;
        for (const auto& info : domainList) {
            if (isTargetDomain(info.domain)) {
                targetCount++;
                fprintf(file, "[★ %d] %s\n", targetCount, info.domain.c_str());
                fprintf(file, "    IP        : %s\n", info.ip.c_str());
                fprintf(file, "    Port      : %d\n", info.port);
                fprintf(file, "    Status    : %s\n", info.isResolved ? "RESOLVED" : "UNRESOLVED");
                fprintf(file, "    Time      : %s\n", info.timestamp.c_str());
                fprintf(file, "    -----------------------------------\n");
            }
        }
        
        if (targetCount == 0) {
            fprintf(file, "    (No target domains found yet)\n\n");
        }
        
        // Other domains
        fprintf(file, "\n=== OTHER DOMAINS ===\n\n");
        
        int otherCount = 0;
        for (const auto& info : domainList) {
            if (!isTargetDomain(info.domain)) {
                otherCount++;
                fprintf(file, "[%d] %s\n", otherCount, info.domain.c_str());
                fprintf(file, "    IP        : %s\n", info.ip.c_str());
                fprintf(file, "    Port      : %d\n", info.port);
                fprintf(file, "    Status    : %s\n", info.isResolved ? "RESOLVED" : "UNRESOLVED");
                fprintf(file, "    Time      : %s\n", info.timestamp.c_str());
                fprintf(file, "    -----------------------------------\n");
            }
        }
        
        fprintf(file, "\n========================================\n");
        fprintf(file, "Total: %d domains (%d targets, %d others)\n", 
                (int)domainList.size(), targetCount, otherCount);
        fprintf(file, "========================================\n");
        
        fclose(file);
        
        // Verify file was created
        FILE* verify = fopen(testPath.c_str(), "r");
        if (verify) {
            fclose(verify);
            saved = true;
            successPath = testPath;
            SCAN_LOGI("✓✓✓ SUCCESS! File saved to: %s", testPath.c_str());
            break;
        }
    }
    
    if (saved) {
        SCAN_LOGI("========================================");
        SCAN_LOGI("FILE SAVED SUCCESSFULLY!");
        SCAN_LOGI("Location: %s", successPath.c_str());
        SCAN_LOGI("Domains: %d", (int)domainList.size());
        SCAN_LOGI("========================================");
    } else {
        SCAN_LOGI("========================================");
        SCAN_LOGI("ERROR: Failed to save file!");
        SCAN_LOGI("========================================");
    }
}

void DomainScanner::clearScans() {
    scannedDomains.clear();
    domainList.clear();
    SCAN_LOGI("Cleared all scanned domains");
}

int DomainScanner::getScanCount() {
    return domainList.size();
}

std::vector<DomainInfo> DomainScanner::getAllDomains() {
    return domainList;
}

void DomainScanner::setEnabled(bool enabled) {
    isScanning = enabled;
    SCAN_LOGI("Scanner %s", enabled ? "ENABLED" : "DISABLED");
}

bool DomainScanner::isEnabled() {
    return isScanning;
}

void initDomainScanner() {
    if (globalScanner == nullptr) {
        globalScanner = new DomainScanner();
        SCAN_LOGI("✓ DomainScanner instance created");
        SCAN_LOGI("✓ Targeting easebar.com & netease.com subdomains");
    } else {
        SCAN_LOGI("DomainScanner already initialized");
    }
}



/*
#include "DomainScanner.h"
#include <cstdlib>

DomainScanner* globalScanner = nullptr;

DomainScanner::DomainScanner() {
    // LANGSUNG KE ROOT STORAGE - SIMPLE!
    savePath = "/storage/emulated/0/";
    isScanning = true;
    
    SCAN_LOGI("Domain Scanner initialized - saving to: %sdomain.txt", savePath.c_str());
}

std::string DomainScanner::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return std::string(buffer);
}

std::string DomainScanner::resolveDomain(const std::string& domain) {
    struct hostent* he;
    struct in_addr** addr_list;
    
    he = gethostbyname(domain.c_str());
    if (he == nullptr) {
        return "UNRESOLVED";
    }
    
    addr_list = (struct in_addr**)he->h_addr_list;
    if (addr_list[0] != nullptr) {
        return std::string(inet_ntoa(*addr_list[0]));
    }
    
    return "UNRESOLVED";
}

bool DomainScanner::isValidDomain(const std::string& domain) {
    if (domain.empty() || domain.length() < 3) return false;
    if (domain.find('.') == std::string::npos) return false;
    if (domain.find("127.0.0.1") != std::string::npos) return false;
    if (domain.find("localhost") != std::string::npos) return false;
    if (domain.find("0.0.0.0") != std::string::npos) return false;
    
    return true;
}

void DomainScanner::addDomain(const std::string& domain, int port) {
    if (!isScanning) return;
    if (!isValidDomain(domain)) return;
    
    if (scannedDomains.find(domain) != scannedDomains.end()) {
        return;
    }
    
    scannedDomains.insert(domain);
    
    DomainInfo info;
    info.domain = domain;
    info.port = port;
    info.timestamp = getCurrentTimestamp();
    info.ip = resolveDomain(domain);
    info.isResolved = (info.ip != "UNRESOLVED");
    
    domainList.push_back(info);
    
    SCAN_LOGI("Domain added: %s -> %s:%d", 
              domain.c_str(), info.ip.c_str(), port);
    
    // Auto-save setiap 5 domains
    if (domainList.size() % 5 == 0) {
        saveToDisk();
    }
}

void DomainScanner::saveToDisk() {
    if (domainList.empty()) {
        SCAN_LOGI("No domains to save");
        return;
    }
    
    // LANGSUNG KE domain.txt
    std::string filename = savePath + "domain.txt";
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        SCAN_LOGI("Failed to open file: %s", filename.c_str());
        return;
    }
    
    // Header
    file << "========================================\n";
    file << "   BLOOD STRIKE DOMAIN SCANNER\n";
    file << "========================================\n";
    file << "Total Domains  : " << domainList.size() << "\n";
    file << "Last Updated   : " << getCurrentTimestamp() << "\n";
    file << "File Location  : /storage/emulated/0/domain.txt\n";
    file << "========================================\n\n";
    
    // List all domains
    int counter = 1;
    for (const auto& info : domainList) {
        file << "[" << counter++ << "] " << info.domain << "\n";
        file << "    IP        : " << info.ip << "\n";
        file << "    Port      : " << info.port << "\n";
        file << "    Status    : " << (info.isResolved ? "RESOLVED" : "UNRESOLVED") << "\n";
        file << "    Time      : " << info.timestamp << "\n";
        file << "    -----------------------------------\n";
    }
    
    file << "\n========================================\n";
    file << "Total: " << domainList.size() << " domains scanned\n";
    file << "========================================\n";
    
    file.close();
    SCAN_LOGI("✓ Saved %d domains to: %s", (int)domainList.size(), filename.c_str());
}

void DomainScanner::clearScans() {
    scannedDomains.clear();
    domainList.clear();
    
    // Hapus file juga
    std::string filename = savePath + "domain.txt";
    remove(filename.c_str());
    
    SCAN_LOGI("Cleared all scanned domains and deleted file");
}

int DomainScanner::getScanCount() {
    return domainList.size();
}

std::vector<DomainInfo> DomainScanner::getAllDomains() {
    return domainList;
}

void DomainScanner::setEnabled(bool enabled) {
    isScanning = enabled;
    SCAN_LOGI("Scanner %s", enabled ? "ENABLED" : "DISABLED");
}

bool DomainScanner::isEnabled() {
    return isScanning;
}

void initDomainScanner() {
    if (globalScanner == nullptr) {
        globalScanner = new DomainScanner();
        SCAN_LOGI("✓ DomainScanner instance created");
    }
}

*/

