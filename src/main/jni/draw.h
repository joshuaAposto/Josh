#include "include/hpp/imgui.h"
#include <EGL/egl.h>
#include <string>
#include <sys/system_properties.h>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <thread>  
#include <atomic>  
#include <signal.h> // Added for Anti-Crack Process Killing
#include "Dobby/dobby.h"
#include "fake_dlfcn.h"

int Width, Height;
using namespace ImGui;
using namespace std;

const char *INI_PATH = "/data/data/" PACKAGE_NAME "/imgui/.ini";
bool bylogo = true;

#include "include/json.hpp" 
using json = nlohmann::json;

#include "Jni_Stuff.h"
#include "Oxorany/oxorany.h"
#include "include/hpp/dlfcn.hpp"
#include "include/hpp/imgui/persistence.h"
#include "include/hpp/imgui/touch.h"
#include "include/sync_with_py.h"
#include "include/sync_entities.h"
#include "Starcool.h"
#include "DreiFont.h"
#include <curl/curl.h>

// ========================================================================================================================
// START OF KEY SYSTEM LOGIC & HELPERS
// ========================================================================================================================

// [ANTI-CRACK]: Replaced vulnerable boolean with an Atomic State Machine
static std::atomic<uint32_t> auth_state(0x8A8A8A8A); 
const uint32_t AUTH_SUCCESS = 0x1337BABE; // Magic Key required to show the menu

static std::atomic<bool> is_authenticating(false); 
static char user_input_key[64] = "";
static char auth_status[256] = "Waiting for key..."; 
static std::string cached_uuid = "";
static int   vip_days_remaining = 0;
static bool  vip_days_received  = false;
static char  vip_username_str[64] = "";

std::string getDeviceName() {
    char model[PROP_VALUE_MAX];
    __system_property_get("ro.product.model", model);
    return std::string(model);
}

std::string getDeviceArchitecture() {
    char abi[PROP_VALUE_MAX];
    __system_property_get("ro.product.cpu.abi", abi);
    std::string arch(abi);
    arch.erase(std::remove(arch.begin(), arch.end(), '\n'), arch.end());
    return arch.empty() ? "Unknown Arch" : arch;
}

std::string GetCleanUUID() {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);

    jclass activityThreadClass = env->FindClass(oxorany("android/app/ActivityThread"));
    jmethodID currentActivityThreadMethod = env->GetStaticMethodID(activityThreadClass, oxorany("currentActivityThread"), oxorany("()Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->CallStaticObjectMethod(activityThreadClass, currentActivityThreadMethod);
    jmethodID getApplicationMethod = env->GetMethodID(activityThreadClass, oxorany("getApplication"), oxorany("()Landroid/app/Application;"));
    jobject mInitialApplication = env->CallObjectMethod(sCurrentActivityThread, getApplicationMethod);

    std::string raw_id = GetAndroidID(env, mInitialApplication);
    raw_id += GetDeviceModel(env);
    raw_id += GetDeviceBrand(env);

    std::string final_uuid = GetDeviceUniqueIdentifier(env, raw_id.c_str());

    jvm->DetachCurrentThread();
    return final_uuid;
}

#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

std::string GenerateSecuritySignature(const std::string& hwid) {
    std::string secret = oxorany("SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR");
    std::string string_to_hash = "hwid=" + hwid + secret;
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, string_to_hash.c_str(), string_to_hash.size());
    MD5_Final(hash, &md5);
    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string GetKeyGeneratorLink(const std::string& uuid) {
    std::string signature = GenerateSecuritySignature(uuid);
    return std::string(oxorany("https://bskey.vercel.app/?hwid=")) + uuid + oxorany("&signature=") + signature;
}

// NOTE: MemoryStruct and WriteMemoryCallback were removed here to fix the redefinition error.
// They are safely pulled from Jni_Stuff.h!

bool VerifyKeyWithServer(const std::string& key, const std::string& uuid) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        snprintf(auth_status, sizeof(auth_status), oxorany("Error: CURL failed to initialize"));
        return false;
    }

    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = 0; 

    char* escaped_key = curl_easy_escape(curl, key.c_str(), key.length());
    char* escaped_uuid = curl_easy_escape(curl, uuid.c_str(), uuid.length());
    
    if (!escaped_key || !escaped_uuid) {
        if(escaped_key) curl_free(escaped_key);
        if(escaped_uuid) curl_free(escaped_uuid);
        curl_easy_cleanup(curl);
        snprintf(auth_status, sizeof(auth_status), oxorany("Error: URL Encoding failed"));
        return false;
    }

    std::string url = oxorany("https://bskey.vercel.app/verify?key=");
    url += escaped_key;
    url += oxorany("&hwid=");
    url += escaped_uuid;

    curl_free(escaped_key);
    curl_free(escaped_uuid);

    struct MemoryStruct chunk{};
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); 
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L); 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, oxorany("libcurl-agent/1.0"));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    
    curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, oxorany("8.8.8.8,8.8.4.4"));
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    CURLcode res = curl_easy_perform(curl);
    bool isSuccess = false;

    if (res == CURLE_OK) {
        try {
            auto data = json::parse(chunk.memory);
            if (data.contains(oxorany("success")) && data[oxorany("success")].is_boolean()) {
                isSuccess = data[oxorany("success")].get<bool>();
                if (!isSuccess) {
                    const char* reason = "Invalid or Expired Key!";
                    if (data.contains("reason") && data["reason"].is_string()) {
                        std::string r = data["reason"].get<std::string>();
                        if (r.find("different device") != std::string::npos)
                            reason = "Key bound to different device!";
                        else if (r.find("revoked") != std::string::npos)
                            reason = "Key revoked by admin!";
                        else if (r.find("expired") != std::string::npos)
                            reason = "Key expired!";
                        else if (r.find("reset") != std::string::npos)
                            reason = "Key reset by admin! Get new free key.";
                    }
                    snprintf(auth_status, sizeof(auth_status), "Login Failed: %s", reason);
                } else {
                    // Parse days remaining and username from server response
                    if (data.contains("days_remaining") && data["days_remaining"].is_number()) {
                        vip_days_remaining = data["days_remaining"].get<int>();
                        vip_days_received  = true;
                    }
                    if (data.contains("username") && data["username"].is_string()) {
                        std::string uname = data["username"].get<std::string>();
                        strncpy(vip_username_str, uname.c_str(), sizeof(vip_username_str) - 1);
                    }
                    snprintf(auth_status, sizeof(auth_status), oxorany("Login Successful!"));
                    // [ANTI-CRACK] Store magic state instead of true/false
                    auth_state.store(AUTH_SUCCESS); 
                }
            } else {
                snprintf(auth_status, sizeof(auth_status), oxorany("Server Error: Contact Developer"));
            }
        } catch (const json::parse_error& e) {
            snprintf(auth_status, sizeof(auth_status), oxorany("Bad Server Response! Contact Developer"));
        }
    } else {
        size_t len = strlen(errbuf);
        if(len > 0) {
            snprintf(auth_status, sizeof(auth_status), oxorany("CURL %d: %s"), res, errbuf);
        } else {
            snprintf(auth_status, sizeof(auth_status), oxorany("CURL Error Code: %d"), res);
        }
    }

    free(chunk.memory);
    curl_easy_cleanup(curl);
    return isSuccess;
}


// ========================================================================================================================

ImColor 红色 = ImColor(ImVec4(233 / 255.f, 55 / 255.f, 51 / 255.f, 0.95f));
ImColor 空白 = ImColor(ImVec4(1.0 / 255.f, 1.0 / 255.f, 1.0 / 255.f, 0.0f));
ImColor 白色 = ImColor(ImVec4(255 / 255.f, 255 / 255.f, 258 / 255.f, 0.95f));
ImColor 黑色 = ImColor(ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.7f));
#define Yellow ImColor(255, 255, 0, 255)
#define Red ImColor(255, 0, 0, 255)
#define Green ImColor(0, 255, 0, 255)
#include "include/helpers.h"

inline ImVec4 RGBA2ImVec4(int r, int g, int b, int a) {
  float newr = (float)r / 255.0f;
  float newg = (float)g / 255.0f;
  float newb = (float)b / 255.0f;
  float newa = (float)a / 255.0f;
  return ImVec4(newr, newg, newb, newa);
}

static float fps = 60.0f;
static float last_time = 0.0f;
static ImVec4 fps_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
static float fps_update_time = 0.0f;
static const float FPS_UPDATE_INTERVAL = 0.5f;

