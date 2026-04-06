#pragma once

#include "JNIStuff.h"
#include "enc/StrEnc.h"
#include "enc/json.hpp"
#include "enc/md5.h"
#include "Oxorany/oxorany.h"
#include "curl/curl-android-arm64-v8a/include/curl/curl.h"

JavaVM *jvm;

using json = nlohmann::json;
std::string g_Token, g_Auth;
bool bValid = false;
std::string EXP = " ";
std::string mod_status = " ";
std::string DeviceID;
std::string DeviceModel = " ";
std::string DeviceBrand;
std::string vrsicheats = OBFUSCATE("VIP BS 008");


const char *GetAndroidID(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass,OBFUSCATE("getContentResolver"),OBFUSCATE("()Landroid/content/ContentResolver;"));
    jclass settingSecureClass = env->FindClass(OBFUSCATE("android/provider/Settings$Secure"));
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass,OBFUSCATE("getString"), OBFUSCATE("(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;"));

    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring) env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj,env->NewStringUTF(OBFUSCATE("android_id")));
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceModel(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("MODEL"),OBFUSCATE("Ljava/lang/String;"));

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceBrand(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("BRAND"),OBFUSCATE("Ljava/lang/String;"));

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid) {
    jclass uuidClass = env->FindClass(OBFUSCATE("java/util/UUID"));

    auto len = strlen(uuid);

    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *) uuid);

    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass,OBFUSCATE("nameUUIDFromBytes"),OBFUSCATE("([B)Ljava/util/UUID;"));
    jmethodID toStringMethod = env->GetMethodID(uuidClass, OBFUSCATE("toString"),OBFUSCATE("()Ljava/lang/String;"));

    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring) env->CallObjectMethod(obj, toStringMethod);
    return env->GetStringUTFChars(str, 0);
}

struct MemoryStruct {
        char *memory;
        size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *) userp;
        
        mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
        if (mem->memory == NULL) {
                return 0;
        }
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
        return realsize;
}

int ShowSoftKeyboardInput() {
        jint result;
        jint flags = 0;
        
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, NULL);
        
        jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
        auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
        env->CallStaticVoidMethod(looperClass, prepareMethod);
        
        jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
        jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
        jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
        
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
        jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
        
        jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
        jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
        jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
        jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
        jobject callObjectMethod = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
        
        jclass classInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
        
        if (result) {
                env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, flags);
        } else {
                env->CallVoidMethod(callObjectMethod, toggleSoftInputId, flags, flags);
        }
        
        env->DeleteLocalRef(classInputMethodManager);
        env->DeleteLocalRef(callObjectMethod);
        env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
        jvm->DetachCurrentThread();
        
        return result;
}

int PollUnicodeChars() {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, NULL);
        
        jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
        auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
        env->CallStaticVoidMethod(looperClass, prepareMethod);
        
        jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
        jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
        jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
        
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
        jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
        
        jclass keyEventClass = env->FindClass(OBFUSCATE("android/view/KeyEvent"));
        jmethodID getUnicodeCharMethod = env->GetMethodID(keyEventClass, OBFUSCATE("getUnicodeChar"), OBFUSCATE("(I)I"));
        
        ImGuiIO& io = ImGui::GetIO();
    
        int return_key = env->CallIntMethod(keyEventClass, getUnicodeCharMethod);
        
        env->DeleteLocalRef(keyEventClass);
        env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
        jvm->DetachCurrentThread();
        
        return return_key;
}

