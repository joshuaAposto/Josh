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
    
    
    if (curl) {
            std::string api_key = oxorany("https://nexusdevhaxpanel.x10.mx/connect");
        curl_easy_setopt(curl, CURLOPT_URL, (api_key.c_str()));
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, oxorany("Content-Type: application/x-www-form-urlencoded"));
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[4096];
        sprintf(data, oxorany("game=BS&user_key=%s&serial=%s"), user_key, UUID.c_str());
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
                        std::string auth = "BS";
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
    errMsg.clear(); 
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
<?php

namespace App\Controllers;

use App\Models\KeysModel;
use App\Models\FuncationModel;

class Connect extends BaseController
{
    protected $model, $game, $uKey, $sDev;

    public function __construct()
    {
        $this->maintenance = false;
        $this->model = new KeysModel();
        $this->model1 = new FuncationModel();
        $this->staticWords = "Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";
    }

    public function loader()
    {
        $html = '<!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
            <title>ANDROID ENGINE PANEL</title>
            <style>
                * {
                    margin: 0;
                    padding: 0;
                    box-sizing: border-box;
                }

                @import url("https://fonts.googleapis.com/css2?family=Poppins:wght@400;600;800&display=swap");
                
                html, body {
                    width: 100%;
                    overflow-x: hidden;
                    position: relative;
                }

                body {
                    margin: 0;
                    padding: 0;
                    background: #0F0F0F;
                    font-family: "Poppins", sans-serif;
                    min-height: 100vh;
                    display: flex;
                    flex-direction: column;
                    justify-content: center;
                    align-items: center;
                }

                .main-container {
                    text-align: center;
                    width: 100%;
                    max-width: 100vw;
                    padding: 20px;
                    box-sizing: border-box;
                }

                .logo {
                    width: clamp(100px, 30vw, 150px);
                    height: clamp(100px, 30vw, 150px);
                    margin-bottom: clamp(20px, 4vw, 30px);
                    animation: float 3s ease-in-out infinite;
                    object-fit: contain;
                }

                @keyframes float {
                    0% { transform: translateY(0px); }
                    50% { transform: translateY(-20px); }
                    100% { transform: translateY(0px); }
                }

                .title {
                    font-size: clamp(24px, 5vw, 36px);
                    font-weight: 800;
                    color: #fff;
                    text-transform: uppercase;
                    margin-bottom: clamp(20px, 4vw, 30px);
                    background: linear-gradient(45deg, #FF3366, #FF6B6B, #4ECDC4, #45B7D1);
                    -webkit-background-clip: text;
                    -webkit-text-fill-color: transparent;
                    animation: gradient 5s ease infinite;
                    background-size: 300% 300%;
                    width: 100%;
                    max-width: 100%;
                }

                @keyframes gradient {
                    0% { background-position: 0% 50%; }
                    50% { background-position: 100% 50%; }
                    100% { background-position: 0% 50%; }
                }

                .loader-wrapper {
                    width: clamp(250px, 80vw, 300px);
                    height: 10px;
                    background: rgba(255,255,255,0.1);
                    border-radius: 20px;
                    overflow: hidden;
                    position: relative;
                    margin: 0 auto;
                }

                .loader-bar {
                    position: absolute;
                    width: 50%;
                    height: 100%;
                    background: linear-gradient(90deg, #FF3366, #FF6B6B);
                    border-radius: 20px;
                    animation: load 1.5s ease-in-out infinite;
                }

                @keyframes load {
                    0% { left: -50%; }
                    100% { left: 100%; }
                }

                .status {
                    margin-top: clamp(15px, 3vw, 20px);
                    color: #fff;
                    font-size: clamp(14px, 4vw, 18px);
                    opacity: 0.8;
                    letter-spacing: 2px;
                }

                .welcome-screen {
                    display: none;
                    position: fixed;
                    top: 0;
                    left: 0;
                    width: 100%;
                    height: 100%;
                    background: #0F0F0F;
                    z-index: 1000;
                    overflow-y: auto;
                    overflow-x: hidden;
                }

                .welcome-content {
                    max-width: min(800px, 95%);
                    margin: 0 auto;
                    text-align: center;
                    color: white;
                    padding: clamp(10px, 3vw, 20px);
                }

                .welcome-title {
                    font-size: clamp(32px, 6vw, 48px);
                    margin-bottom: clamp(20px, 4vw, 30px);
                    color: #FF3366;
                    text-transform: uppercase;
                    word-wrap: break-word;
                }

                .feature-grid {
                    display: grid;
                    grid-template-columns: repeat(auto-fit, minmax(min(250px, 100%), 1fr));
                    gap: clamp(15px, 3vw, 20px);
                    margin-top: clamp(30px, 5vw, 40px);
                    padding: clamp(10px, 2vw, 15px);
                    width: 100%;
                }

                .feature-card {
                    background: rgba(255,255,255,0.05);
                    border-radius: 15px;
                    padding: clamp(15px, 3vw, 20px);
                    transition: transform 0.3s ease;
                    width: 100%;
                }

                .feature-card:hover {
                    transform: translateY(-5px);
                    background: rgba(255,255,255,0.1);
                }

                .feature-icon {
                    font-size: clamp(30px, 6vw, 40px);
                    margin-bottom: clamp(10px, 2vw, 15px);
                }

                .feature-title {
                    font-size: clamp(16px, 4vw, 20px);
                    font-weight: 600;
                    margin-bottom: clamp(8px, 2vw, 10px);
                    color: #FF6B6B;
                }

                .feature-desc {
                    font-size: clamp(12px, 3vw, 14px);
                    line-height: 1.6;
                    color: rgba(255,255,255,0.7);
                }

                .btn-start {
                    display: inline-block;
                    margin-top: clamp(30px, 5vw, 40px);
                    padding: clamp(10px, 3vw, 15px) clamp(20px, 5vw, 40px);
                    background: linear-gradient(45deg, #FF3366, #FF6B6B);
                    color: white;
                    text-decoration: none;
                    border-radius: 30px;
                    font-weight: 600;
                    font-size: clamp(14px, 3vw, 16px);
                    text-transform: uppercase;
                    letter-spacing: 1px;
                    transition: transform 0.3s ease, box-shadow 0.3s ease;
                    white-space: nowrap;
                }

                .btn-start:hover {
                    transform: translateY(-2px);
                    box-shadow: 0 5px 15px rgba(255, 51, 102, 0.3);
                }

                @media (max-width: 768px) {
                    .welcome-content {
                        padding: 15px;
                        width: 100%;
                    }
                    
                    .feature-grid {
                        grid-template-columns: 1fr;
                        width: 100%;
                    }
                    
                    .btn-start {
                        width: 80%;
                        max-width: 300px;
                        text-align: center;
                    }

                    .feature-card {
                        margin: 10px 0;
                        width: 100%;
                    }
                }

                @media (max-width: 480px) {
                    body {
                        width: 100vw;
                    }

                    .welcome-title {
                        font-size: 28px;
                        padding: 0 10px;
                    }

                    .feature-icon {
                        font-size: 32px;
                    }

                    .feature-title {
                        font-size: 18px;
                    }

                    .feature-desc {
                        font-size: 12px;
                    }
                }
            </style>
        </head>
        <body>
            <div class="main-container">
                <img src="https://Sasuke.web.id/public/NAMAN.png" alt="Logo" class="logo">
                <div class="title">ANDROID ENGINE</div>
                <div class="loader-wrapper">
                    <div class="loader-bar"></div>
                </div>
                <div class="status">Initializing System...</div>
            </div>

            <div class="welcome-screen">
                <div class="welcome-content">
                    <h1 class="welcome-title">Welcome to Android Engine</h1>
                    <p>Experience Gaming at a Whole New Level</p>
                    
                    <div class="feature-grid">
                        <div class="feature-card">
                            <div class="feature-icon">🎮</div>
                            <div class="feature-title">Premium Mods</div>
                            <div class="feature-desc">Access exclusive gaming modifications with enhanced features.</div>
                        </div>
                        
                        <div class="feature-card">
                            <div class="feature-icon">🛡️</div>
                            <div class="feature-title">Anti-Ban</div>
                            <div class="feature-desc">Advanced protection system to keep your account safe.</div>
                        </div>
                        
                        <div class="feature-card">
                            <div class="feature-icon">🚀</div>
                            <div class="feature-title">Performance</div>
                            <div class="feature-desc">Optimized for smooth gameplay without any lag.</div>
                        </div>
                        
                        <div class="feature-card">
                            <div class="feature-icon">⚡</div>
                            <div class="feature-title">Quick Updates</div>
                            <div class="feature-desc">Regular updates with new features and improvements.</div>
                        </div>
                    </div>
                    
                    <a href="https://telegram.me/Android_Engine_Owner" target="_blank" class="btn-start">Get Started</a>
                </div>
            </div>

            <script>
                const statusText = document.querySelector(".status");
                const loadingTexts = [
                    "Initializing System...",
                    "Checking Updates...",
                    "Loading Resources...",
                    "Almost Ready..."
                ];
                let currentText = 0;

                setInterval(() => {
                    statusText.textContent = loadingTexts[currentText];
                    currentText = (currentText + 1) % loadingTexts.length;
                }, 1000);

                setTimeout(() => {
                    document.querySelector(".main-container").style.display = "none";
                    document.querySelector(".welcome-screen").style.display = "block";
                    
                    document.querySelectorAll(".feature-card").forEach((card, index) => {
                        setTimeout(() => {
                            card.style.opacity = "0";
                            card.style.transform = "translateY(20px)";
                            card.style.transition = "all 0.5s ease";
                            
                            setTimeout(() => {
                                card.style.opacity = "1";
                                card.style.transform = "translateY(0)";
                            }, 100);
                        }, index * 200);
                    });
                }, 5000);
            </script>
        </body>
        </html>';

        return $this->response->setHeader('Content-Type', 'text/html')->setBody($html);
    }

    public function index()
    {
        if ($this->request->getPost()) {
            return $this->index_post();
        } else {
            return $this->loader();
        }
    }

    public function index_post()
    {
        $isMT = $this->maintenance;
        $game = $this->request->getPost('game');
        $uKey = $this->request->getPost('user_key');
        $sDev = $this->request->getPost('serial');

        $form_rules = [
            'game' => 'required|alpha_dash'
        ];

        if (!$this->validate($form_rules)) {
            $data = [
                'status' => false,
                'reason' => "Bad Parameter",
            ];
            return $this->response->setJSON($data);
        }

        if ($isMT) {
            $data = [
                'status' => false,
                'reason' => 'MAINTENANCE'
            ];
        } else {
            if (!$game or !$uKey or !$sDev) {
                $data = [
                    'status' => false,
                    'reason' => 'INVALID PARAMETER'
                ];
            } else {
                $time = new \CodeIgniter\I18n\Time;
                $model = $this->model;
                $model1 = $this->model1;
                $findKey = $model->getKeysGame(['user_key' => $uKey, 'game' => $game]);
                $findFuncation = $model1->getFuncation(['NAMAN_SINGH' => "NAMAN_SINGH", 'id_path' => 1]);
        
                if ($findKey) {
                    if ($findKey->status != 1) {
                        $data = [
                            'status' => false,
                            'reason' => 'USER BLOCKED'
                        ];
                    } else {
                        $id_keys = $findKey->id_keys;
                        $duration = $findKey->duration;
                        $expired = $findKey->expired_date;
                        $max_dev = $findKey->max_devices;
                        $devices = $findKey->devices;
    
                        function checkDevicesAdd($serial, $devices, $max_dev)
                        {
                            $lsDevice = explode(",", $devices);
                            $cDevices = isset($devices) ? count($lsDevice) : 0;
                            $serialOn = in_array($serial, $lsDevice);
    
                            if ($serialOn) {
                                return true;
                            } else {
                                if ($cDevices < $max_dev) {
                                    array_push($lsDevice, $serial);
                                    $setDevice = reduce_multiples(implode(",", $lsDevice), ",", true);
                                    return ['devices' => $setDevice];
                                } else {
                                    return false;
                                }
                            }
                        }
    
                        if (!$expired) {
                            $setExpired = $time::now()->addHours($duration);
                            $model->update($id_keys, ['expired_date' => $setExpired]);
                            $data['status'] = true;
                        } else {
                            if ($time::now()->isBefore($expired)) {
                                $data['status'] = true;
                            } else {
                                $data = [
                                    'status' => false,
                                    'reason' => 'EXPIRED KEY'
                                ];
                            }
                        }
    
                        if ($findFuncation->Online !== 'false') {
                            $data = [
                                'status' => false,
                                'reason' => $findFuncation->Maintenance
                            ];
                        } else {
                            if ($data['status']) {
                                $devicesAdd = checkDevicesAdd($sDev, $devices, $max_dev);
                                if ($devicesAdd) {
                                    if (is_array($devicesAdd)) {
                                        $model->update($id_keys, $devicesAdd);
                                    }
                                    
                                    // PENTING: Ini yang diubah - tidak pakai MD5 lagi
                                    // Token sekarang langsung string plain seperti di C++
                                    $token = "$game-$uKey-$sDev-$this->staticWords";
                               
                                    if ($expired == null) {  
                                        $expiredX = $time::now()->addHours($duration);
                                    } else {  
                                        $expiredX = $findKey->expired_date;  
                                    }
                                 
                                    $data = [
                                        'status' => true,
                                        'data' => [
                                            'token' => $token,  // Token plain text tanpa MD5
                                            'EXP' => $expiredX, 
                                            'rng' => $time->getTimestamp(),
                                            'mod_status' => $findFuncation->status,
                                            'Online' => $findFuncation->Online,
                                            'Bullet' => $findFuncation->Bullet,
                                            'Aimbot' => $findFuncation->Aimbot,
                                            'Memory' => $findFuncation->Memory,
                                            'Esp' => $findFuncation->Esp,
                                            'ModName' => $findFuncation->ModName,
                                            'ftext' => $findFuncation->ftext
                                        ],
                                    ];
                                } else {
                                    $data = [
                                        'status' => false,
                                        'reason' => 'MAX DEVICE REACHED'
                                    ];
                                }
                            }
                        }
                    }
                } else {
                    $data = [
                        'status' => false,
                        'reason' => 'USER OR GAME NOT REGISTERED'
                    ];
                }
            }
        }
        return $this->response->setJSON($data);
    }
}
*/