const unordered_set<string> BLOCKED_DOMAINS = {
    "analytics.bloodstrike.netease.com",
    "bamreport.bloodstrike.netease.com",
    "reportban.netease.com",
    "ban.netease.com",
    "matchreport.bloodstrike.netease.com",
    "securitycheck.netease.com",
    "accountsecurity.netease.com",
    "userlog.bloodstrike.netease.com",
    "qsec.netease.com",
    "tinfo.netease.com",
    "log.qos.netease.com",
    "tracking.netease.com",
    "anticheat.bloodstrike.net",
    "anticheat-api.bloodstrike.com",
    "report.bloodstrike.net",
    "report-api.bloodstrike.io",
    "report-api.bloodstrike.net",
    "appeal-api.bloodstrike.io",
    "log.bloodstrike.net",
    "track.bloodstrike.io",
    "log-collector.bloodstrike.com",
    "stat-api.bloodstrike.net",
    "update-api.bloodstrike.io",
    "config.bloodstrike.io",
    "easebar.com",
    "monitor.easebar.com",
    "data.easebar.com",
    "report.easebar.com",
    "analytics.easebar.com",
    "track.easebar.com",
    "stat.easebar.com",
    "mgbsdknaeast.matrix.easebar.com",
    "protocol.unisdk.easebar.com",
    "translate.mpsdk.easebar.com",
    "g0.gsf.easebar.com",
    "sdkgate.pushv3.easebar.com",
    "who.easebar.com",
    "drpf-g83naxx1ena.proxima.nie.easebar.com",
    "data-detect.nie.easebar.com",
    "s.q.easebar.com",
    "optsdk.gameyw.easebar.com",
    "nos.gameyw.easebar.com",
    "g83naxx1ena.gph.easebar.com",
    "sdk-api.easebar.com",
    "cdn.easebar.com",
    "realtime.easebar.com",
    "cheatcheck.easebar.com",
    "api.easebar.com",
    "bs.easebar.com",
    "log.easebar.com",
    "whoami.nie.easebar.com",
    "sdk-os.mpsdk.easebar.com",
    "unisdk.proxima.nie.easebar.com",
    "b.q.easebar.com",
    "acsdk.gameyw.easebar.com",
    "dns.update.easebar.com",
    "newspike-avatar.fp.ps.easebar.com",
    "android.googleapis.com",
    "prod-lt-playstoregatewayadapter-pa.googleapis.com",
    "prod-it-playstoregatewayadapter-pa.googleapis.com",
    "firebaseremoteconfig.googleapis.com",
    "firebaseinstallations.googleapis.com",
    "connectivitycheck.gstatic.com",
    "crashlyticsreports-pa.googleapis.com",
    "crashlytics.com",
    "reports.crashlytics.com",
    "t.appsflyer.com",
    "events.appsflyer.com",
    "c5ipmc-inapps.appsflyersdk.com",
    "app-measurement.com",
    "api.access.umeng.com",
    "log-api.dragonflygames",
    "log-tracker.dragonflygames.cn",
    "litemsg.ekatox.com",
    "whoami.akamai.net",
    "googleapis.cn",
    "settings.crashlytics.com",
    "firebase-settings.crashlytics.com",
    "www.google-analytics.com",
    "ssl.google-analytics.com",
    "pagead2.googlesyndication.com",
    "googleads.g.doubleclick.net",
    "adservice.google.com",
    "adservice.google.co.id",
    "g.doubleclick.net",
    "ads.unityads.unity3d.com",
    "config.unityads.unity3d.com",
    "webview.unityads.unity3d.com",
    "unityads.unity3d.com",
    "unity.com",
    "analytics.cloud.unity3d.com",
    "log.byteoversea.com",
    "sf16-website-va.ibytedtos.com",
    "mon.isnssdk.com",
    "abtest-sg.byteoversea.com",
    "graph.facebook.com",
    "api.facebook.com",
    "facebook.com",
    "connect.facebook.net",
    "oth.eve.mdt.qq.com",
    "tdata.m.qq.com",
    "mtrace.qq.com",
    "aax-us-east.amazon-adsystem.com",
    "fls-na.amazon.com",
    "metrics.amazonaws.com",
    "o123456.ingest.sentry.io",
    "notify.bugsnag.com",
    "sessions.bugsnag.com",
    "app.adjust.com",
    "gdpr.adjust.com",
    "malicious.adjust.com",
    "data.hicloud.com",
    "metrics1.data.hicloud.com",
    "tracking.miui.com",
    "api.ad.xiaomi.com",
    "data.mistat.xiaomi.com",
    "adsfs.oppomobile.com",
    "adsfslocation.oppomobile.com",
    "ads-dra.oppo.com",
    "data.adrealme.com",
    "vivoice.vivo.com.cn",
    "onesignal.com",
    "api.onesignal.com",
    "push.onesignal.com",
    "stats.appsflyer.com",
    "log.revenuedigital.io",
    "api.sdkbox.com",
    "collect.tealiumiq.com",
    "track.appsflyer.com",
    "track.customer.io",
    "api.crashlytics.com",
    "update.crashlytics.com",
    "event.crashlytics.com",
    "analytics.google.com",
    "api.analytics.google.com",
    "upload.google-analytics.com",
    "ssl.gstatic.com",
    "gstatic.com",
    "fonts.googleapis.com",
    "fonts.gstatic.cn",
    "ssl.gstatic.cn",
    "maps.googleapis.com",
    "maps.gstatic.com",
    "maps.gstatic.cn",
    "clients1.google.com",
    "clients2.google.com",
    "clients3.google.com",
    "clients4.google.com",
    "play.googleapis.com",
    "update.googleapis.com",
    "safebrowsing.googleapis.com",
    "security.googleapis.com",
    "chromium-telemetry.appspot.com",
    "telemetry.apis.google.com",
    "telemetry.googleapis.com",
    "firebaseanalytics.googleapis.com",
    "firebaseperf.googleapis.com",
    "metrica.yandex.net",
    "mc.yandex.ru",
    "tracker.yandex.net",
    "reporting.yandex.net",
    "connect.ok.ru",
    "pushok.ru",
    "d.xtarget.ru",
    "metryka.sputnik.ru",
    "api-metrika.yandex.net",
    "inrix.com",
    "geo.yandex.net",
    "metrics.mail.ru",
    "stat.mail.ru",
    "track.mail.ru",
    "mc.mail.ru",
    "api.vk.com",
    "pixel.vk.com",
    "analytics.vk.com",
    "connect.vk.com",
    "stats.g.doubleclick.net",
    "securepubads.g.doubleclick.net",
    "pagead-googlehosted.l.google.com",
    "pubads.g.doubleclick.net",
    "adservice.google.com.au",
    "adservice.google.co.uk",
    "ads.youtube.com",
    "youtubei.googleapis.com",
    "s.youtube.com",
    "storage.googleapis.com",
    "adl.google.com",
    "zrtp.google.com",
    "firebaseinstallations.googleapis.com",
    "firebaselogging.googleapis.com",
    "firebaserealtime.googleapis.com",
    "gcm.googleapis.com",
    "fcm.googleapis.com",
    "firebasehosting.googleapis.com",
    "firebase-storage.googleapis.com",
    "firebasestorage.googleapis.com",
    "firebasehosting.googleapis.com",
    "firebaseappdistribution.googleapis.com",
    "applovin.com",
    "applovin.sdk",
    "ads.applovin.com",
    "ads-by-applovin.appspot.com",
    "app.appsflyer.com",
    "api.branch.io",
    "branch.io"
};

bool shouldBypassDomain(const string &domain) {
  return BLOCKED_DOMAINS.find(domain) != BLOCKED_DOMAINS.end();
}

