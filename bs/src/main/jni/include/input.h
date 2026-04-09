#pragma once

#include "../And64InlineHook/And64InlineHook.hpp"
#include "include/xdl.h"
#include "hpp/imgui.h"
#include "hpp/imgui/touch.h"
#include "hpp/dlfcn.hpp"
#include "imgui/backends/imgui_impl_android.h"
// #include "obfuscate.h"

#include <android/input.h>

#include "hpp/imgui/persistence.h"

bool g_DraggingFromOutsideImGui = false;
int g_ActivePointerIndex = -1;


bool IsSupportedAction(int32_t actionMasked) {
    switch (actionMasked) {
        case AMOTION_EVENT_ACTION_DOWN:
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_MOVE:
        case AMOTION_EVENT_ACTION_CANCEL:
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
        case AMOTION_EVENT_ACTION_POINTER_UP:
        case AMOTION_EVENT_ACTION_HOVER_MOVE:   // Android 15 support
        case AMOTION_EVENT_ACTION_SCROLL:       // Android 15 support
            return true;
        default:
            return false;
    }
}



float (*__AMotionEvent_getY)(AInputEvent* motion_event, size_t pointer_index);
DEFINES(float, _AMotionEvent_getX, AInputEvent* motion_event, size_t pointer_index) {
    if (!_features[xorstr_("bBetterTouch")] || !__AMotionEvent_getY)
        return __AMotionEvent_getX(motion_event, pointer_index);

    float x = __AMotionEvent_getX(motion_event, pointer_index);
    float y = __AMotionEvent_getY(motion_event, pointer_index);

    int32_t action = AMotionEvent_getAction(motion_event);
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;

    if (!IsSupportedAction(actionMasked)) return x;

    if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN) {
        if (!IsImGuiTouchPoint(x, y)) {
            g_DraggingFromOutsideImGui = true;
            g_ActivePointerIndex = pointer_index;
        } else g_DraggingFromOutsideImGui = false;
    } else if (actionMasked == AMOTION_EVENT_ACTION_UP ||
               actionMasked == AMOTION_EVENT_ACTION_POINTER_UP ||
               actionMasked == AMOTION_EVENT_ACTION_CANCEL) {
        if (pointer_index == g_ActivePointerIndex) {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
        }
    }

    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex)
        return x;

    return IsImGuiTouchPoint(x, y) ? -99999.0f : x;
}

float _AMotionEvent_getY(AInputEvent* motion_event, size_t pointer_index) {
    if (!_features[xorstr_("bBetterTouch")] || !__AMotionEvent_getX)
        return __AMotionEvent_getY(motion_event, pointer_index);

    float y = __AMotionEvent_getY(motion_event, pointer_index);
    float x = __AMotionEvent_getX(motion_event, pointer_index);

    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex)
        return y;

    return IsImGuiTouchPoint(x, y) ? -99999.0f : y;
}

DEFINES(void, InitializeMotionEvent, AInputEvent* event, const void* msg) {
    _InitializeMotionEvent(event, msg);
    if (!event) return;

    ImGui_ImplAndroid_HandleInputEvent(event, {0.0f, 0.0f});
}

int32_t (*origConsume)(void *thiz, void *arg1, bool arg2, long arg3, uint32_t *arg4, AInputEvent **input_event) = nullptr;
void (*origInput)(void *thiz, void *ex_ab, void *ex_ac) = nullptr;

int32_t myConsume(void *thiz, void *arg1, bool arg2, long arg3, uint32_t *arg4, AInputEvent **input_event) {
    int32_t result = origConsume(thiz, arg1, arg2, arg3, arg4, input_event);
    if (result != 0 || *input_event == nullptr)
        return result;
    ImGui_ImplAndroid_HandleInputEvent(*input_event, ImVec2(1.0f, 1.0f));
    return result;
}

void myInput(void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)thiz, ImVec2(1.0f, 1.0f));
}

void __INPUT__() {
    void *handle_input = xdl_open(xorstr_("libinput.so"), XDL_DEFAULT);

    void *xdl_sym_input = xdl_sym(handle_input,
                                  xorstr_("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"),
                                  nullptr);

    if (xdl_sym_input == nullptr) {
        xdl_sym_input = xdl_sym(handle_input,
                                xorstr_("_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEblPjPPNS_10InputEventE"),
                                nullptr);
        DobbyHook(xdl_sym_input, (void*)myConsume, (void**)&origConsume);
    } else {
        DobbyHook(xdl_sym_input, (void*)myInput, (void**)&origInput);
    }
}
