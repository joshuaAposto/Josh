#include "AntiTelemetry.h"
#include <algorithm>
#include <ctime>
#include <sstream>

AntiTelemetry* globalAntiTelemetry = nullptr;

AntiTelemetry::AntiTelemetry() {
    isEnabled = true;
    
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry System Initialized");
    TELE_LOGI("========================================");
    TELE_LOGI("Blocking categories:");
    TELE_LOGI("  ✓ Easebar telemetry (30+ domains)");
    TELE_LOGI("  ✓ NetEase telemetry (log, stat, bi, report)");
    TELE_LOGI("  ✓ BloodStrike telemetry & anti-cheat");
    TELE_LOGI("  ✓ Firebase Analytics & Remote Config");
    TELE_LOGI("  ✓ AppsFlyer tracking");
    TELE_LOGI("  ✓ Anti-cheat systems");
    TELE_LOGI("  ✓ Analytics & tracking");
    TELE_LOGI("  ✓ Crash reporting");
    TELE_LOGI("  ✓ Device fingerprinting");
    TELE_LOGI("  ✓ Third-party analytics");
    TELE_LOGI("  ✓ IP blacklist (40+ IPs)");
    TELE_LOGI("========================================");
    TELE_LOGI("Whitelisted (allowed):");
    TELE_LOGI("  ✓ game.163.com (gameplay)");
    TELE_LOGI("  ✓ login.163.com (auth)");
    TELE_LOGI("  ✓ cdn/update/match servers");
    TELE_LOGI("========================================");
}