static std::map<std::string, std::map<int, std::string>> weapon_skins = {
    {"M4A1",
     {
         {11199015, "Black Dragon EVO"},
         {11199016, "Gold Dragon EVO"},
         {11199017, "Red Dragon EVO"},
         {11199018, "Green Dragon EVO"},
         {11199019, "Ice Dragon EVO"},
         {11199020, "White Dragon EVO"},
         {11100018, "Neo Core"},
         {11100020, "Matrix Prime"},
     }},

    {"SCAR",
     {{231100022, "Default"},
      {231100021, "Deadshot Classic"},
      {231100027, "Fang of Death"},
      {231100029, "Cursed Skull"}}},

    {"AK-47",
     {{101199002, "AOT Evo Blue"},
      {101100022, "Quartz EVO"},
      {101100030, "AOT Evo Red"},
      {101199010, "Red Evo Mamba"},
      {101199011, "Blue Evo Mamba"}}},

    {"KAG6",
     {{141100011, "Default"},
      {141100012, "Chromatic Pulse"},
      {141100013, "Neon Stripes"},
      {141100014, "Golden Flare"}}},

    {"VSS",
     {{251100016, "Default"},
      {251100017, "Classic"},
      {251100018, "Tiger"},
      {251100019, "Gold"},
      {251100015, "Silver"},
      {251100014, "Neon"},
      {251100013, "Dragon"},
      {251100012, "Carbon"},
      {251100011, "Rainbow"}}},

    {"AR97",
     {{271199002, "Default"},
      {270100085, "Classic"},
      {270100144, "Tiger"},
      {271199001, "Gold"}}},

    {"AUG",
     {{291199012, "Default"},
      {291100010, "Classic"},
      {291199007, "Dragon"},
      {291100005, "Tiger"},
      {2911990091, "Gold Variant 1"},
      {291199011, "Silver"},
      {291100009, "Neon"},
      {291199006, "Carbon"},
      {291100004, "Rainbow"},
      {291199001, "Camo"},
      {2911990095, "Gold Variant 2"},
      {291100008, "Wood"},
      {291199005, "Digital"},
      {291100003, "Blood"},
      {2911990094, "Gold Variant 3"},
      {291199014, "Ice"},
      {291199009, "Lava"},
      {291100007, "Sand"},
      {291199004, "Emerald"},
      {291100002, "Ruby"},
      {2911990093, "Gold Variant 4"},
      {291199013, "Sapphire"},
      {291100011, "Amber"},
      {291199008, "Pearl"},
      {291100006, "Onyx"},
      {291199003, "Platinum"},
      {291100001, "Titanium"},
      {2911990092, "Gold Variant 5"}}},

    {"MCX",
     {{311199005, "Default"},
      {311199014, "Classic"},
      {311199009, "Dragon"},
      {311199004, "Tiger"},
      {311199013, "Gold"},
      {311199008, "Silver"},
      {311199003, "Neon"},
      {311100001, "Carbon"},
      {311199012, "Rainbow"},
      {311199007, "Camo"},
      {311199002, "Wood"},
      {311199016, "Digital"},
      {311199011, "Blood"},
      {311199006, "Ice"},
      {311199001, "Lava"},
      {311199015, "Sand"},
      {311199010, "Emerald"}}},

    {"FAL",
     {{321199016, "Default"},
      {321199011, "Classic"},
      {321199006, "Dragon"},
      {321199001, "Tiger"},
      {321199015, "Gold"},
      {321199010, "Silver"},
      {321199005, "Neon"},
      {321199014, "Carbon"},
      {321199009, "Rainbow"},
      {321199004, "Camo"},
      {321199018, "Wood"},
      {321199013, "Digital"},
      {321199008, "Blood"},
      {321199003, "Ice"},
      {321100001, "Lava"},
      {321199017, "Sand"},
      {321199012, "Emerald"},
      {321199007, "Ruby"},
      {321199002, "Sapphire"}}},

    {"FN2000",
     {{371199003, "Default"},
      {371199002, "Classic"},
      {371199006, "Dragon"},
      {371199001, "Tiger"},
      {371199005, "Gold"},
      {371199004, "Silver"}}},

    {"GALIL", {{381199005, "Default"},   {3811990035, "Classic"},
               {381199009, "Dragon"},    {381199004, "Tiger"},
               {3811990034, "Gold"},     {381199008, "Silver"},
               {381199003, "Neon"},      {3811990063, "Carbon"},
               {3811990033, "Rainbow"},  {381199012, "Camo"},
               {381199007, "Wood"},      {381199002, "Digital"},
               {3811990062, "Blood"},    {3811990032, "Ice"},
               {381199011, "Lava"},      {381199006, "Sand"},
               {381199001, "Emerald"},   {3811990061, "Ruby"},
               {3811990031, "Sapphire"}, {381199010, "Amber"}}},

    {"QBZ95",
     {{3911990124, "Default"}, {3911990154, "Classic"}, {391199012, "Dragon"}}},

    {"M700",
     {{181100020, "Default"},
      {181100019, "Classic"},
      {181100018, "Dragon"},
      {181100017, "Tiger"},
      {181100016, "Gold"},
      {181100015, "Silver"}}},

    {"KATANA",
     {
         {21000008, "Golden Fang"},   
         {31000007, "Crimson Vow"},   
         {31000004, "Azure Strike"},  
         {31000010, "Spectral"},      
         {21000007, "Mythic Edge"}    
     }},

    {"MELEE",
     {
         {11000008, "Bloodfang"},   
         {11000011, "Dragon Fang"}  
     }},

    {"KAR98",
     {{331100003, "Default"},
      {331199012, "Classic"},
      {331199007, "Dragon"},
      {331199002, "Tiger"},
      {331199011, "Gold"},
      {331199006, "Silver"},
      {331199001, "Neon"},
      {331199010, "Carbon"},
      {331199005, "Rainbow"},
      {331199009, "Camo"},
      {331199004, "Wood"},
      {331100002, "Digital"},
      {331199008, "Blood"},
      {331199003, "Ice"},
      {331100001, "Lava"}}},

    {"KALA", {{1311990074, "Default"}, {131100012, "Classic"},
              {131199009, "Dragon"},   {131100007, "Tiger"},
              {131199004, "Gold"},     {131100002, "Silver"},
              {1311990073, "Neon"},    {131100011, "Carbon"},
              {131199008, "Rainbow"},  {131100006, "Camo"},
              {131199003, "Wood"},     {131100001, "Digital"},
              {131100010, "Blood"},    {131100005, "Ice"},
              {131199002, "Lava"},     {1311990071, "Sand"},
              {131100014, "Emerald"},  {131100009, "Ruby"},
              {131199006, "Sapphire"}, {131100004, "Amber"},
              {1311990075, "Pearl"},   {1311990070, "Onyx"},
              {131100013, "Platinum"}, {131100008, "Titanium"},
              {131199005, "Copper"},   {131100003, "Bronze"},
              {1311990072, "Diamond"}}},

    {"SVD",
     {{401199002, "Default"},
      {401199006, "Classic"},
      {401199001, "Dragon"},
      {401199005, "Tiger"},
      {401199004, "Gold"},
      {401199003, "Silver"}}},

    {"BOW",
     {{221100005, "Default"},
      {221100009, "Classic"},
      {221100004, "Dragon"},
      {221100008, "Tiger"},
      {221100003, "Gold"},
      {221100007, "Silver"},
      {221100002, "Neon"},
      {221100006, "Carbon"},
      {221100001, "Rainbow"}}},

    {"ORIGIN12",
     {{61100017, "Default"},
      {61100012, "Classic"},
      {61100007, "Dragon"},
      {61199004, "Tiger"},
      {61100002, "Gold"},
      {61100016, "Silver"},
      {61100011, "Neon"},
      {61100006, "Carbon"},
      {61199003, "Rainbow"},
      {61100001, "Camo"},
      {61100015, "Wood"},
      {61100010, "Digital"},
      {61100005, "Blood"},
      {61199002, "Ice"}}},

    {"MP155",
     {{201100012, "Default"},
      {201100007, "Classic"},
      {201199004, "Dragon"},
      {201100002, "Tiger"},
      {201100011, "Gold"},
      {201100006, "Silver"},
      {201199003, "Neon"},
      {201100001, "Carbon"},
      {201100010, "Rainbow"}}},

    {"M1887", {{421199001, "Default"}}},

    {"MP5",
     {{21100011, "Default"},
      {21100009, "Classic"},
      {21100008, "Dragon"},
      {21100007, "Tiger"},
      {21100006, "Gold"},
      {21100005, "Silver"},
      {21100004, "Neon"},
      {21100003, "Carbon"},
      {21100002, "Rainbow"},
      {21100000, "Camo"}}},

    {"VECTOR", {{81199007, "Black Dragon"}, {81199009, "Ice Dragon"}}},

    {"URB",
     {{161100011, "Default"},  {161199008, "Classic"}, {161100006, "Dragon"},
      {161199003, "Tiger"},    {161100001, "Gold"},    {161100015, "Silver"},
      {161100010, "Neon"},     {161199007, "Carbon"},  {161100005, "Rainbow"},
      {161199002, "Camo"},     {161100014, "Wood"},    {161100009, "Digital"},
      {161199006, "Blood"},    {161100004, "Ice"},     {161199001, "Lava"},
      {161100013, "Sand"},     {161100008, "Emerald"}, {161199005, "Ruby"},
      {161100003, "Sapphire"}, {161100012, "Amber"},   {161100007, "Pearl"},
      {161199004, "Onyx"},     {161100002, "Platinum"}}},

    {"INP9",
     {{171100013, "Default"},  {171199010, "Classic"},  {171100008, "Dragon"},
      {171199005, "Tiger"},    {171100003, "Gold"},     {171199014, "Silver"},
      {171100012, "Neon"},     {171199004, "Carbon"},   {171100002, "Rainbow"},
      {171199013, "Camo"},     {171100011, "Wood"},     {171100006, "Digital"},
      {171199003, "Blood"},    {171100001, "Ice"},      {171100015, "Lava"},
      {171199012, "Sand"},     {171100010, "Emerald"},  {171100005, "Ruby"},
      {171199002, "Sapphire"}, {171100014, "Amber"},    {171199011, "Pearl"},
      {171100009, "Onyx"},     {171199006, "Platinum"}, {171100004, "Titanium"},
      {171199001, "Diamond"}}},

    {"P90",
     {{241100012, "Default"}, {241100011, "Classic"}, {241100013, "Dragon"}}},

    {"UZI",
     {{351199006, "Default"},
      {351199005, "Classic"},
      {3511990132, "Red Scorpio"},
      {351199012, "Blue Scorpio"}}},
    {"MAGNUM",
     {{301199003, "Default"},
      {301199007, "Classic"},
      {301199002, "Dragon"},
      {301199006, "Tiger"},
      {301199001, "Gold"},
      {301199005, "Silver"},
      {301199004, "Neon"}}},

    {"GLOCK",
     {{41100014, "Default"},
      {41100009, "Classic"},
      {41100004, "Dragon"},
      {41199001, "Tiger"},
      {41100013, "Gold"},
      {41100008, "Silver"},
      {41100003, "Neon"},
      {41100012, "Carbon"},
      {41100007, "Rainbow"},
      {41199004, "Camo"},
      {41100002, "Wood"},
      {41199002, "Digital"},
      {41100005, "Blood"},
      {41100010, "Ice"},
      {41100015, "Lava"}}},

    {"DEAGLE",
     {{151100010, "Default"},
      {151100005, "Classic"},
      {151199002, "Dragon"},
      {151100009, "Tiger"},
      {151100004, "Gold"},
      {151199001, "Silver"},
      {151100013, "Neon"},
      {151100008, "Carbon"},
      {151199005, "Rainbow"},
      {151100003, "Camo"},
      {151100012, "Wood"},
      {151100007, "Digital"},
      {151199004, "Blood"},
      {151100002, "Ice"},
      {151100011, "Lava"},
      {151100006, "Sand"},
      {151199003, "Emerald"},
      {151100001, "Ruby"}}}};

