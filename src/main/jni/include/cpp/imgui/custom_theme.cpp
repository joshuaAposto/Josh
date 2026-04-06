#pragma once

#include "../../hpp/imgui/custom_theme.h"

void StyleColorsCustom(ImGuiStyle* _style) {
    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();

    // Rounding dan border
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.TabRounding = 4.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabBorderSize = 1.0f;

    // Warna dasar
    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // putih
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.40f); // hitam transparan
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.00f, 0.10f, 0.35f); // hitam pink gelap
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.15f, 0.00f, 0.15f, 0.85f);

    // Border
    style.Colors[ImGuiCol_Border]                = ImVec4(1.00f, 0.40f, 0.70f, 0.50f); // pink
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frame
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.00f, 0.20f, 0.50f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(1.00f, 0.40f, 0.70f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 0.40f, 0.70f, 0.60f);

    // Title Bar
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.15f, 0.00f, 0.15f, 0.85f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.20f, 0.00f, 0.20f, 0.90f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.10f, 0.00f, 0.10f, 0.40f);

    // Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.10f, 0.00f, 0.10f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(1.00f, 0.40f, 0.70f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(1.00f, 0.50f, 0.80f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(1.00f, 0.50f, 0.80f, 1.00f);

    // Buttons
    style.Colors[ImGuiCol_Button]                = ImVec4(0.40f, 0.00f, 0.30f, 0.50f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(1.00f, 0.40f, 0.70f, 0.60f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(1.00f, 0.50f, 0.80f, 0.90f);

    // Header (used in tabs, tree, etc)
    style.Colors[ImGuiCol_Header]                = ImVec4(1.00f, 0.40f, 0.70f, 0.40f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(1.00f, 0.50f, 0.80f, 0.60f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(1.00f, 0.60f, 0.90f, 0.90f);

    // Tabs
    style.Colors[ImGuiCol_Tab]                   = ImVec4(0.25f, 0.00f, 0.20f, 0.60f);
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(1.00f, 0.50f, 0.80f, 0.90f);
    style.Colors[ImGuiCol_TabActive]             = ImVec4(1.00f, 0.40f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.15f, 0.00f, 0.10f, 0.40f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.25f, 0.00f, 0.20f, 0.60f);

    // Sliders
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(1.00f, 0.40f, 0.70f, 0.60f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(1.00f, 0.50f, 0.80f, 0.80f);

    // Checkbox
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    // Resize grip
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 0.50f, 0.80f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 0.60f, 0.90f, 0.90f);

    // Plot
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.40f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.40f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.50f, 0.80f, 1.00f);

    // Text selection
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(1.00f, 0.50f, 0.80f, 0.30f);

    // Modal window dim
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.10f, 0.00f, 0.10f, 0.35f);
}