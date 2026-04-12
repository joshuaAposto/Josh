#ifndef ANTITELEMETRY_H
#define ANTITELEMETRY_H

#include <string>
#include <vector>
#include <set>
#include <android/log.h>

#define TELEMETRY_TAG "AntiTelemetry"
// Silenced: logcat output is visible to anti-cheat scanners
#define TELE_LOGI(...) ((void)0)

// Struct untuk tracking blocked requests
struct BlockedRequest {
    std::string domain;
    std::string reason;
    int count;
    std::string lastBlocked;
};

class AntiTelemetry {
private:
    std::set<std::string> blockedDomains;
    std::vector<BlockedRequest> blockHistory;
    bool isEnabled;
    
    // Helper methods (PRIVATE)
    bool isWhitelisted(const std::string& domain);
    std::string getBlockReason(const std::string& domain);
    void logBlock(const std::string& domain, const std::string& reason);
    
public:
    AntiTelemetry();
    
    // Public methods
    bool checkAndBlock(const std::string& domain);
    bool shouldBlock(const std::string& domain);  // ← HANYA SATU, DI PUBLIC
    void setEnabled(bool enabled);
    bool getEnabled();
    std::vector<BlockedRequest> getBlockHistory();
    int getTotalBlocked();
};

extern AntiTelemetry* globalAntiTelemetry;
void initAntiTelemetry();

#endif






/*
#ifndef ANTITELEMETRY_H
#define ANTITELEMETRY_H

#include <string>
#include <vector>
#include <set>
#include <android/log.h>

#define TELEMETRY_TAG "AntiTelemetry"
// Silenced: logcat output is visible to anti-cheat scanners
#define TELE_LOGI(...) ((void)0)

// Struct untuk tracking blocked requests
struct BlockedRequest {
    std::string domain;
    std::string reason;
    int count;
    std::string lastBlocked;
};

class AntiTelemetry {
private:
    std::set<std::string> blockedDomains;
    std::vector<BlockedRequest> blockHistory;
    bool isEnabled;
    
    bool shouldBlock(const std::string& domain);
    bool isWhitelisted(const std::string& domain);
    std::string getBlockReason(const std::string& domain);
    void logBlock(const std::string& domain, const std::string& reason);
        
public:  // ← PASTIKAN shouldBlock ada di public
    AntiTelemetry();
    bool checkAndBlock(const std::string& domain);
    bool shouldBlock(const std::string& domain);  // ← Harus public!
    void setEnabled(bool enabled);
    bool getEnabled();
    std::vector<BlockedRequest> getBlockHistory();
    int getTotalBlocked();
};
        
        
        
 /*
public:
    AntiTelemetry();
    bool checkAndBlock(const std::string& domain);
        //bool shouldBlock(const std::string& domain);  // ← Harus public!
    void setEnabled(bool enabled);
    bool getEnabled();
    std::vector<BlockedRequest> getBlockHistory();
    int getTotalBlocked();
};

extern AntiTelemetry* globalAntiTelemetry;
void initAntiTelemetry();

#endif
*/