void bypassBlockedDomains() {
  for (int i = 0; i < 100; i++) {
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

bool Zaijan = true;
struct { MemoryPatch Feature1; } var;

void 绘制字体描边(float size, int x, int y, ImVec4 color, const char *str) {
  ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
  ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
  ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
  ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
  ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

bool isVPN() {
  char command[256] = "";
  memset(command, 0, 256);
  if ((access(oxorany("/system/bin/ifconfig"), F_OK)) != -1) sprintf(command, "%s", oxorany("/system/bin/ifconfig"));
  else return true;
  FILE *fp = popen(command, oxorany("r"));
  char line[1024] = "";
  while (fgets(line, 1024, fp) != NULL) {
    if (strstr(line, oxorany("tun0")) != NULL || strstr(line, oxorany("ppppp0")) != NULL) {
      pclose(fp);
      return true;
    }
  }
  pclose(fp);
  return false;
}

// ========================================================================================================================
// ESP RENDERER 
// ========================================================================================================================

void visiblePlayerss(ImDrawList *draw) {
    if (!sync_bool["bESP"]) return;
    auto entities = get_entities_all_ref();
    int visible = 0;
    for (const Entity &e : *entities) {
        if (!e.is_bot && e.is_visible) visible++;
    }
    std::string playerVisible = "";
    if (visible == 0) {
        playerVisible = " NO PLAYER CAN SEE YOU ";
    } else {
        playerVisible = std::to_string(visible) + " PLAYER CAN SEE YOU ";
    }
    float fontSize = 25.0f;
    float centerX = Width / 2.0f;
    ImVec2 visSize = ImGui::CalcTextSize2(playerVisible.c_str(), 0, fontSize);
    float visX = centerX - (visSize.x / 2.0f);
    draw->AddRectFilled(ImVec2(visX - 15.0f, 105.0f), ImVec2(visX + visSize.x + 15.0f, 135.0f), 0x90000000, 5, 0);
    ImColor visColor = (visible > 0) ? ImColor(255, 50, 50) : ImColor(255, 255, 255);
    draw->AddText(NULL, fontSize, ImVec2(visX, 107.0f), visColor, playerVisible.c_str());
}

void noMore(ImDrawList *draw) {
  if (!sync_bool["bESP"]) return;
  auto entities = get_entities_all_ref();
  float espFontSize = ImGui::GetFontSize() * 0.65f; 
  int players = 0, bots = 0;
  
  for (const Entity &e : *entities) {
    if (e.is_bot) bots++;
    else players++;
    if (!e.is_on_screen) continue;
    ImVec2 From;
    switch (sync_int["iESP_Point"]) {
      case 0: From = ImVec2(Width / 2, Height); break;
      case 1: From = ImVec2(Width / 2, Height / 2); break;
      case 2: From = ImVec2(Width / 2, 0); break;
    }

    if (sync_bool["bESP_Skeleton"]) {
      auto head = ToImVec2(e.bones.at("head"));
      auto spine = ToImVec2(e.bones.at("spine"));
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
      ImVec2 chest((limbs_r_upperarm.x + limbs_l_upperarm.x) / 2, (limbs_r_upperarm.y + limbs_l_upperarm.y) / 2);
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
    }

    ImVec2 To(ToImVec2(e.bones.at("head")));

    if (e.is_bot) {
      if (sync_bool["bESP_LineBots"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
    } else {
      if (sync_bool["bESP_Line"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
    }

    if (sync_bool["bAIM_SnapLine"]) {
        ImVec2 screenCenter = ImVec2(Width / 2.f, Height / 2.f);
        float aimFov = sync_float["fAIM_Fov"]; 
        float dx = To.x - screenCenter.x;
        float dy = To.y - screenCenter.y;
        float distanceToCrosshair = sqrtf(dx * dx + dy * dy);
        if (e.is_visible && distanceToCrosshair <= aimFov) {
            ImU32 aimColor = IM_COL32(255, 255, 0, 255); 
            draw->AddLine(screenCenter, To, aimColor, sync_float["fESP_LineThickness"]);
        }
    }

    if ((sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"] || sync_bool["bESP_Distance"]) && !e.bones.empty()) {
        ImVec2 head = ToImVec2(e.bones.at("head"));
        ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
        ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
        ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

        float height = feet.y - head.y;
        float width = height * 0.6f;
        ImVec2 boxMin(head.x - width / 2, head.y);
        ImVec2 boxMax(head.x + width / 2, feet.y);
        ImColor fillColor = e.is_visible ? ImColor(0, 255, 0, 90) : ImColor(0, 255, 0, 40);
        draw->AddRectFilled(boxMin, boxMax, fillColor);

        float distance = 200.0f / height;
        const char* tag = e.is_knocked ? "KNOCK" : (e.is_bot ? "BOT" : "PLAYER");
        std::string teamText = std::to_string((int)e.team_id);
        std::string distanceText = std::to_string((int)distance) + "m";

        ImVec2 teamSize = ImGui::CalcTextSize(teamText.c_str());
        ImVec2 tagSize = ImGui::CalcTextSize(tag);
        ImVec2 distSize = ImGui::CalcTextSize(distanceText.c_str());

        float padding = 6.0f;
        float spacing = 10.0f;
        float totalWidth = padding;
        if (sync_bool["bESP_TeamID"])    totalWidth += teamSize.x + spacing;
        if (sync_bool["bESP_Name"])      totalWidth += tagSize.x + spacing;
        if (sync_bool["bESP_Distance"])  totalWidth += distSize.x;
        totalWidth += padding;

        float boxHeight = std::max({ teamSize.y, tagSize.y, distSize.y }) + 4;
        ImVec2 boxPos = ImVec2(head.x - totalWidth / 2, head.y - boxHeight - 10);
        ImVec2 boxEnd = ImVec2(boxPos.x + totalWidth, boxPos.y + boxHeight);

        draw->AddRectFilled(boxPos, boxEnd, IM_COL32(0, 0, 0, 180), 3.0f);
        draw->AddRect(boxPos, boxEnd, IM_COL32(255, 255, 255, 40), 3.0f);

        float cursorX = boxPos.x + padding;
        if (sync_bool["bESP_TeamID"]) {
            draw->AddText(ImVec2(cursorX, boxPos.y + 2), IM_COL32(255, 255, 255, 255), teamText.c_str());
            cursorX += teamSize.x + spacing;
        }
        if (sync_bool["bESP_Name"]) {
            ImU32 nameColor = e.is_knocked ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
            draw->AddText(ImVec2(cursorX, boxPos.y + 2), nameColor, tag);
            cursorX += tagSize.x + spacing;
        }
        if (sync_bool["bESP_Distance"]) {
            draw->AddText(ImVec2(cursorX, boxPos.y + 2), IM_COL32(0, 255, 0, 255), distanceText.c_str());
        }
    }

    if (sync_bool["bESP_Health"] && !e.bones.empty()) {
        ImVec2 head = ToImVec2(e.bones.at("head"));
        ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
        ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
        ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

        float height = feet.y - head.y;
        float width = height * 0.6f;
        ImVec2 boxMin = ImVec2(head.x - width / 2, head.y);
        
        float health = (e.is_health < 0.0f) ? 0.0f : (e.is_health > 100.0f) ? 100.0f : e.is_health;
        float hpPercent = health / 100.0f;
        float barW = 4.0f;
        ImVec2 barTop = ImVec2(boxMin.x - 6.0f, boxMin.y);
        ImVec2 barBottom = ImVec2(barTop.x + barW, boxMin.y + height);
        ImVec2 fillTop = ImVec2(barTop.x, barTop.y + height * (1.0f - hpPercent));
        ImVec2 fillBottom = ImVec2(barBottom.x, barBottom.y);
        ImU32 hpColor = IM_COL32((1.0f - hpPercent) * 255, hpPercent * 255, 0, 255);
        draw->AddRectFilled(fillTop, fillBottom, hpColor);
        draw->AddRect(barTop, barBottom, IM_COL32(0, 0, 0, 255));
    }

    if (sync_bool["bESP_Box"] && !e.bones.empty()) {
        ImVec2 head = ToImVec2(e.bones.at("head"));
        auto foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
        auto foot_r = ToImVec2(e.bones.at("limbs_r_calf"));

        ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;
        float height = feet.y - head.y;
        float width = height * 0.6f;

        ImVec2 boxMin(head.x - width / 2, head.y);
        ImVec2 boxMax(head.x + width / 2, feet.y);

        ImColor fillColor = e.is_visible ? ImColor(128, 0, 128, 90) : ImColor(128, 0, 128, 40);
        draw->AddRectFilled(boxMin, boxMax, fillColor);
    }

    if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
      draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
      draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
    }
  }

  std::string yan = "";
  if (players + bots == 0) {
      yan = " SAFE ";
  } else {
      yan = "PLAYER : " + std::to_string(players) + "  BOT : " + std::to_string(bots);
  }

  float fontSize = 25.0f;
  float centerX = Width / 2.0f;
  ImVec2 yanSize = ImGui::CalcTextSize2(yan.c_str(), 0, fontSize);
  float yanX = centerX - (yanSize.x / 2.0f);
  
  draw->AddRectFilled(ImVec2(yanX - 15.0f, 70.0f), ImVec2(yanX + yanSize.x + 15.0f, 100.0f), 0x90000000, 5, 0);
  draw->AddText(NULL, fontSize, ImVec2(yanX, 72.0f), ImColor(255, 255, 255), yan.c_str());

  if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
    if (sync_bool["bAIM_DrawFov"])
      draw->AddCircle(ImVec2(Width / 2, Height / 2), sync_float["fAIM_Fov"], WHITE, 0, sync_float["fESP_LineThickness"]);
  }
}

// ========================================================================================================================
// START OF MAIN MENU (ALL FEATURES RESTORED)
// ========================================================================================================================
void DrawMenu(ImGuiIO &io) {
    static bool showMenu = false; 
    bool need_sync = false, tab_changed = false;
    sync_float["fHeight"] = Height;
    sync_float["fWidth"] = Width;
    static int tab = 1;

    ImVec4 themeColor = ImVec4(0.95f, 0.20f, 0.20f, 1.0f); 
    ImVec4 accentColor = ImVec4(1.0f, 0.80f, 0.10f, 1.0f); 

    // ── helper: draw the SacRedBS shield logo at any position/size ──────────
    auto DrawShieldLogo = [](ImDrawList* ld, ImVec2 center, float sz, float t) {
        float hw = sz * 0.48f, hh = sz * 0.54f;
        // Shield polygon (5-point)
        ImVec2 shield[5] = {
            ImVec2(center.x - hw, center.y - hh),
            ImVec2(center.x + hw, center.y - hh),
            ImVec2(center.x + hw, center.y + hh * 0.25f),
            ImVec2(center.x,      center.y + hh),
            ImVec2(center.x - hw, center.y + hh * 0.25f),
        };
        // Fill
        ld->AddConvexPolyFilled(shield, 5, IM_COL32(140, 8, 8, 255));
        // Inner shine highlight
        float shx = hw * 0.85f, shy = hh * 0.85f;
        ImVec2 inner[5] = {
            ImVec2(center.x - shx, center.y - shy),
            ImVec2(center.x + shx, center.y - shy),
            ImVec2(center.x + shx, center.y + shy * 0.25f),
            ImVec2(center.x,       center.y + shy),
            ImVec2(center.x - shx, center.y + shy * 0.25f),
        };
        ld->AddConvexPolyFilled(inner, 5, IM_COL32(180, 12, 12, 180));
        // Animated gold border
        int ga = (int)(180 + 75 * fabsf(sinf(t * 2.2f)));
        ld->AddPolyline(shield, 5, IM_COL32(255, 200, 0, ga), ImDrawFlags_Closed, 2.2f);
        // Inner stripe (shine)
        ld->AddRectFilled(
            ImVec2(center.x - hw * 0.7f, center.y - hh * 0.88f),
            ImVec2(center.x + hw * 0.7f, center.y - hh * 0.60f),
            IM_COL32(255, 255, 255, 18), 2.0f);
        // Draw "S" as pure geometry – 5 bars
        float bw = sz * 0.32f, bh = sz * 0.07f, bc = sz * 0.08f;
        float sx = center.x - bw * 0.5f, sy = center.y - (bh*3 + bc*2) * 0.5f - sz * 0.04f;
        ImU32 gc = IM_COL32(255, 215, 0, 255);
        // top bar
        ld->AddRectFilled(ImVec2(sx, sy),               ImVec2(sx+bw, sy+bh),           gc, 2.0f);
        // top-left connector
        ld->AddRectFilled(ImVec2(sx, sy+bh),            ImVec2(sx+bh, sy+bh+bc),        gc);
        // middle bar
        ld->AddRectFilled(ImVec2(sx, sy+bh+bc),         ImVec2(sx+bw, sy+bh*2+bc),      gc, 2.0f);
        // bottom-right connector
        ld->AddRectFilled(ImVec2(sx+bw-bh, sy+bh*2+bc),ImVec2(sx+bw, sy+bh*2+bc+bc),   gc);
        // bottom bar
        ld->AddRectFilled(ImVec2(sx, sy+bh*2+bc*2),    ImVec2(sx+bw, sy+bh*3+bc*2),     gc, 2.0f);
        // corner dots
        ld->AddCircleFilled(ImVec2(center.x - hw + 5, center.y - hh + 5), 3.0f, IM_COL32(255,200,0,160));
        ld->AddCircleFilled(ImVec2(center.x + hw - 5, center.y - hh + 5), 3.0f, IM_COL32(255,200,0,160));
    };

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

            // outer glow ring
            int ga = (int)(140 + 80 * fabsf(sinf(t * 2.0f)));
            bdl->AddCircle(ImVec2(cx, cy), 42.0f, IM_COL32(200, 20, 20, ga), 32, 3.0f);
            bdl->AddCircleFilled(ImVec2(cx, cy), 38.0f, IM_COL32(12, 4, 4, 245));

            DrawShieldLogo(bdl, ImVec2(cx, cy - 2), 52.0f, t);

            // invisible drag + click area
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
        return;
    }

    // ========================================================================
    // KEY SYSTEM AUTHENTICATION BLOCK (MULTISPACE FULLY FIXED)
    // ========================================================================
    
    // [ANTI-CRACK]: Compare atomic state instead of checking a simple boolean
    if (auth_state.load() != AUTH_SUCCESS) {
        if (cached_uuid.empty()) {
            cached_uuid = GetCleanUUID();
        }
        std::string link = GetKeyGeneratorLink(cached_uuid);

        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 420), ImVec2(io.DisplaySize.x * 0.95f, io.DisplaySize.y * 0.9f));
        ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_FirstUseEver);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.06f, 0.04f, 0.04f, 0.98f));
        ImGui::PushStyleColor(ImGuiCol_Border,        ImVec4(0.70f, 0.06f, 0.06f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,       ImVec4(0.12f, 0.07f, 0.07f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.08f, 0.08f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.05f, 0.05f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark,     ImVec4(0.95f, 0.20f, 0.20f, 1.0f));
        if (ImGui::Begin("##KeySystem", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
            
            float windowWidth = ImGui::GetWindowSize().x;
            
            // Title
            ImGui::SetWindowFontScale(1.1f);
            const char* title = oxorany("SACREDBS VIP - LOGIN");
            ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(title).x) * 0.5f);
            ImGui::TextColored(themeColor, "%s", title);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Separator();
            ImGui::Spacing();

            // Device info
            ImGui::TextDisabled(oxorany("DEVICE:"));
            ImGui::SameLine();
            ImGui::Text("%s", getDeviceName().c_str());
            ImGui::TextDisabled(oxorany("HWID:"));
            ImGui::SameLine();
            ImGui::Text("%s", cached_uuid.c_str());

            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            ImGui::TextColored(themeColor, oxorany("STEP 1: GET YOUR FREE 1DAY KEY"));

            float halfW = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
            if (ImGui::Button(oxorany("OPEN IN BROWSER"), ImVec2(halfW, 45))) {
                OpenBrowserWithUrl(link);
                snprintf(auth_status, sizeof(auth_status), oxorany("Opening browser..."));
            }
            ImGui::SameLine();
            if (ImGui::Button(oxorany("COPY LINK"), ImVec2(halfW, 45))) {
                CopyLinkToClipboard(link);
                snprintf(auth_status, sizeof(auth_status), oxorany("Link Copied! Paste in browser."));
            }

            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            ImGui::TextColored(themeColor, oxorany("STEP 2: LOGIN"));

            ImGui::PushItemWidth(-1);
            if (ImGui::InputTextWithHint("##keyinput", oxorany("click PASTE..."), user_input_key, IM_ARRAYSIZE(user_input_key))) {
                ShowSoftKeyboardInput();
            }
            ImGui::PopItemWidth();

            ImGui::Spacing();
            
            if (ImGui::Button(oxorany("PASTE KEY FROM CLIPBOARD"), ImVec2(-1, 40))) {
                std::string clip = getClipboard(); 
                if (!clip.empty()) {
                    strncpy(user_input_key, clip.c_str(), IM_ARRAYSIZE(user_input_key) - 1);
                    snprintf(auth_status, sizeof(auth_status), oxorany("Key pasted!"));
                } else {
                    snprintf(auth_status, sizeof(auth_status), oxorany("Clipboard is empty!"));
                }
            }

            ImGui::Spacing();
            
            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.70f, 0.06f, 0.06f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1.0f,  1.0f,  1.0f,  1.0f));
            
            if (is_authenticating) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                ImGui::Button(oxorany("VERIFYING..."), ImVec2(-1, 48));
                ImGui::PopStyleVar();
            } else {
                if (ImGui::Button(oxorany("LOGIN"), ImVec2(-1, 48))) {
                    is_authenticating = true;
                    snprintf(auth_status, sizeof(auth_status), oxorany("Connecting to server..."));

                    std::string temp_key(user_input_key);
                    std::string temp_hwid(cached_uuid);

                    std::thread([temp_key, temp_hwid]() {
                        JNIEnv *env;
                        int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
                        bool did_attach = false;
                        if (getEnvStat == JNI_EDETACHED) {
                            if (jvm->AttachCurrentThread(&env, NULL) == JNI_OK) {
                                did_attach = true;
                            }
                        }

                        VerifyKeyWithServer(temp_key, temp_hwid);
                        is_authenticating = false; 

                        if (did_attach) {
                            jvm->DetachCurrentThread();
                        }
                    }).detach();
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::Spacing();
            float statusWidth = ImGui::CalcTextSize(auth_status).x;
            ImGui::SetCursorPosX((windowWidth - statusWidth) * 0.5f);
            bool isFail = strstr(auth_status, "Failed") || strstr(auth_status, "Error") || strstr(auth_status, "expired") || strstr(auth_status, "device");
            ImGui::TextColored(isFail ? ImVec4(1.0f,0.2f,0.2f,1.0f) : ImVec4(0.3f,1.0f,0.3f,1.0f), "%s", auth_status);
        }
        ImGui::End();
        ImGui::PopStyleColor(6);
        ImGui::PopStyleVar(3);
        return; 
    }

    float maxWidth = io.DisplaySize.x * 0.95f;
    float maxHeight = io.DisplaySize.y * 0.95f;
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(maxWidth, maxHeight));
    ImGui::SetNextWindowSize(ImVec2(700, 440), ImGuiCond_FirstUseEver);

    ImGuiStyle& style = GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 12.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg,    ImVec4(0.06f, 0.04f, 0.04f, 0.98f));
    ImGui::PushStyleColor(ImGuiCol_Border,      ImVec4(0.70f, 0.06f, 0.06f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.04f, 0.02f, 0.02f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab,       ImVec4(0.55f, 0.05f, 0.05f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,ImVec4(0.75f, 0.10f, 0.10f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.90f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Separator,   ImVec4(0.50f, 0.04f, 0.04f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg,     ImVec4(0.10f, 0.06f, 0.06f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_Button,      ImVec4(0.14f, 0.08f, 0.08f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.40f, 0.05f, 0.05f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Header,      ImVec4(0.50f, 0.04f, 0.04f, 0.60f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark,   ImVec4(0.95f, 0.20f, 0.20f, 1.0f));

    if (ImGui::Begin("##SACREDBS_MENU", &showMenu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse)) {

        float ww    = ImGui::GetWindowSize().x;
        float t_now = ImGui::GetTime();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // ── HEADER BACKGROUND ──────────────────────────────────────────
        float HDR_H  = 56.0f;
        float logoSz = 44.0f;
        ImVec2 hdrMin = ImVec2(winPos.x + 1, winPos.y + 1);
        ImVec2 hdrMax = ImVec2(winPos.x + ww - 1, winPos.y + HDR_H);
        dl->AddRectFilled(hdrMin, hdrMax, IM_COL32(12, 4, 4, 255), 8.0f, ImDrawFlags_RoundCornersTop);
        dl->AddLine(ImVec2(hdrMin.x, hdrMax.y), ImVec2(hdrMax.x, hdrMax.y), IM_COL32(180, 120, 0, 180), 1.5f);
        dl->AddRect(hdrMin, hdrMax, IM_COL32(150, 12, 12, 160), 8.0f, ImDrawFlags_RoundCornersTop, 1.2f);

        // ── SHIELD LOGO left ─────────────────────────────────────────────
        DrawShieldLogo(dl, ImVec2(winPos.x + 8 + logoSz * 0.5f, winPos.y + HDR_H * 0.5f), logoSz, t_now);

        // ── DRAG HANDLE (InvisibleButton spanning header between logo and close btn) ───
        // Title is drawn via DrawList so it doesn't consume touch input — only the
        // InvisibleButton below does, making the entire header area draggable.
        ImGui::SetWindowFontScale(0.90f);
        {
            float dragX  = 8 + logoSz + 6;
            float closeW = 68.0f; // close button area reserved on the right
            float dragW  = ww - dragX - closeW - ImGui::GetStyle().ItemSpacing.x;
            ImGui::SetCursorPos(ImVec2(dragX, 0));
            ImGui::InvisibleButton("##hdr_drag", ImVec2(dragW, HDR_H));
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                ImVec2 delta = ImGui::GetIO().MouseDelta;
                ImGui::SetWindowPos(ImVec2(winPos.x + delta.x, winPos.y + delta.y));
            }
            // Draw title text ON the DrawList so it's purely visual (no input capture)
            ImGui::SetWindowFontScale(0.90f);
            float fh = ImGui::GetFontSize();
            int pr = (int)(220 + 35 * sinf(t_now * 2.5f));
            int pg = (int)(150 * fabsf(sinf(t_now * 1.3f)));
            dl->AddText(ImVec2(winPos.x + dragX, winPos.y + (HDR_H - fh) * 0.5f),
                        IM_COL32(pr, pg, 10, 255), oxorany("SACREDBS VIP  :::  drag to move"));
        }

        // ── CLOSE BUTTON (top-right, pure ImGui Button) ──────────────────
        ImGui::SetWindowFontScale(0.70f);
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.60f, 0.04f, 0.04f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.35f, 0.02f, 0.02f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1.0f,  1.0f,  1.0f,  1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);
        float btnH = HDR_H - 16.0f;
        ImGui::SetCursorPos(ImVec2(ww - 68, (HDR_H - btnH) * 0.5f));
        if (ImGui::Button(oxorany(" X "), ImVec2(56, btnH))) showMenu = false;
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(4);

        // ── NAV TABS ────────────────────────────────────────────────────
        ImGui::SetWindowFontScale(1.0f);
        ImGui::SetCursorPosY(HDR_H + 4);
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

        ImGui::SetWindowFontScale(0.80f); 
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float btnWidth = (totalWidth - (style.ItemSpacing.x * 4)) / 5.0f; 
        
        auto NavBtn = [&](const char* label, int id) {
            bool is_active = (tab == id);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
            if (is_active) {
                ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.75f, 0.05f, 0.05f, 0.90f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1.0f,  0.85f, 0.10f, 1.0f));
            }
            if (ImGui::Button(label, ImVec2(btnWidth, 38))) { tab = id; tab_changed = true; }
            if (is_active) ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
        };

        NavBtn(oxorany("VISUAL"), 1); ImGui::SameLine();
        NavBtn(oxorany("COMBAT"), 2); ImGui::SameLine();
        NavBtn(oxorany("MEMORY"), 3); ImGui::SameLine();
        NavBtn(oxorany("UI"), 4); ImGui::SameLine(); 
        NavBtn(oxorany("INFO"), 5);

        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

        ImGui::BeginChild("##MainContent", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        
        if (tab == 1) { 
       
            ImGui::Spacing();

            if (ImGui::BeginTable("##V_T", 2, ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("ESP SETTINGS"));
                ImGui::Separator(); ImGui::Spacing();
                need_sync |= Checkbox(oxorany("ENABLE ESP"), &sync_bool["bESP"]);
                need_sync |= Combo("Starting Point", &sync_int["iESP_Point"], espPoint, IM_ARRAYSIZE(espPoint));
                need_sync |= Checkbox(oxorany("LINE"), &sync_bool["bESP_Line"]);
                need_sync |= Checkbox(oxorany("LINE [ BOT ]"), &sync_bool["bESP_LineBots"]);
                need_sync |= Checkbox(oxorany("BULLET LINE"), &sync_bool["bDrawBulletLine"]);
                need_sync |= Checkbox(oxorany("SKELETON"), &sync_bool["bESP_Skeleton"]);
                need_sync |= Checkbox(oxorany("BOX"), &sync_bool["bESP_Box"]);       
                need_sync |= Checkbox(oxorany("DISTANCE"), &sync_bool["bESP_Distance"]);             
                need_sync |= Checkbox(oxorany("HEALTH"), &sync_bool["bESP_Health"]);             
                need_sync |= Checkbox(oxorany("NAME"), &sync_bool["bESP_Name"]);
                need_sync |= Checkbox(oxorany("TEAM ID"), &sync_bool["bESP_TeamID"]);  

                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("COLOR CONFIG"));
                ImGui::Separator(); ImGui::Spacing();
                
                auto ColorRow = [&](const char* name, std::string key1, std::string key2) {
                    ImGui::Text("%s", name); ImGui::SameLine(85); 
                    ImGui::PushID(key1.c_str());
                    ImGui::PushItemWidth(30);
                    if (ImGui::ColorEdit4("##vis", &colors[key1].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) need_sync = true;
                    ImGui::PopItemWidth(); ImGui::PopID();
                    ImGui::SameLine();
                    ImGui::PushID(key2.c_str());
                    ImGui::PushItemWidth(30);
                    if (ImGui::ColorEdit4("##hid", &colors[key2].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) need_sync = true;
                    ImGui::PopItemWidth(); ImGui::PopID();
                };

                ColorRow(oxorany("BOT"), "cESP_LineBots", "cESP_LineBotsHidden");
                ColorRow(oxorany("ENEMY "), "cESP_Line", "cESP_LineHidden");
                ColorRow(oxorany("SKEL"), "cESP_Skeleton", "cESP_SkeletonHidden");

                ImGui::EndTable();
            }
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
            need_sync |= SliderFloat(oxorany("Esp Line Size"), &sync_float["fESP_LineThickness"], 1.0f, 10.0f);
            ImGui::PopItemWidth();
        }
            
        if (tab == 2) { 
            if (ImGui::BeginTable("##C_T", 2, ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("AIMBOT SETTINGS"));
                ImGui::Separator(); ImGui::Spacing();
                need_sync |= Checkbox(oxorany("Enable Aimbot"), &sync_bool["bAIM"]);
                need_sync |= Combo("Trigger", &sync_int["iAIM_Trigger"], aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
                need_sync |= Combo("Target Bone", &sync_int["iAIM_TargetBone"], targetBone, IM_ARRAYSIZE(targetBone));
                need_sync |= Checkbox(oxorany("Draw Snapline"), &sync_bool["bAIM_SnapLine"]);
                ImGui::PushItemWidth(120);
                need_sync |= SliderFloat(oxorany("Aim Snap Strength"), &sync_float["fAIM_SnapStrength"], 0.001f, 0.90f);
                ImGui::PopItemWidth();
                ImGui::Spacing(); ImGui::Spacing();
                
                ImGui::TextColored(themeColor, oxorany("TARGET FILTERS"));
                ImGui::Separator(); ImGui::Spacing();
                need_sync |= Checkbox(oxorany("Visible Check"), &sync_bool["bAIM_CheckVisibility"]);
                need_sync |= Checkbox(oxorany("Ignore Bots"), &sync_bool["bAIM_IgnoreBots"]);
                need_sync |= Checkbox(oxorany("Ignore Knocked"), &sync_bool["bAIM_IgnoreKnocked"]);

                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("FOV CONFIG"));
                ImGui::Separator(); ImGui::Spacing();
                need_sync |= Checkbox(oxorany("Draw FOV Circle"), &sync_bool["bAIM_DrawFov"]);
                need_sync |= Checkbox(oxorany("Hide FOV if safe"), &sync_bool["bAIM_NoTargetHideFov"]);
                ImGui::Spacing();
                ImGui::PushItemWidth(120);
                need_sync |= SliderFloat(oxorany("Fov Size"), &sync_float["fAIM_Fov"], 30.0f, 1500.0f);
                ImGui::PopItemWidth();

                ImGui::EndTable();
            }
        }

        if (tab == 3) { 
            if (ImGui::BeginTable("##M_T", 2, ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("WEAPON EXPLOITS"));
                ImGui::Separator(); ImGui::Spacing();
                
                need_sync |= Checkbox("NoRecoil v1", &sync_bool["bFeature1"]);
                if (sync_bool["bFeature1"] && !last_sync_bool["bFeature1"]) {
                    var.Feature1.Modify();
                } else if (!sync_bool["bFeature1"] && last_sync_bool["bFeature1"]) {
                    var.Feature1.Restore();   
                }
                
                need_sync |= Checkbox(oxorany("NoRecoil v2"), &sync_bool["bNoRecoil"]);
                need_sync |= Checkbox(oxorany("No Spread"), &sync_bool["bNoSpread"]);
                need_sync |= Checkbox(oxorany("Fast Switch"), &sync_bool["bSwitch"]);
                need_sync |= Checkbox(oxorany("Unlock Skin"), &sync_bool["bSkinHack"]);

                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("PLAYER EXPLOITS"));
                ImGui::Separator(); ImGui::Spacing();
                need_sync |= Checkbox(oxorany("Wallhack Red"), &sync_bool["bXray"]);
                need_sync |= Checkbox(oxorany("Speed Boost"), &sync_bool["bSpeed"]);
                
                if (sync_bool["bSpeed"]) {
                    ImGui::Spacing();
                    ImGui::PushItemWidth(120);
                    need_sync |= SliderFloat(oxorany("Speed Multiplier"), &sync_float["fSpeed"], 1.0f, 1.5f);
                    ImGui::PopItemWidth();
                }

                ImGui::EndTable();
            }
            last_sync_bool = sync_bool;
        }
        
        if (tab == 4) { 
            if (ImGui::BeginTable("##UI_T", 2, ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("MENU STYLING"));
                ImGui::Separator(); ImGui::Spacing();
                
                static bool style_changed = false;
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
                if (ImGui::Combo(oxorany("Theme"), &current_theme, themes, IM_ARRAYSIZE(themes))) {
                    switch_theme(current_theme);
                    style_changed = true;
                }
                
                if (ImGui::SliderFloat(oxorany("Rounding"), &Rounding, 0.0f, 20.0f)) {
                    style.FrameRounding = Rounding;
                    style.WindowRounding = Rounding;
                    style.ChildRounding = Rounding;
                    style.PopupRounding = Rounding;
                    style.ScrollbarRounding = Rounding;
                    style.GrabRounding = Rounding;
                    style.LogSliderDeadzone = Rounding;
                    style.TabRounding = Rounding;
                    style_changed = true;
                }
                
                style_changed |= ImGui::SliderFloat(oxorany("Opacity"), &style.Alpha, 0.6f, 1.0f);
                style_changed |= ImGui::SliderFloat(oxorany("Border Size"), &style.FrameBorderSize, 0.0f, 5.0f);
                style_changed |= ImGui::SliderFloat(oxorany("Scrollbar Size"), &style.ScrollbarSize, 10.0f, 50.0f);
                ImGui::PopItemWidth();
                if (style_changed) save_imgui_style(&style);

                ImGui::TableNextColumn();
                ImGui::TextColored(themeColor, oxorany("SYSTEM SETTINGS"));
                ImGui::Separator(); ImGui::Spacing();
                bool settings_changed = false;
                settings_changed |= Checkbox("AlwaysAutoResize", &features["bAlwaysAutoResize"]);
                settings_changed |= Checkbox("RememberSettings (restart req)", &features["bImguiAutoSave"]);
                ImGui::PushItemWidth(120);
                settings_changed |= SliderFloat("FontScale (restart req)", &features_float["fFontScale"], 30.0f, 60.0f);
                ImGui::PopItemWidth();
                settings_changed |= Checkbox("Better Touch (May crash)", &_features["bBetterTouch"]);
                if (settings_changed) save_features();
                
                ImGui::EndTable();
            }
        }

        if (tab == 5) { 
            if (ImGui::BeginTable("##I_T", 2, ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableNextColumn();

                // ── VIP STATUS ─────────────────────────────
                ImGui::TextColored(accentColor, oxorany("VIP STATUS"));
                ImGui::Separator(); ImGui::Spacing();
                if (strlen(vip_username_str) > 0) {
                    ImGui::TextDisabled(oxorany("USER:"));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.1f, 1.0f), "%s", vip_username_str);
                }
                ImGui::TextDisabled(oxorany("DAYS REMAINING:"));
                ImGui::SameLine();
                if (vip_days_received) {
                    float dr = (float)vip_days_remaining;
                    ImVec4 daysColor = dr > 7 ? ImVec4(0.3f,1.0f,0.3f,1.0f)
                                     : dr > 2 ? ImVec4(1.0f,0.7f,0.1f,1.0f)
                                     :          ImVec4(1.0f,0.2f,0.2f,1.0f);
                    ImGui::TextColored(daysColor, "%d DAYS", vip_days_remaining);
                } else {
                    ImGui::TextDisabled(oxorany("---"));
                }
                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

                ImGui::TextColored(themeColor, oxorany("ABOUT"));
                ImGui::Separator(); ImGui::Spacing();
                ImGui::Text(oxorany("DEVELOPER : SACREDBS VIP"));
                ImGui::Spacing();
                ImGui::Text(oxorany("OFFICIAL TELEGRAM : T.ME/SACREDBS_VIP"));
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::TextDisabled(oxorany("LAUNCHER VERSION : 1.003.650015"));
                ImGui::Separator(); ImGui::Spacing();
                ImGui::TextColored(themeColor, oxorany("FIX HACK NOT WORKING"));
                ImGui::Separator(); ImGui::Spacing();
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::TextDisabled(oxorany("make sure your bloodstrike version is the same in the version of this launcher"));
                ImGui::TextDisabled(oxorany("if the bloodstrike version and this launcher is same follow the next step"));
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::TextDisabled(oxorany("step 1: go to firing range and check the ENABLE ESP, ESP LINE [ BOT ] "));
                ImGui::TextDisabled(oxorany("step 2: turn off wifi/data then wait for the bloodstrike login screen appears"));
                ImGui::TextDisabled(oxorany("step 3: if the login screen appears then turn on your wifi/data then wait"));
                ImGui::TextDisabled(oxorany("for the bloodstrike to load, you can click whatever in the login screen then "));
                ImGui::TextDisabled(oxorany("when it loads then congrats, the cheat is now working. you can quit the firing range!"));
                ImGui::EndTable();
            }
        }

        if (tab_changed || need_sync) sync_with_py();

        ImGui::EndChild(); 
        ImGui::SetWindowFontScale(1.0f); 
    } 
    ImGui::End(); 
    ImGui::PopStyleColor(12);
    ImGui::PopStyleVar(3);
}

int screenWidth = -1, screenHeight = -1;
static bool setup = false;
int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow *window);
int32_t _ANativeWindow_getWidth(ANativeWindow *window) {
  screenWidth = orig_ANativeWindow_getWidth(window);
  return orig_ANativeWindow_getWidth(window);
}
int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow *window);
int32_t _ANativeWindow_getHeight(ANativeWindow *window) {
  screenHeight = orig_ANativeWindow_getHeight(window);
  return orig_ANativeWindow_getHeight(window);
}

