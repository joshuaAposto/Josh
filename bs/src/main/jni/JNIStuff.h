#pragma once
#include "ByNameModding/Tools.h"
JNIEnv *genv;

bool bInitDone;

#define g_LogTag OBFUSCATE("Hello-LOGGER")

jboolean (*old_nativeInjectEvent)(JNIEnv *env, jobject m_obj, jobject inputEvent);
jboolean nativeInjectEvent(JNIEnv *env, jobject thiz, jobject inputEvent) {
	
	jclass motionEventClass = env->FindClass(OBFUSCATE("android/view/MotionEvent"));
	
	if (env->IsInstanceOf(inputEvent, motionEventClass)) {
		
		jmethodID getX = env->GetMethodID(motionEventClass, OBFUSCATE("getRawX"), OBFUSCATE("()F"));
		jmethodID getY = env->GetMethodID(motionEventClass, OBFUSCATE("getRawY"), OBFUSCATE("()F"));
		jmethodID getAction = env->GetMethodID(motionEventClass, OBFUSCATE("getAction"), OBFUSCATE("()I"));
		
		jfloat x = env->CallFloatMethod(inputEvent, getX);
		jfloat y = env->CallFloatMethod(inputEvent, getY);
		jint action = env->CallIntMethod(inputEvent, getAction);
		
		ImGuiIO& io = ImGui::GetIO();
		switch (action) {
			case 0:
				io.MouseDown[0] = true;
				break;
			case 1:
				io.MouseDown[0] = false;
				break;
		}
		io.MousePos = ImVec2(x, y);
	}
	return old_nativeInjectEvent(env, thiz, inputEvent);
}

jint (*old_RegisterNatives)(JNIEnv* env, jclass klazz, const JNINativeMethod*, jint);
jint hook_RegisterNatives(JNIEnv* env, jclass klazz, const JNINativeMethod* methods, jint methodcount) {
	
	for (int i = 0; i < methodcount; ++i) {
		
		if (strcmp(methods[i].name, OBFUSCATE("nativeInjectEvent")) == 0) {
			Tools::Hook((void *) methods[i].fnPtr, (void *) nativeInjectEvent, (void **) &old_nativeInjectEvent);
		}
		
	}
	return old_RegisterNatives(env, klazz, methods, methodcount);
}

std::string getPublicStaticString(JNIEnv *env, const char *className, const char *fieldName) {
    jclass clazz = env->FindClass(className);
    if (clazz != nullptr) {
        jfieldID fid = env->GetStaticFieldID(clazz, fieldName, OBFUSCATE("Ljava/lang/String;"));
        if (fid != nullptr) {
            jstring GladioReceiver = (jstring) env->GetStaticObjectField(clazz, fid);
            jboolean blnIsCopy;
            std::string mystr = env->GetStringUTFChars(GladioReceiver, &blnIsCopy);
            return mystr;
        }
    }
    return OBFUSCATE("ERROR");
}

std::string CreateDeviceUniqueID(JNIEnv *env) {
    std::string strReturn;
    std::string board = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("BOARD"));
    std::string brand = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("BRAND"));
    std::string display = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("DISPLAY"));
    std::string device = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("DEVICE"));
    std::string manufacturer = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("MANUFACTURER"));
    std::string model = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("MODEL"));
    std::string product = getPublicStaticString(env, OBFUSCATE("android/os/Build"), OBFUSCATE("PRODUCT"));

    int mod = 10;
    int a1 = ((int) board.size()) % mod;
    int a2 = ((int) brand.size()) % mod;
    int a3 = ((int) display.size()) % mod;
    int a4 = ((int) device.size()) % mod;
    int a5 = ((int) manufacturer.size()) % mod;
    int a6 = ((int) model.size()) % mod;
    int a7 = ((int) product.size()) % mod;

    strReturn = "699" +
            std::to_string(a1) +
            std::to_string(a2) +
            std::to_string(a3) +
            std::to_string(a4) +
            std::to_string(a5) +
            std::to_string(a6) +
            std::to_string(a7);

    return strReturn;
}