void CopyLinkToClipboard(const std::string& text) {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);

    // Prepare Looper for background thread
    jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    jmethodID prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);
    env->ExceptionClear(); // Ignore if already prepared

    // Get Application Context
    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);

    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);

    // Get Clipboard Manager
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jstring str = env->NewStringUTF(OBFUSCATE("clipboard"));
    jobject clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);

    // Create ClipData
    jclass clipDataClass = env->FindClass(OBFUSCATE("android/content/ClipData"));
    jmethodID newPlainTextMethod = env->GetStaticMethodID(clipDataClass, OBFUSCATE("newPlainText"), OBFUSCATE("(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;"));
    jstring label = env->NewStringUTF(OBFUSCATE("RyukoLink"));
    jstring jText = env->NewStringUTF(text.c_str());
    jobject clipData = env->CallStaticObjectMethod(clipDataClass, newPlainTextMethod, label, jText);

    // Set Clipboard
    jclass clipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    jmethodID setPrimaryClipMethod = env->GetMethodID(clipboardManagerClass, OBFUSCATE("setPrimaryClip"), OBFUSCATE("(Landroid/content/ClipData;)V"));
    env->CallVoidMethod(clipboardManager, setPrimaryClipMethod, clipData);

    // Clean up
    env->DeleteLocalRef(label);
    env->DeleteLocalRef(jText);
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(clipDataClass);
    env->DeleteLocalRef(clipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);

    jvm->DetachCurrentThread();
}

void OpenBrowserWithUrl(const std::string& url) {
    JNIEnv *env;
    bool did_attach = false;
    int stat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (stat == JNI_EDETACHED) {
        jvm->AttachCurrentThread(&env, NULL);
        did_attach = true;
    }

    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThreadClass, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = env->CallStaticObjectMethod(activityThreadClass, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThreadClass, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = env->CallObjectMethod(at, getApplication);

    jclass uriClass = env->FindClass(OBFUSCATE("android/net/Uri"));
    jmethodID uriParse = env->GetStaticMethodID(uriClass, OBFUSCATE("parse"), OBFUSCATE("(Ljava/lang/String;)Landroid/net/Uri;"));
    jstring jUrl = env->NewStringUTF(url.c_str());
    jobject uri = env->CallStaticObjectMethod(uriClass, uriParse, jUrl);

    jclass intentClass = env->FindClass(OBFUSCATE("android/content/Intent"));
    jmethodID intentCtor = env->GetMethodID(intentClass, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;Landroid/net/Uri;)V"));
    jstring actionView = env->NewStringUTF(OBFUSCATE("android.intent.action.VIEW"));
    jobject intent = env->NewObject(intentClass, intentCtor, actionView, uri);

    jmethodID addFlags = env->GetMethodID(intentClass, OBFUSCATE("addFlags"), OBFUSCATE("(I)Landroid/content/Intent;"));
    env->CallObjectMethod(intent, addFlags, 0x10000000); // FLAG_ACTIVITY_NEW_TASK

    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID startActivity = env->GetMethodID(contextClass, OBFUSCATE("startActivity"), OBFUSCATE("(Landroid/content/Intent;)V"));
    env->CallVoidMethod(context, startActivity, intent);

    env->DeleteLocalRef(jUrl);
    env->DeleteLocalRef(actionView);
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(intent);
    env->DeleteLocalRef(uriClass);
    env->DeleteLocalRef(intentClass);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(at);
    env->DeleteLocalRef(activityThreadClass);

    if (did_attach) jvm->DetachCurrentThread();
}

std::string getClipboard() {
    std::string result;
    JNIEnv *env;

    jvm->AttachCurrentThread(&env, NULL);

    auto looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"),
                                                OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);

    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE(
            "sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass,
                                                               sCurrentActivityThreadField);

    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass,
                                                        OBFUSCATE("mInitialApplication"),
                                                        OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread,
                                                      mInitialApplicationField);

    auto contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    auto getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"),
                                                   OBFUSCATE(
                                                           "(Ljava/lang/String;)Ljava/lang/Object;"));

    auto str = env->NewStringUTF(OBFUSCATE("clipboard"));
    auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    env->DeleteLocalRef(str);

    jclass ClipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    auto getText = env->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"),
                                    OBFUSCATE("()Ljava/lang/CharSequence;"));

    jclass CharSequenceClass = env->FindClass(OBFUSCATE("java/lang/CharSequence"));
    auto toStringMethod = env->GetMethodID(CharSequenceClass, OBFUSCATE("toString"),
                                           OBFUSCATE("()Ljava/lang/String;"));

    auto text = env->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) env->CallObjectMethod(text, toStringMethod);
        result = env->GetStringUTFChars(str, 0);
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }

    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    jvm->DetachCurrentThread();
    return result.c_str();
}