ImVec2 windowPos = ImVec2(50, 50);
ImVec2 touchStart;
bool isDragging = false;
bool windowOpen = true;

ImVec2 SmoothMove(ImVec2 start, ImVec2 end, float speed) {
  return ImVec2(start.x + (end.x - start.x) * speed, start.y + (end.y - start.y) * speed);
}

void *protectFunc(void *addr) {
  size_t pageSize = sysconf(_SC_PAGESIZE);
  void *alignedAddr = (void *)((uintptr_t)addr & ~(pageSize - 1));
  mprotect(alignedAddr, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
  return addr;
}

int32_t (*orig_AInputQueue_getEvent)(AInputQueue *queue, AInputEvent **outEvent);

int32_t hooked_AInputQueue_getEvent(AInputQueue *queue, AInputEvent **outEvent) {
  int32_t res = orig_AInputQueue_getEvent(queue, outEvent);
  if (res >= 0 && setup) {
    int eventType = AInputEvent_getType(*outEvent);
    ImGuiIO &io = ImGui::GetIO();
    if (eventType == AINPUT_EVENT_TYPE_MOTION) {
      int action = AMotionEvent_getAction(*outEvent) & AMOTION_EVENT_ACTION_MASK;
      float x = AMotionEvent_getX(*outEvent, 0);
      float y = AMotionEvent_getY(*outEvent, 0);

      float normX = (x / (float)Width) * Width;
      float normY = (y / (float)Height) * Height;

      io.MousePos = ImVec2(normX, normY);

      switch (action) {
        case AMOTION_EVENT_ACTION_DOWN:
          io.MouseDown[0] = true;
          isDragging = true;
          touchStart = ImVec2(normX - windowPos.x, normY - windowPos.y);
          break;
        case AMOTION_EVENT_ACTION_MOVE:
          if (isDragging) {
            ImVec2 targetPos = ImVec2(normX - touchStart.x, normY - touchStart.y);
            windowPos = SmoothMove(windowPos, targetPos, 0.2f);
          }
          break;
        case AMOTION_EVENT_ACTION_UP:
          io.MouseDown[0] = false;
          isDragging = false;
          break;
      }
      return 1;
    }
  }
  return res;
}

DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);

  if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);

  if (!setup) {
    setup = true;
    CreateContext();
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

    switch_theme(current_theme);
    load_features();
    load_imgui_style();
    load_colors();

    io.IniFilename = features["bImguiAutoSave"] ? INI_PATH : nullptr;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = features_float["fFontScale"];
    io.Fonts->AddFontDefault(&font_cfg);
  }

  ImGuiIO &io = ImGui::GetIO();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(Width, Height);
  NewFrame();

  ClearImGuiWindowTracking();

  auto draw = GetBackgroundDrawList();
  noMore(draw);
  visiblePlayerss(draw);

  DrawMenu(io);

  EndFrame();
  Render();
  ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

  return _Draw(dpy, surface);
}

