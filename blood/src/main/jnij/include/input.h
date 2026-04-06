#pragma once

#include "../And64InlineHook/And64InlineHook.hpp"

#include "hpp/imgui.h"
#include "hpp/imgui/touch.h"
#include "hpp/dlfcn.hpp"
#include "obfuscate.h"

#include <android/input.h>

#include "hpp/imgui/persistence.h"

bool g_DraggingFromOutsideImGui = false;
int g_ActivePointerIndex = -1;

float (*__AMotionEvent_getY)(AInputEvent* motion_event, size_t pointer_index);
DEFINES(float, _AMotionEvent_getX, AInputEvent* motion_event, size_t pointer_index) {
    if (!_features["bBetterTouch"] || !__AMotionEvent_getY) return __AMotionEvent_getX(motion_event, pointer_index);

    float x = __AMotionEvent_getX(motion_event, pointer_index);
    float y = __AMotionEvent_getY(motion_event, pointer_index);
    
    int32_t action = AMotionEvent_getAction(motion_event);
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    
    // Handle different touch actions
    if (actionMasked == AMOTION_EVENT_ACTION_DOWN) {
       //  Start of touch - check if it's outside ImGui
        if (!IsImGuiTouchPoint(x, y)) {
            g_DraggingFromOutsideImGui = true;
            g_ActivePointerIndex = pointer_index;
        } else g_DraggingFromOutsideImGui = false;
    } else if (actionMasked == AMOTION_EVENT_ACTION_UP || actionMasked == AMOTION_EVENT_ACTION_CANCEL) {
        // End of touch - reset flags
        if (pointer_index == g_ActivePointerIndex) {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
        }
    }
    
    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex) return x;
    
    return IsImGuiTouchPoint(x, y) ? -99999.0f : x;
}

float _AMotionEvent_getY(AInputEvent* motion_event, size_t pointer_index) {
    if (!_features["bBetterTouch"] || !__AMotionEvent_getX) return __AMotionEvent_getY(motion_event, pointer_index);

    float y = __AMotionEvent_getY(motion_event, pointer_index);
    float x = __AMotionEvent_getX(motion_event, pointer_index);
    
    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex) return y;
    
    return IsImGuiTouchPoint(x, y) ? -99999.0f : y;
}

DEFINES(void, InitializeMotionEvent, AInputEvent* event, const void* msg) {
    _InitializeMotionEvent(event, msg);
    if (!event) return;
    
    ImGui_ImplAndroid_HandleInputEvent(event, {0.0f, 0.0f});
}

void __INPUT__() {
   HOOKIT("libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE", InitializeMotionEvent);
}

