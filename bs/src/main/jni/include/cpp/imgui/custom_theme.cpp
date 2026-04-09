#pragma once

#include "../../hpp/imgui/custom_theme.h"

void StyleColorsCustom(ImGuiStyle* _style) {
    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    
    style.WindowRounding = 7.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowBorderSize = 2.0f;
    style.ChildBorderSize = 2.0f;
    style.PopupBorderSize = 2.0f;
    style.FrameBorderSize = 2.0f;

    // Spacing and Padding
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 10.0f;

	
	    ImVec4* colors = style.Colors;

// Base background
ImVec4 baseColor = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);  // #282b32
for (int i = 0; i < ImGuiCol_COUNT; i++)
    colors[i] = baseColor;

// Text
colors[ImGuiCol_Text]              = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
colors[ImGuiCol_TextDisabled]      = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

// Checkbox & Frame Backgrounds
colors[ImGuiCol_FrameBg]           = ImVec4(0.29f, 0.33f, 0.40f, 1.00f); // #4b5366
colors[ImGuiCol_FrameBgHovered]    = ImVec4(0.32f, 0.36f, 0.44f, 1.00f);
colors[ImGuiCol_FrameBgActive]     = ImVec4(0.35f, 0.40f, 0.48f, 1.00f);

// Checkmark
colors[ImGuiCol_CheckMark]         = ImVec4(0.30f, 0.40f, 0.60f, 1.00f);

// Slider
colors[ImGuiCol_SliderGrab]        = ImVec4(0.51f, 0.62f, 0.76f, 1.00f); // #829fc3
colors[ImGuiCol_SliderGrabActive]  = ImVec4(0.51f, 0.62f, 0.76f, 1.00f);

// Button
colors[ImGuiCol_Button]            = ImVec4(0.18f, 0.20f, 0.25f, 1.00f); // #2d333f
colors[ImGuiCol_ButtonHovered]     = ImVec4(0.22f, 0.24f, 0.29f, 1.00f);
colors[ImGuiCol_ButtonActive]      = ImVec4(0.26f, 0.28f, 0.34f, 1.00f);

// Tabs
colors[ImGuiCol_Tab]               = ImVec4(0.22f, 0.23f, 0.28f, 1.00f);
colors[ImGuiCol_TabHovered]        = ImVec4(0.26f, 0.27f, 0.32f, 1.00f);
colors[ImGuiCol_TabActive]         = ImVec4(0.30f, 0.31f, 0.36f, 1.00f);

// Scrollbar
colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.25f, 0.26f, 0.30f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.31f, 0.35f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.35f, 0.36f, 0.40f, 1.00f);

// Borders
colors[ImGuiCol_Border]           = ImVec4(0.18f, 0.19f, 0.23f, 1.00f); // #2d3341
colors[ImGuiCol_BorderShadow]     = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

// Separator (Super black + make thickness via style vars below)
colors[ImGuiCol_Separator]        = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // black
colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
colors[ImGuiCol_SeparatorActive]  = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

// Window background (lighter)
colors[ImGuiCol_WindowBg]         = ImVec4(0.27f, 0.29f, 0.37f, 1.00f); // #444b5e
}
