#pragma once
#include "include/hpp/imgui.h"


namespace ImGui {
    // Custom window dengan background
    bool MainBegin(const char* name, bool* p_open, ImGuiWindowFlags flags) {
        return Begin(name, p_open, flags);
    }

    // Custom button dengan active state
    bool OptButton(const char* label, const ImVec2& size, bool active) {
        if (!active) {
            PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));
            PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        }
        bool clicked = Button(label, size);
        if (!active) {
            PopStyleColor(2);
        }
        return clicked;
    }

    // Custom button variant
    bool OptButton0(const char* label, const ImVec2& size, bool active) {
        return OptButton(label, size, active);
    }

    // Icon button untuk sidebar
    bool ButtonExes1(const char* icon, bool inactive) {
        ImVec4 activeColor = ImVec4(0.3f, 0.3f, 0.8f, 1.0f);
        ImVec4 inactiveColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        
        PushStyleColor(ImGuiCol_Button, inactive ? inactiveColor : activeColor);
        PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.9f, 1.0f));
        PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 1.0f, 1.0f));
        
        bool clicked = Button(icon, ImVec2(60, 60));
        
        PopStyleColor(3);
        return clicked;
    }

    // Sub-tab button
    bool ButtonExes2(const char* label, bool inactive) {
        ImVec4 activeColor = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);
        ImVec4 inactiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        
        PushStyleColor(ImGuiCol_Button, inactive ? inactiveColor : activeColor);
        PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
        PushStyleColor(ImGuiCol_Text, inactive ? ImVec4(0.6f, 0.6f, 0.6f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        bool clicked = Button(label, ImVec2(120, 35));
        
        PopStyleColor(3);
        return clicked;
    }

    // Child window dengan border
    void MenuChild(const char* label, const ImVec2& size) {
        PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
        PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 0.95f));
        PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 0.8f));
        
        BeginChild(label, size, true);
    }

    // Checkbox dengan custom style
    bool Land_Checkbox(const char* label, bool* v) {
        PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
        PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
        PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.9f));
        PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        
        bool changed = Checkbox(label, v);
        
        PopStyleColor(4);
        return changed;
    }
}