std::string Login(const char *user_key) {
        JNIEnv *env;
    jvm->AttachCurrentThread(&env, 0);
        
        auto looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
        auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
        env->CallStaticVoidMethod(looperClass, prepareMethod);
        
        jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
        jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
        jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
        
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
        jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
        
        std::string hwid = user_key;
        hwid += GetAndroidID(env, mInitialApplication);
        hwid += GetDeviceModel(env);
        hwid += GetDeviceBrand(env);
        std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());
        DeviceBrand = GetDeviceBrand(env);
    DeviceModel = GetDeviceModel(env);
    DeviceID = GetAndroidID(env, mInitialApplication);
        jvm->DetachCurrentThread();
        std::string errMsg;
        
        struct MemoryStruct chunk{};
        chunk.memory = (char *) malloc(1);
        chunk.size = 0;
        
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        
        
        // KURO PANEL
    
   if (curl) {
            std::string api_key = oxorany("http://panelofdcibx4am911.x10.mx/connect");
        curl_easy_setopt(curl, CURLOPT_URL, (api_key.c_str()));
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, oxorany("Content-Type: application/x-www-form-urlencoded"));
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[4096];
        sprintf(data, oxorany("game=PUBG&user_key=%s&serial=%s"), user_key, UUID.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                if (result[std::string(OBFUSCATE("status"))] == true) {
                    std::string token = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("token"))].get<std::string>();
                    time_t rng = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("rng"))].get<time_t>();
                    
                    EXP = result["data"]["EXP"].get<std::string>();
                    mod_status = result["data"]["mod_status"].get<std::string>();
                    
                    if (rng + 30 > time(0)) {
                        std::string auth = "PUBG";
                        auth += "-";
                        auth += user_key;
                        auth += "-";
                        auth += hwid;
                        auth += "-";        
                        auth += "Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";
                                                                      
                        bValid = g_Token == g_Auth;
                                           
                        if (g_Token == g_Auth) {
                                                        
                        }
                                         
                   }
                   
                   
                } else {
                    errMsg = result[std::string(oxorany("reason"))].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = "{";
                errMsg += e.what();
                errMsg += "}\n{";
                errMsg += chunk.memory;
                errMsg += "}";
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    jvm->DetachCurrentThread();
    return bValid ? "OK" : errMsg;
}

    
 /*   
if (curl) {
    std::string api_key = oxorany("https://pastebin.com/raw/Gi5BLpAR");
    curl_easy_setopt(curl, CURLOPT_URL, api_key.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        try {
            json result = json::parse(chunk.memory);
            auto keys = result["keys"];
            if (keys.contains(user_key)) {
                std::string exp_date = keys[user_key];
                
                // Check expiry
                time_t now = time(0);
                struct tm tm_exp = {};
                strptime(exp_date.c_str(), "%Y-%m-%d", &tm_exp);
                time_t t_exp = mktime(&tm_exp);
                
                if (t_exp >= now) {
                    bValid = true;
                } else {
                    errMsg = "KEY EXPIRED BUY @EXSTARCOOL";
                }
            } else {
                errMsg = "KEY ERROR BUY @EXSTARCOOL";
            }
        } catch (json::exception &e) {
            errMsg = "{";
            errMsg += e.what();
            errMsg += "}\n{";
            errMsg += chunk.memory;
            errMsg += "}";
        }
    } else {
        errMsg = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
}
jvm->DetachCurrentThread();
return bValid ? "OK" : errMsg;
}*/
