#pragma once

#include "../imgui.h"
#include "../../includes.h"

#include "persistence.h"

struct ImGuiWindowTracker {
    ImVec2 Position;
    ImVec2 Size;
    char Name[64];
};

#define MAX_TRACKED_WINDOWS 8
ImGuiWindowTracker g_TrackedWindows[MAX_TRACKED_WINDOWS];
int g_NumTrackedWindows = 0;

void TrackImGuiWindow(const char* name) {
    if (!_features["bBetterTouch"]) return;

    if (g_NumTrackedWindows < MAX_TRACKED_WINDOWS) {
        ImGuiWindowTracker& window = g_TrackedWindows[g_NumTrackedWindows++];
        window.Position = ImGui::GetWindowPos();
        window.Size = ImGui::GetWindowSize();
        strncpy(window.Name, name, sizeof(window.Name)-1);
        window.Name[sizeof(window.Name)-1] = '\0';
        LOGI("Tracking ImGui window '%s' at pos %f,%f size %f,%f",
             window.Name, window.Position.x, window.Position.y, window.Size.x, window.Size.y);
    }
}

void ClearImGuiWindowTracking() {
    if (!_features["bBetterTouch"]) return;
    
    g_NumTrackedWindows = 0;
}

bool IsImGuiTouchPoint(float x, float y) {
    if (!_features["bBetterTouch"]) return false;

    if (g_NumTrackedWindows == 0) return false;
    
    for (int i = 0; i < g_NumTrackedWindows; i++) {
        const ImGuiWindowTracker& window = g_TrackedWindows[i];
        
        if (x >= window.Position.x && x <= window.Position.x + window.Size.x &&
            y >= window.Position.y && y <= window.Position.y + window.Size.y
        ) {
            LOGI("Touch hit ImGui window '%s' at %f,%f", window.Name, x, y);
            return true;
        }
    }
    
    return false;
}