void __IMGUI__() {
  curl_global_init(CURL_GLOBAL_ALL);

  // [ANTI-CRACK] Spawn a persistent background thread to monitor for Debuggers and Memory Dumpers
  std::thread([]() {
      while (true) {
          // Detect Debuggers (IDA, GDB, GameGuardian ptracing)
          FILE *fp = fopen(oxorany("/proc/self/status"), oxorany("r"));
          if (fp) {
              char line[256];
              while (fgets(line, sizeof(line), fp)) {
                  if (strncmp(line, oxorany("TracerPid:"), 10) == 0) {
                      if (atoi(&line[10]) != 0) {
                          kill(getpid(), SIGKILL); // Kill the game instantly if traced
                      }
                      break;
                  }
              }
              fclose(fp);
          }

          // Detect Frida, Xposed, or dumping tools hooked into the process
          fp = fopen(oxorany("/proc/self/maps"), oxorany("r"));
          if (fp) {
              char line[512];
              while (fgets(line, sizeof(line), fp)) {
                  if (strstr(line, oxorany("frida")) || strstr(line, oxorany("xposed"))) {
                      kill(getpid(), SIGKILL); // Kill the game instantly if tools are found
                  }
              }
              fclose(fp);
          }

          std::this_thread::sleep_for(std::chrono::seconds(3));
      }
  }).detach();

  start_entity_reader();

  void *libAndroid = dlopen_ex(("libandroid.so"), 4);
  if (libAndroid) {
    void *symEvent1 = dlsym_ex(libAndroid, ("ANativeWindow_getWidth"));
    DobbyHook(symEvent1, (void *)_ANativeWindow_getWidth, (void **)&orig_ANativeWindow_getWidth);

    void *symEvent2 = dlsym_ex(libAndroid, ("ANativeWindow_getHeight"));
    DobbyHook(symEvent2, (void *)_ANativeWindow_getHeight, (void **)&orig_ANativeWindow_getHeight);

    void *symEvent = dlsym_ex(libAndroid, ("AInputQueue_getEvent"));
    protectFunc(symEvent);
    DobbyHook(symEvent, (void *)hooked_AInputQueue_getEvent, (void **)&orig_AInputQueue_getEvent);
  }
  HOOKIT("libEGL.so", "eglSwapBuffers", Draw);
}
