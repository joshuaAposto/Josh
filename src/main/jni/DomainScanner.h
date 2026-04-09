#ifndef DOMAINSCANNER_H
#define DOMAINSCANNER_H

#include <string>
#include <vector>
#include <set>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <android/log.h>

#define SCANNER_TAG "DomainScanner"
#define SCAN_LOGI(...) __android_log_print(ANDROID_LOG_INFO, SCANNER_TAG, __VA_ARGS__)

struct DomainInfo {
    std::string domain;
    std::string ip;
    int port;
    bool isResolved;
    std::string timestamp;
};

class DomainScanner {
private:
    std::set<std::string> scannedDomains;
    std::vector<DomainInfo> domainList;
    std::string savePath;
    bool isScanning;
    
    std::string getCurrentTimestamp();
    std::string resolveDomain(const std::string& domain);
    bool isValidDomain(const std::string& domain);
    bool isTargetDomain(const std::string& domain); // NEW
    
public:
    DomainScanner();
    void addDomain(const std::string& domain, int port = 0);
    void saveToDisk();
    void clearScans();
    int getScanCount();
    std::vector<DomainInfo> getAllDomains();
    void setEnabled(bool enabled);
    bool isEnabled();
};

extern DomainScanner* globalScanner;
void initDomainScanner();

#endif






/*

#ifndef DOMAINSCANNER_H
#define DOMAINSCANNER_H

#include <string>
#include <vector>
#include <set>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <android/log.h>

#define SCANNER_TAG "DomainScanner"
#define SCAN_LOGI(...) __android_log_print(ANDROID_LOG_INFO, SCANNER_TAG, __VA_ARGS__)

struct DomainInfo {
    std::string domain;
    std::string ip;
    int port;
    bool isResolved;
    std::string timestamp;
};

class DomainScanner {
private:
    std::set<std::string> scannedDomains;
    std::vector<DomainInfo> domainList;
    std::string savePath;
    bool isScanning;
    
    std::string getCurrentTimestamp();
    std::string resolveDomain(const std::string& domain);
    bool isValidDomain(const std::string& domain);
    
public:
    DomainScanner();
    void addDomain(const std::string& domain, int port = 0);
    void saveToDisk();
    void clearScans();
    int getScanCount();
    std::vector<DomainInfo> getAllDomains();
    void setEnabled(bool enabled);
    bool isEnabled();
};

extern DomainScanner* globalScanner;
void initDomainScanner();

#endif

*/


