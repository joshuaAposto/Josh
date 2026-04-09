LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := curl/curl-android-$(TARGET_ARCH_ABI)/lib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := curl/openssl-android-$(TARGET_ARCH_ABI)/lib/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := curl/openssl-android-$(TARGET_ARCH_ABI)/lib/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := BSglobal


LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fexceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/curl-android-$(TARGET_ARCH_ABI)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/openssl-android-$(TARGET_ARCH_ABI)/include


LOCAL_SRC_FILES := main.cpp \
DomainScanner.cpp \
NetworkHooks.cpp \
AntiDetection.cpp \
AntiTelemetry.cpp \
    KittyMemory/KittyMemory.cpp\
    KittyMemory/KittyUtils.cpp\
    KittyMemory/MemoryBackup.cpp\
    KittyMemory/MemoryPatch.cpp\
    And64InlineHook/And64InlineHook.cpp \
    include/cpp/imgui.cpp \
    include/cpp/dlfcn.cpp \
    include/cpp/srcpy.cpp \
    ByNameModding/Tools.cpp \
    include/cpp/imgui/custom_theme.cpp \
    Oxorany/oxorany.cpp \
    xhook/xhook.c \
    xhook/xh_core.c \
    xhook/xh_elf.c \
    xhook/xh_jni.c \
    xhook/xh_log.c \
    xhook/xh_util.c \
    xhook/xh_version.c \
    src/xdl.c \
    imgui/stb_image.cpp \
    src/xdl_iterate.c \
    src/xdl_linker.c \
    src/xdl_lzma.c \
    src/xdl_util.c    

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3 -lz

LOCAL_STATIC_LIBRARIES := libdobby libcurl libssl libcrypto



include $(BUILD_SHARED_LIBRARY)