// Helper function untuk check whitelist
bool AntiTelemetry::isWhitelisted(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Domain game yang BOLEH lewat (gameplay penting)
    std::vector<std::string> whitelist = {
        "game.163.com",           // Game server utama
        "api.game.163.com",       // API gameplay
        "login.163.com",          // Login server
        "auth.netease.com",       // Authentication
        "cdn.netease.com",        // Content delivery
        "update.netease.com",     // Game updates
        "match.netease.com",      // Matchmaking
        "voice.netease.com"       // Voice chat
    };
    
    for (const auto& allowed : whitelist) {
        if (lowerDomain.find(allowed) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string AntiTelemetry::getBlockReason(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Skip whitelist
    if (isWhitelisted(lowerDomain)) {
        return "";
    }
    
    // ========== EASEBAR SPECIFIC DOMAINS (HIGH PRIORITY) ==========
    //if (lowerDomain.find("easebar.com") != std::string::npos) return "Easebar Com";
    if (lowerDomain.find("netease.com") != std::string::npos) return "Easebar Com";
    if (lowerDomain.find("g83naxx1ena.gph.easebar.com") != std::string::npos) return "Easebar GPH";
    if (lowerDomain.find("applog.matrix.easebar.com") != std::string::npos) return "Easebar AppLog";
    if (lowerDomain.find("drpf-g83naxx1ena.proxima.nie.easebar.com") != std::string::npos) return "Easebar Proxima";
    if (lowerDomain.find("data-detect.nie.easebar.com") != std::string::npos) return "Easebar Data Detect";
    if (lowerDomain.find("sdk-os.mpsdk.easebar.com") != std::string::npos) return "Easebar SDK OS";
    if (lowerDomain.find("mgbnaeast-g83naxx1ena.unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK";
    if (lowerDomain.find("who.easebar.com") != std::string::npos) return "Easebar Who";
    if (lowerDomain.find("optsdk.gameyw.easebar.com") != std::string::npos) return "Easebar OptSDK";
    if (lowerDomain.find("sdkgate.pushv3.easebar.com") != std::string::npos) return "Easebar Push";
    if (lowerDomain.find("protocol.unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK Protocol";
    if (lowerDomain.find("unisdk.proxima.nie.easebar.com") != std::string::npos) return "Easebar UniSDK Proxima";
    if (lowerDomain.find("mcount.easebar.com") != std::string::npos) return "Easebar MCount";
    if (lowerDomain.find("httpdns.nie.easebar.com") != std::string::npos) return "Easebar HttpDNS";
    if (lowerDomain.find("g83naxx1ena.appdump.nie.easebar.com") != std::string::npos) return "Easebar AppDump";
    if (lowerDomain.find("mgbsdknaeast.matrix.easebar.com") != std::string::npos) return "Easebar Matrix SDK";
    if (lowerDomain.find("translate.mpsdk.easebar.com") != std::string::npos) return "Easebar Translate";
    if (lowerDomain.find("whoami-ipv4.nie.easebar.com") != std::string::npos) return "Easebar WhoAmI";
    if (lowerDomain.find("g0.gsf.easebar.com") != std::string::npos) return "Easebar GSF";
    if (lowerDomain.find("s.q.easebar.com") != std::string::npos) return "Easebar S.Q";
    if (lowerDomain.find("audiostatlog.cc.easebar.com") != std::string::npos) return "Easebar Audio Log";
    if (lowerDomain.find("nos.gameyw.easebar.com") != std::string::npos) return "Easebar NOS";
    
    // Generic Easebar patterns (catch-all for any missed domains)
    if (lowerDomain.find("matrix.easebar.com") != std::string::npos) return "Easebar Matrix";
    if (lowerDomain.find("proxima.nie.easebar.com") != std::string::npos) return "Easebar NIE Proxima";
    if (lowerDomain.find("nie.easebar.com") != std::string::npos) return "Easebar NIE";
    if (lowerDomain.find("mpsdk.easebar.com") != std::string::npos) return "Easebar MPSDK";
    if (lowerDomain.find("unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK";
    if (lowerDomain.find("gameyw.easebar.com") != std::string::npos) return "Easebar GameYW";
    if (lowerDomain.find("gph.easebar.com") != std::string::npos) return "Easebar GPH";
    if (lowerDomain.find("gsf.easebar.com") != std::string::npos) return "Easebar GSF";
    if (lowerDomain.find("monitor.easebar.com") != std::string::npos) return "Easebar Monitor";
    if (lowerDomain.find("data.easebar.com") != std::string::npos) return "Easebar Data";
    if (lowerDomain.find("report.easebar.com") != std::string::npos) return "Easebar Report";
    if (lowerDomain.find("analytics.easebar.com") != std::string::npos) return "Easebar Analytics";
    if (lowerDomain.find("track.easebar.com") != std::string::npos) return "Easebar Track";
    if (lowerDomain.find("stat.easebar.com") != std::string::npos) return "Easebar Stat";
    
    // ========== NETEASE NIE DOMAINS ==========
    if (lowerDomain.find("sigma-pharosv3-pathn.proxima.nie.netease.com") != std::string::npos) return "NetEase Sigma Pharos";
    if (lowerDomain.find("proxima.nie.netease.com") != std::string::npos) return "NetEase Proxima";
    if (lowerDomain.find("nie.netease.com") != std::string::npos) return "NetEase NIE";
    
    // ========== FIREBASE BLOCKING ==========
    if (lowerDomain.find("firebaselogging.googleapis.com") != std::string::npos) return "Firebase Logging";
    if (lowerDomain.find("firebaseremoteconfig.googleapis.com") != std::string::npos) return "Firebase Remote Config";
    if (lowerDomain.find("firebaseinstallations.googleapis.com") != std::string::npos) return "Firebase Installations";
    if (lowerDomain.find("firebaseanalytics") != std::string::npos) return "Firebase Analytics";
    if (lowerDomain.find("firebase") != std::string::npos) return "Firebase Services";
    
    // ========== APPSFLYER TRACKING ==========
    if (lowerDomain.find("c5ipmc.inapps.appsflyersdk.com") != std::string::npos) return "AppsFlyer InApps";
    if (lowerDomain.find("c5ipmc.launches.appsflyersdk.com") != std::string::npos) return "AppsFlyer Launches";
    if (lowerDomain.find("c5ipmc.cdn-settings.appsflyersdk.com") != std::string::npos) return "AppsFlyer CDN";
    if (lowerDomain.find("appsflyersdk.com") != std::string::npos) return "AppsFlyer SDK";
    if (lowerDomain.find("t.appsflyer.com") != std::string::npos) return "AppsFlyer Track";
    if (lowerDomain.find("events.appsflyer.com") != std::string::npos) return "AppsFlyer Events";
    if (lowerDomain.find("appsflyer") != std::string::npos) return "AppsFlyer";
    
    // ========== NETEASE SPECIFIC TELEMETRY ==========
    if (lowerDomain.find("log.netease.com") != std::string::npos) return "NetEase Logging";
    if (lowerDomain.find("stat.netease.com") != std::string::npos) return "NetEase Stats";
    if (lowerDomain.find("report.netease.com") != std::string::npos) return "NetEase Report";
    if (lowerDomain.find("bi.netease.com") != std::string::npos) return "NetEase BI";
    if (lowerDomain.find("bigdata.netease.com") != std::string::npos) return "NetEase BigData";
    if (lowerDomain.find("data.netease.com") != std::string::npos) return "NetEase Data";
    if (lowerDomain.find("analytics.netease.com") != std::string::npos) return "NetEase Analytics";
    if (lowerDomain.find("feedback.netease.com") != std::string::npos) return "NetEase Feedback";
    if (lowerDomain.find("analytics.bloodstrike.netease.com") != std::string::npos) return "BloodStrike Analytics";
    if (lowerDomain.find("bamreport.bloodstrike.netease.com") != std::string::npos) return "BloodStrike BAM Report";
    if (lowerDomain.find("reportban.netease.com") != std::string::npos) return "NetEase Report Ban";
    if (lowerDomain.find("ban.netease.com") != std::string::npos) return "NetEase Ban";
    if (lowerDomain.find("matchreport.bloodstrike.netease.com") != std::string::npos) return "BloodStrike Match Report";
    if (lowerDomain.find("securitycheck.netease.com") != std::string::npos) return "NetEase Security Check";
    if (lowerDomain.find("accountsecurity.netease.com") != std::string::npos) return "NetEase Account Security";
    if (lowerDomain.find("userlog.bloodstrike.netease.com") != std::string::npos) return "BloodStrike User Log";
    if (lowerDomain.find("qsec.netease.com") != std::string::npos) return "NetEase QSec";
    if (lowerDomain.find("tinfo.netease.com") != std::string::npos) return "NetEase TInfo";
    if (lowerDomain.find("log.qos.netease.com") != std::string::npos) return "NetEase QoS Log";
    if (lowerDomain.find("tracking.netease.com") != std::string::npos) return "NetEase Tracking";
    
    // NetEase 163.com domains
    if (lowerDomain.find("log.163.com") != std::string::npos) return "163 Logging";
    if (lowerDomain.find("stat.163.com") != std::string::npos) return "163 Stats";
    if (lowerDomain.find("bi.163.com") != std::string::npos) return "163 BI";
    
    // ========== ANTI-CHEAT SYSTEMS ==========
    if (lowerDomain.find("anticheat") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("anticheating") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("cheat") != std::string::npos && 
        lowerDomain.find("detect") != std::string::npos) return "Cheat Detection";
    if (lowerDomain.find("security") != std::string::npos && 
        (lowerDomain.find("check") != std::string::npos || 
         lowerDomain.find("verify") != std::string::npos)) return "Security Check";
    if (lowerDomain.find("protection") != std::string::npos) return "Protection System";
    if (lowerDomain.find("detection") != std::string::npos) return "Detection System";
    
    // ========== TELEMETRY & ANALYTICS ==========
    if (lowerDomain.find("telemetry") != std::string::npos) return "Telemetry";
    if (lowerDomain.find("analytics") != std::string::npos) return "Analytics";
    if (lowerDomain.find("tracking") != std::string::npos) return "Tracking";
    if (lowerDomain.find("metrics") != std::string::npos) return "Metrics";
    if (lowerDomain.find("statistics") != std::string::npos) return "Statistics";
    if (lowerDomain.find("stats") != std::string::npos) return "Statistics";
    if (lowerDomain.find("counter") != std::string::npos) return "Counter";
    if (lowerDomain.find("collect") != std::string::npos) return "Data Collection";
    
    // ========== CRASH & ERROR REPORTING ==========
    if (lowerDomain.find("crashlytics") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("crash") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("sentry") != std::string::npos) return "Error Report";
    if (lowerDomain.find("bugsnag") != std::string::npos) return "Bug Report";
    if (lowerDomain.find("error") != std::string::npos && 
        lowerDomain.find("report") != std::string::npos) return "Error Report";
    if (lowerDomain.find("exception") != std::string::npos) return "Exception Report";
    
    // ========== DEVICE INFO & FINGERPRINTING ==========
    if (lowerDomain.find("deviceinfo") != std::string::npos) return "Device Info";
    if (lowerDomain.find("fingerprint") != std::string::npos) return "Fingerprint";
    if (lowerDomain.find("integrity") != std::string::npos) return "Integrity Check";
    if (lowerDomain.find("device") != std::string::npos && 
        lowerDomain.find("check") != std::string::npos) return "Device Check";
    if (lowerDomain.find("safetynet") != std::string::npos) return "SafetyNet";
    if (lowerDomain.find("playintegrity") != std::string::npos) return "Play Integrity";
    
    // ========== MONITORING & SURVEILLANCE ==========
    if (lowerDomain.find("monitor") != std::string::npos) return "Monitoring";
    if (lowerDomain.find("surveillance") != std::string::npos) return "Surveillance";
    if (lowerDomain.find("watchdog") != std::string::npos) return "Watchdog";
    if (lowerDomain.find("observer") != std::string::npos) return "Observer";
    
    // ========== KNOWN ANTI-CHEAT SERVICES ==========
    if (lowerDomain.find("battleye") != std::string::npos) return "BattleEye";
    if (lowerDomain.find("easyanticheat") != std::string::npos) return "EasyAntiCheat";
    if (lowerDomain.find("vac") != std::string::npos) return "VAC";
    if (lowerDomain.find("xigncode") != std::string::npos) return "XIGNCODE";
    if (lowerDomain.find("gameguard") != std::string::npos) return "nProtect GameGuard";
    if (lowerDomain.find("nprotect") != std::string::npos) return "nProtect";
    if (lowerDomain.find("wellbia") != std::string::npos) return "Wellbia";
    
    // ========== BEHAVIOR TRACKING ==========
    if (lowerDomain.find("behavior") != std::string::npos) return "Behavior Track";
    if (lowerDomain.find("activity") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Activity Track";
    if (lowerDomain.find("usage") != std::string::npos) return "Usage Track";
    if (lowerDomain.find("session") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Session Track";
    
    // ========== THIRD-PARTY ANALYTICS ==========
    if (lowerDomain.find("google-analytics") != std::string::npos) return "Google Analytics";
    if (lowerDomain.find("mixpanel") != std::string::npos) return "Mixpanel";
    if (lowerDomain.find("amplitude") != std::string::npos) return "Amplitude";
    if (lowerDomain.find("adjust.com") != std::string::npos) return "Adjust";
    if (lowerDomain.find("branch.io") != std::string::npos) return "Branch.io";
    if (lowerDomain.find("api.access.umeng.com") != std::string::npos) return "Umeng Analytics";
    
    // ========== BLOODSTRIKE SPECIFIC ==========
    if (lowerDomain.find("report.bloodstrike.net") != std::string::npos) return "BloodStrike Report";
    if (lowerDomain.find("report-api.bloodstrike.io") != std::string::npos) return "BloodStrike Report API";
    if (lowerDomain.find("report-api.bloodstrike.net") != std::string::npos) return "BloodStrike Report API";
    if (lowerDomain.find("appeal-api.bloodstrike.io") != std::string::npos) return "BloodStrike Appeal API";
    if (lowerDomain.find("log.bloodstrike.net") != std::string::npos) return "BloodStrike Log";
    if (lowerDomain.find("track.bloodstrike.io") != std::string::npos) return "BloodStrike Track";
    if (lowerDomain.find("log-collector.bloodstrike.com") != std::string::npos) return "BloodStrike Log Collector";
    if (lowerDomain.find("stat-api.bloodstrike.net") != std::string::npos) return "BloodStrike Stat API";
    if (lowerDomain.find("anticheat.bloodstrike.net") != std::string::npos) return "BloodStrike AntiCheat";
    if (lowerDomain.find("anticheat-api.bloodstrike.com") != std::string::npos) return "BloodStrike AntiCheat API";
    if (lowerDomain.find("update-api.bloodstrike.io") != std::string::npos) return "BloodStrike Update API";
    if (lowerDomain.find("config.bloodstrike.io") != std::string::npos) return "BloodStrike Config";
    if (lowerDomain.find("log-api.dragonflygames") != std::string::npos) return "DragonFly Log API";
    if (lowerDomain.find("log-tracker.dragonflygames.cn") != std::string::npos) return "DragonFly Tracker";
    
    // ========== GOOGLE SERVICES ==========
    if (lowerDomain.find("android.googleapis.com") != std::string::npos) return "Google Android API";
    if (lowerDomain.find("prod-lt-playstoregatewayadapter-pa.googleapis.com") != std::string::npos) return "Google Play Gateway";
    if (lowerDomain.find("crashlyticsreports-pa.googleapis.com") != std::string::npos) return "Crashlytics Reports";
    if (lowerDomain.find("app-measurement.com") != std::string::npos) return "Google App Measurement";
    
    return "";
}

bool AntiTelemetry::shouldBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    std::string reason = getBlockReason(domain);
    return !reason.empty();
}

void AntiTelemetry::logBlock(const std::string& domain, const std::string& reason) {
    // Check if already in history
    for (auto& block : blockHistory) {
        if (block.domain == domain) {
            block.count++;
            
            // Update timestamp
            time_t now = time(nullptr);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
            block.lastBlocked = std::string(buffer);
            
            TELE_LOGI("🚫 BLOCKED [%dx]: %s [%s]", 
                      block.count, domain.c_str(), reason.c_str());
            return;
        }
    }
    
    // Add new entry
    BlockedRequest newBlock;
    newBlock.domain = domain;
    newBlock.reason = reason;
    newBlock.count = 1;
    
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    newBlock.lastBlocked = std::string(buffer);
    
    blockHistory.push_back(newBlock);
    
    TELE_LOGI("========================================");
    TELE_LOGI("🚫 NEW DOMAIN BLOCKED!");
    TELE_LOGI("Domain: %s", domain.c_str());
    TELE_LOGI("Reason: %s", reason.c_str());
    TELE_LOGI("Total Unique: %d", (int)blockHistory.size());
    TELE_LOGI("========================================");
}

bool AntiTelemetry::checkAndBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    if (shouldBlock(domain)) {
        std::string reason = getBlockReason(domain);
        logBlock(domain, reason);
        return true;
    }
    
    return false;
}

void AntiTelemetry::setEnabled(bool enabled) {
    isEnabled = enabled;
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry %s", enabled ? "ENABLED" : "DISABLED");
    TELE_LOGI("========================================");
}

bool AntiTelemetry::getEnabled() {
    return isEnabled;
}

std::vector<BlockedRequest> AntiTelemetry::getBlockHistory() {
    return blockHistory;
}

int AntiTelemetry::getTotalBlocked() {
    int total = 0;
    for (const auto& block : blockHistory) {
        total += block.count;
    }
    return total;
}

void initAntiTelemetry() {
    if (globalAntiTelemetry == nullptr) {
        globalAntiTelemetry = new AntiTelemetry();
        TELE_LOGI("✓ Anti-Telemetry instance created");
        TELE_LOGI("✓ Ready to block telemetry & anti-cheat");
        TELE_LOGI("✓ Blocking 30+ Easebar domains");
        TELE_LOGI("✓ Blocking Firebase & AppsFlyer");
        TELE_LOGI("✓ Blocking BloodStrike telemetry");
        TELE_LOGI("✓ IP blacklist active (40+ IPs)");
    }
}










/*
#include "AntiTelemetry.h"
#include <algorithm>
#include <ctime>
#include <sstream>

AntiTelemetry* globalAntiTelemetry = nullptr;

AntiTelemetry::AntiTelemetry() {
    isEnabled = true;
    
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry System Initialized");
    TELE_LOGI("========================================");
    TELE_LOGI("Blocking categories:");
    TELE_LOGI("  ✓ Easebar telemetry (28+ domains)");
    TELE_LOGI("  ✓ NetEase telemetry (log, stat, bi, report)");
    TELE_LOGI("  ✓ Firebase Analytics & Remote Config");
    TELE_LOGI("  ✓ AppsFlyer tracking");
    TELE_LOGI("  ✓ Anti-cheat systems");
    TELE_LOGI("  ✓ Analytics & tracking");
    TELE_LOGI("  ✓ Crash reporting");
    TELE_LOGI("  ✓ Device fingerprinting");
    TELE_LOGI("  ✓ Third-party analytics");
    TELE_LOGI("  ✓ IP blacklist (2 IPs)");
    TELE_LOGI("========================================");
    TELE_LOGI("Whitelisted (allowed):");
    TELE_LOGI("  ✓ game.163.com (gameplay)");
    TELE_LOGI("  ✓ login.163.com (auth)");
    TELE_LOGI("  ✓ cdn/update/match servers");
    TELE_LOGI("========================================");
}

// Helper function untuk check whitelist
bool AntiTelemetry::isWhitelisted(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Domain game yang BOLEH lewat (gameplay penting)
    std::vector<std::string> whitelist = {
        "game.163.com",           // Game server utama
        "api.game.163.com",       // API gameplay
        "login.163.com",          // Login server
        "auth.netease.com",       // Authentication
        "cdn.netease.com",        // Content delivery
        "update.netease.com",     // Game updates
        "match.netease.com",      // Matchmaking
        "voice.netease.com"       // Voice chat
    };
    
    for (const auto& allowed : whitelist) {
        if (lowerDomain.find(allowed) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string AntiTelemetry::getBlockReason(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Skip whitelist
    if (isWhitelisted(lowerDomain)) {
        return "";
    }
    
    // ========== EASEBAR SPECIFIC DOMAINS (HIGH PRIORITY) ==========
	//if (lowerDomain.find("easebar.com") != std::string::npos) return "Easebar Com";
	if (lowerDomain.find("NeteaSe.com") != std::string::npos) return "Easebar Com";
    if (lowerDomain.find("g83naxx1ena.gph.easebar.com") != std::string::npos) return "Easebar GPH";
    if (lowerDomain.find("applog.matrix.easebar.com") != std::string::npos) return "Easebar AppLog";
    if (lowerDomain.find("drpf-g83naxx1ena.proxima.nie.easebar.com") != std::string::npos) return "Easebar Proxima";
    if (lowerDomain.find("data-detect.nie.easebar.com") != std::string::npos) return "Easebar Data Detect";
    if (lowerDomain.find("sdk-os.mpsdk.easebar.com") != std::string::npos) return "Easebar SDK OS";
    if (lowerDomain.find("mgbnaeast-g83naxx1ena.unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK";
    if (lowerDomain.find("who.easebar.com") != std::string::npos) return "Easebar Who";
    if (lowerDomain.find("optsdk.gameyw.easebar.com") != std::string::npos) return "Easebar OptSDK";
    if (lowerDomain.find("sdkgate.pushv3.easebar.com") != std::string::npos) return "Easebar Push";
    if (lowerDomain.find("protocol.unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK Protocol";
    if (lowerDomain.find("unisdk.proxima.nie.easebar.com") != std::string::npos) return "Easebar UniSDK Proxima";
    if (lowerDomain.find("mcount.easebar.com") != std::string::npos) return "Easebar MCount";
    if (lowerDomain.find("httpdns.nie.easebar.com") != std::string::npos) return "Easebar HttpDNS";
    if (lowerDomain.find("g83naxx1ena.appdump.nie.easebar.com") != std::string::npos) return "Easebar AppDump";
    if (lowerDomain.find("mgbsdknaeast.matrix.easebar.com") != std::string::npos) return "Easebar Matrix SDK";
    if (lowerDomain.find("translate.mpsdk.easebar.com") != std::string::npos) return "Easebar Translate";
    if (lowerDomain.find("whoami-ipv4.nie.easebar.com") != std::string::npos) return "Easebar WhoAmI";
    if (lowerDomain.find("g0.gsf.easebar.com") != std::string::npos) return "Easebar GSF";
    if (lowerDomain.find("s.q.easebar.com") != std::string::npos) return "Easebar S.Q";
	
    
    // Generic Easebar patterns (catch-all for any missed domains)
    if (lowerDomain.find("matrix.easebar.com") != std::string::npos) return "Easebar Matrix";
    if (lowerDomain.find("proxima.nie.easebar.com") != std::string::npos) return "Easebar NIE Proxima";
    if (lowerDomain.find("nie.easebar.com") != std::string::npos) return "Easebar NIE";
    if (lowerDomain.find("mpsdk.easebar.com") != std::string::npos) return "Easebar MPSDK";
    if (lowerDomain.find("unisdk.easebar.com") != std::string::npos) return "Easebar UniSDK";
    if (lowerDomain.find("gameyw.easebar.com") != std::string::npos) return "Easebar GameYW";
    if (lowerDomain.find("gph.easebar.com") != std::string::npos) return "Easebar GPH";
    if (lowerDomain.find("gsf.easebar.com") != std::string::npos) return "Easebar GSF";
    
    // ========== NETEASE NIE DOMAINS ==========
    if (lowerDomain.find("sigma-pharosv3-pathn.proxima.nie.netease.com") != std::string::npos) return "NetEase Sigma Pharos";
    if (lowerDomain.find("proxima.nie.netease.com") != std::string::npos) return "NetEase Proxima";
    if (lowerDomain.find("nie.netease.com") != std::string::npos) return "NetEase NIE";
    
    // ========== FIREBASE BLOCKING ==========
    if (lowerDomain.find("firebaselogging.googleapis.com") != std::string::npos) return "Firebase Logging";
    if (lowerDomain.find("firebaseremoteconfig.googleapis.com") != std::string::npos) return "Firebase Remote Config";
    if (lowerDomain.find("firebaseinstallations.googleapis.com") != std::string::npos) return "Firebase Installations";
    if (lowerDomain.find("firebaseanalytics") != std::string::npos) return "Firebase Analytics";
    if (lowerDomain.find("firebase") != std::string::npos) return "Firebase Services";
    
    // ========== APPSFLYER TRACKING ==========
    if (lowerDomain.find("c5ipmc.inapps.appsflyersdk.com") != std::string::npos) return "AppsFlyer InApps";
    if (lowerDomain.find("c5ipmc.launches.appsflyersdk.com") != std::string::npos) return "AppsFlyer Launches";
    if (lowerDomain.find("c5ipmc.cdn-settings.appsflyersdk.com") != std::string::npos) return "AppsFlyer CDN";
    if (lowerDomain.find("appsflyersdk.com") != std::string::npos) return "AppsFlyer SDK";
    if (lowerDomain.find("appsflyer") != std::string::npos) return "AppsFlyer";
    
    // ========== NETEASE SPECIFIC TELEMETRY ==========
    if (lowerDomain.find("log.netease.com") != std::string::npos) return "NetEase Logging";
    if (lowerDomain.find("stat.netease.com") != std::string::npos) return "NetEase Stats";
    if (lowerDomain.find("report.netease.com") != std::string::npos) return "NetEase Report";
    if (lowerDomain.find("bi.netease.com") != std::string::npos) return "NetEase BI";
    if (lowerDomain.find("bigdata.netease.com") != std::string::npos) return "NetEase BigData";
    if (lowerDomain.find("data.netease.com") != std::string::npos) return "NetEase Data";
    if (lowerDomain.find("analytics.netease.com") != std::string::npos) return "NetEase Analytics";
    if (lowerDomain.find("feedback.netease.com") != std::string::npos) return "NetEase Feedback";
    
    // NetEase 163.com domains
    if (lowerDomain.find("log.163.com") != std::string::npos) return "163 Logging";
    if (lowerDomain.find("stat.163.com") != std::string::npos) return "163 Stats";
    if (lowerDomain.find("bi.163.com") != std::string::npos) return "163 BI";
    
    // ========== ANTI-CHEAT SYSTEMS ==========
    if (lowerDomain.find("anticheat") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("anticheating") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("cheat") != std::string::npos && 
        lowerDomain.find("detect") != std::string::npos) return "Cheat Detection";
    if (lowerDomain.find("security") != std::string::npos && 
        (lowerDomain.find("check") != std::string::npos || 
         lowerDomain.find("verify") != std::string::npos)) return "Security Check";
    if (lowerDomain.find("protection") != std::string::npos) return "Protection System";
    if (lowerDomain.find("detection") != std::string::npos) return "Detection System";
    
    // ========== TELEMETRY & ANALYTICS ==========
    if (lowerDomain.find("telemetry") != std::string::npos) return "Telemetry";
    if (lowerDomain.find("analytics") != std::string::npos) return "Analytics";
    if (lowerDomain.find("tracking") != std::string::npos) return "Tracking";
    if (lowerDomain.find("metrics") != std::string::npos) return "Metrics";
    if (lowerDomain.find("statistics") != std::string::npos) return "Statistics";
    if (lowerDomain.find("stats") != std::string::npos) return "Statistics";
    if (lowerDomain.find("counter") != std::string::npos) return "Counter";
    if (lowerDomain.find("collect") != std::string::npos) return "Data Collection";
    
    // ========== CRASH & ERROR REPORTING ==========
    if (lowerDomain.find("crashlytics") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("crash") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("sentry") != std::string::npos) return "Error Report";
    if (lowerDomain.find("bugsnag") != std::string::npos) return "Bug Report";
    if (lowerDomain.find("error") != std::string::npos && 
        lowerDomain.find("report") != std::string::npos) return "Error Report";
    if (lowerDomain.find("exception") != std::string::npos) return "Exception Report";
    
    // ========== DEVICE INFO & FINGERPRINTING ==========
    if (lowerDomain.find("deviceinfo") != std::string::npos) return "Device Info";
    if (lowerDomain.find("fingerprint") != std::string::npos) return "Fingerprint";
    if (lowerDomain.find("integrity") != std::string::npos) return "Integrity Check";
    if (lowerDomain.find("device") != std::string::npos && 
        lowerDomain.find("check") != std::string::npos) return "Device Check";
    if (lowerDomain.find("safetynet") != std::string::npos) return "SafetyNet";
    if (lowerDomain.find("playintegrity") != std::string::npos) return "Play Integrity";
    
    // ========== MONITORING & SURVEILLANCE ==========
    if (lowerDomain.find("monitor") != std::string::npos) return "Monitoring";
    if (lowerDomain.find("surveillance") != std::string::npos) return "Surveillance";
    if (lowerDomain.find("watchdog") != std::string::npos) return "Watchdog";
    if (lowerDomain.find("observer") != std::string::npos) return "Observer";
    
    // ========== KNOWN ANTI-CHEAT SERVICES ==========
    if (lowerDomain.find("battleye") != std::string::npos) return "BattleEye";
    if (lowerDomain.find("easyanticheat") != std::string::npos) return "EasyAntiCheat";
    if (lowerDomain.find("vac") != std::string::npos) return "VAC";
    if (lowerDomain.find("xigncode") != std::string::npos) return "XIGNCODE";
    if (lowerDomain.find("gameguard") != std::string::npos) return "nProtect GameGuard";
    if (lowerDomain.find("nprotect") != std::string::npos) return "nProtect";
    if (lowerDomain.find("wellbia") != std::string::npos) return "Wellbia";
    
    // ========== BEHAVIOR TRACKING ==========
    if (lowerDomain.find("behavior") != std::string::npos) return "Behavior Track";
    if (lowerDomain.find("activity") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Activity Track";
    if (lowerDomain.find("usage") != std::string::npos) return "Usage Track";
    if (lowerDomain.find("session") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Session Track";
    
    // ========== THIRD-PARTY ANALYTICS ==========
    if (lowerDomain.find("google-analytics") != std::string::npos) return "Google Analytics";
    if (lowerDomain.find("mixpanel") != std::string::npos) return "Mixpanel";
    if (lowerDomain.find("amplitude") != std::string::npos) return "Amplitude";
    if (lowerDomain.find("adjust.com") != std::string::npos) return "Adjust";
    if (lowerDomain.find("branch.io") != std::string::npos) return "Branch.io";
    
    return "";
}

bool AntiTelemetry::shouldBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    std::string reason = getBlockReason(domain);
    return !reason.empty();
}

void AntiTelemetry::logBlock(const std::string& domain, const std::string& reason) {
    // Check if already in history
    for (auto& block : blockHistory) {
        if (block.domain == domain) {
            block.count++;
            
            // Update timestamp
            time_t now = time(nullptr);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
            block.lastBlocked = std::string(buffer);
            
            TELE_LOGI("🚫 BLOCKED [%dx]: %s [%s]", 
                      block.count, domain.c_str(), reason.c_str());
            return;
        }
    }
    
    // Add new entry
    BlockedRequest newBlock;
    newBlock.domain = domain;
    newBlock.reason = reason;
    newBlock.count = 1;
    
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    newBlock.lastBlocked = std::string(buffer);
    
    blockHistory.push_back(newBlock);
    
    TELE_LOGI("========================================");
    TELE_LOGI("🚫 NEW DOMAIN BLOCKED!");
    TELE_LOGI("Domain: %s", domain.c_str());
    TELE_LOGI("Reason: %s", reason.c_str());
    TELE_LOGI("Total Unique: %d", (int)blockHistory.size());
    TELE_LOGI("========================================");
}

bool AntiTelemetry::checkAndBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    if (shouldBlock(domain)) {
        std::string reason = getBlockReason(domain);
        logBlock(domain, reason);
        return true;
    }
    
    return false;
}

void AntiTelemetry::setEnabled(bool enabled) {
    isEnabled = enabled;
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry %s", enabled ? "ENABLED" : "DISABLED");
    TELE_LOGI("========================================");
}

bool AntiTelemetry::getEnabled() {
    return isEnabled;
}

std::vector<BlockedRequest> AntiTelemetry::getBlockHistory() {
    return blockHistory;
}

int AntiTelemetry::getTotalBlocked() {
    int total = 0;
    for (const auto& block : blockHistory) {
        total += block.count;
    }
    return total;
}

void initAntiTelemetry() {
    if (globalAntiTelemetry == nullptr) {
        globalAntiTelemetry = new AntiTelemetry();
        TELE_LOGI("✓ Anti-Telemetry instance created");
        TELE_LOGI("✓ Ready to block telemetry & anti-cheat");
        TELE_LOGI("✓ Blocking 28+ Easebar domains");
        TELE_LOGI("✓ Blocking Firebase & AppsFlyer");
        TELE_LOGI("✓ IP blacklist active");
    }
}







/*
#include "AntiTelemetry.h"
#include <algorithm>
#include <ctime>
#include <sstream>

AntiTelemetry* globalAntiTelemetry = nullptr;

AntiTelemetry::AntiTelemetry() {
    isEnabled = true;
    
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry System Initialized");
    TELE_LOGI("========================================");
    TELE_LOGI("Blocking categories:");
    TELE_LOGI("  ✓ NetEase telemetry (log, stat, bi, report)");
    TELE_LOGI("  ✓ Anti-cheat systems");
    TELE_LOGI("  ✓ Analytics & tracking");
    TELE_LOGI("  ✓ Crash reporting");
    TELE_LOGI("  ✓ Device fingerprinting");
    TELE_LOGI("  ✓ Third-party analytics");
    TELE_LOGI("========================================");
    TELE_LOGI("Whitelisted (allowed):");
    TELE_LOGI("  ✓ game.163.com (gameplay)");
    TELE_LOGI("  ✓ login.163.com (auth)");
    TELE_LOGI("  ✓ cdn/update/match servers");
    TELE_LOGI("========================================");
}

// Helper function untuk check whitelist
bool AntiTelemetry::isWhitelisted(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Domain game yang BOLEH lewat (gameplay penting)
    std::vector<std::string> whitelist = {
        "game.163.com",           // Game server utama
        "api.game.163.com",       // API gameplay
        "login.163.com",          // Login server
        "auth.netease.com",       // Authentication
        "cdn.netease.com",        // Content delivery
        "update.netease.com",     // Game updates
        "match.netease.com",      // Matchmaking
        "voice.netease.com"       // Voice chat
    };
    
    for (const auto& allowed : whitelist) {
        if (lowerDomain.find(allowed) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string AntiTelemetry::getBlockReason(const std::string& domain) {
    std::string lowerDomain = domain;
    std::transform(lowerDomain.begin(), lowerDomain.end(), 
                   lowerDomain.begin(), ::tolower);
    
    // Skip whitelist
    if (isWhitelisted(lowerDomain)) {
        return "";
    }
    
    // ========== NETEASE SPECIFIC TELEMETRY ==========
    // Domain NetEase yang pasti telemetry
    if (lowerDomain.find("log.netease.com") != std::string::npos) return "NetEase Logging";
    if (lowerDomain.find("stat.netease.com") != std::string::npos) return "NetEase Stats";
    if (lowerDomain.find("report.netease.com") != std::string::npos) return "NetEase Report";
    if (lowerDomain.find("bi.netease.com") != std::string::npos) return "NetEase BI";
    if (lowerDomain.find("bigdata.netease.com") != std::string::npos) return "NetEase BigData";
    if (lowerDomain.find("data.netease.com") != std::string::npos) return "NetEase Data";
    if (lowerDomain.find("analytics.netease.com") != std::string::npos) return "NetEase Analytics";
    if (lowerDomain.find("feedback.netease.com") != std::string::npos) return "NetEase Feedback";
    
    // NetEase 163.com domains
    if (lowerDomain.find("log.163.com") != std::string::npos) return "163 Logging";
    if (lowerDomain.find("stat.163.com") != std::string::npos) return "163 Stats";
    if (lowerDomain.find("bi.163.com") != std::string::npos) return "163 BI";
    
    // ========== ANTI-CHEAT SYSTEMS ==========
    if (lowerDomain.find("anticheat") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("anticheating") != std::string::npos) return "Anti-Cheat";
    if (lowerDomain.find("cheat") != std::string::npos && 
        lowerDomain.find("detect") != std::string::npos) return "Cheat Detection";
    if (lowerDomain.find("security") != std::string::npos && 
        (lowerDomain.find("check") != std::string::npos || 
         lowerDomain.find("verify") != std::string::npos)) return "Security Check";
    if (lowerDomain.find("protection") != std::string::npos) return "Protection System";
    if (lowerDomain.find("detection") != std::string::npos) return "Detection System";
    
    // ========== TELEMETRY & ANALYTICS ==========
    if (lowerDomain.find("telemetry") != std::string::npos) return "Telemetry";
    if (lowerDomain.find("analytics") != std::string::npos) return "Analytics";
    if (lowerDomain.find("tracking") != std::string::npos) return "Tracking";
    if (lowerDomain.find("metrics") != std::string::npos) return "Metrics";
    if (lowerDomain.find("statistics") != std::string::npos) return "Statistics";
    if (lowerDomain.find("stats") != std::string::npos) return "Statistics";
    if (lowerDomain.find("counter") != std::string::npos) return "Counter";
    if (lowerDomain.find("collect") != std::string::npos) return "Data Collection";
    
    // ========== CRASH & ERROR REPORTING ==========
    if (lowerDomain.find("crashlytics") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("crash") != std::string::npos) return "Crash Report";
    if (lowerDomain.find("sentry") != std::string::npos) return "Error Report";
    if (lowerDomain.find("bugsnag") != std::string::npos) return "Bug Report";
    if (lowerDomain.find("error") != std::string::npos && 
        lowerDomain.find("report") != std::string::npos) return "Error Report";
    if (lowerDomain.find("exception") != std::string::npos) return "Exception Report";
    
    // ========== DEVICE INFO & FINGERPRINTING ==========
    if (lowerDomain.find("deviceinfo") != std::string::npos) return "Device Info";
    if (lowerDomain.find("fingerprint") != std::string::npos) return "Fingerprint";
    if (lowerDomain.find("integrity") != std::string::npos) return "Integrity Check";
    if (lowerDomain.find("device") != std::string::npos && 
        lowerDomain.find("check") != std::string::npos) return "Device Check";
    if (lowerDomain.find("safetynet") != std::string::npos) return "SafetyNet";
    if (lowerDomain.find("playintegrity") != std::string::npos) return "Play Integrity";
    
    // ========== MONITORING & SURVEILLANCE ==========
    if (lowerDomain.find("monitor") != std::string::npos) return "Monitoring";
    if (lowerDomain.find("surveillance") != std::string::npos) return "Surveillance";
    if (lowerDomain.find("watchdog") != std::string::npos) return "Watchdog";
    if (lowerDomain.find("observer") != std::string::npos) return "Observer";
    
    // ========== KNOWN ANTI-CHEAT SERVICES ==========
    if (lowerDomain.find("battleye") != std::string::npos) return "BattleEye";
    if (lowerDomain.find("easyanticheat") != std::string::npos) return "EasyAntiCheat";
    if (lowerDomain.find("vac") != std::string::npos) return "VAC";
    if (lowerDomain.find("xigncode") != std::string::npos) return "XIGNCODE";
    if (lowerDomain.find("gameguard") != std::string::npos) return "nProtect GameGuard";
    if (lowerDomain.find("nprotect") != std::string::npos) return "nProtect";
    if (lowerDomain.find("wellbia") != std::string::npos) return "Wellbia";
    
    // ========== BEHAVIOR TRACKING ==========
    if (lowerDomain.find("behavior") != std::string::npos) return "Behavior Track";
    if (lowerDomain.find("activity") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Activity Track";
    if (lowerDomain.find("usage") != std::string::npos) return "Usage Track";
    if (lowerDomain.find("session") != std::string::npos && 
        lowerDomain.find("track") != std::string::npos) return "Session Track";
    
    // ========== THIRD-PARTY ANALYTICS ==========
    if (lowerDomain.find("google-analytics") != std::string::npos) return "Google Analytics";
    if (lowerDomain.find("firebase") != std::string::npos) return "Firebase Analytics";
    if (lowerDomain.find("mixpanel") != std::string::npos) return "Mixpanel";
    if (lowerDomain.find("amplitude") != std::string::npos) return "Amplitude";
    if (lowerDomain.find("appsflyer") != std::string::npos) return "AppsFlyer";
    if (lowerDomain.find("adjust.com") != std::string::npos) return "Adjust";
    if (lowerDomain.find("branch.io") != std::string::npos) return "Branch.io";
    
    return "";
}

bool AntiTelemetry::shouldBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    std::string reason = getBlockReason(domain);
    return !reason.empty();
}

void AntiTelemetry::logBlock(const std::string& domain, const std::string& reason) {
    // Check if already in history
    for (auto& block : blockHistory) {
        if (block.domain == domain) {
            block.count++;
            
            // Update timestamp
            time_t now = time(nullptr);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
            block.lastBlocked = std::string(buffer);
            
            TELE_LOGI("🚫 BLOCKED [%dx]: %s [%s]", 
                      block.count, domain.c_str(), reason.c_str());
            return;
        }
    }
    
    // Add new entry
    BlockedRequest newBlock;
    newBlock.domain = domain;
    newBlock.reason = reason;
    newBlock.count = 1;
    
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    newBlock.lastBlocked = std::string(buffer);
    
    blockHistory.push_back(newBlock);
    
    TELE_LOGI("========================================");
    TELE_LOGI("🚫 NEW DOMAIN BLOCKED!");
    TELE_LOGI("Domain: %s", domain.c_str());
    TELE_LOGI("Reason: %s", reason.c_str());
    TELE_LOGI("Total Unique: %d", (int)blockHistory.size());
    TELE_LOGI("========================================");
}

bool AntiTelemetry::checkAndBlock(const std::string& domain) {
    if (!isEnabled) return false;
    
    if (shouldBlock(domain)) {
        std::string reason = getBlockReason(domain);
        logBlock(domain, reason);
        return true;
    }
    
    return false;
}

void AntiTelemetry::setEnabled(bool enabled) {
    isEnabled = enabled;
    TELE_LOGI("========================================");
    TELE_LOGI("Anti-Telemetry %s", enabled ? "ENABLED" : "DISABLED");
    TELE_LOGI("========================================");
}

bool AntiTelemetry::getEnabled() {
    return isEnabled;
}

std::vector<BlockedRequest> AntiTelemetry::getBlockHistory() {
    return blockHistory;
}

int AntiTelemetry::getTotalBlocked() {
    int total = 0;
    for (const auto& block : blockHistory) {
        total += block.count;
    }
    return total;
}

void initAntiTelemetry() {
    if (globalAntiTelemetry == nullptr) {
        globalAntiTelemetry = new AntiTelemetry();
        TELE_LOGI("✓ Anti-Telemetry instance created");
        TELE_LOGI("✓ Ready to block telemetry & anti-cheat");
    }
}

*/
