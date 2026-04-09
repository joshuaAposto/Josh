#pragma once
#include "imgui/imgui.h"

bool Toggle(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = 35.0f;
    float width = height * 1.9f;
    float radius = height * 0.5f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    bool clicked = ImGui::IsItemClicked();
    if (clicked)
        *v = !*v;

    // Warna background: Cyan (ON), Merah (OFF)

    ImU32 col_bg = *v
    ? ImGui::GetColorU32(ImVec4(1.00f, 0.00f, 0.00f, 1.00f)) // Red when ON
    : ImGui::GetColorU32(ImVec4(0.15f, 0.15f, 0.15f, 1.00f)); // Black when OFF

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, radius);

    float circle_x = *v ? (p.x + width - radius) : (p.x + radius);
    draw_list->AddCircleFilled(ImVec2(circle_x, p.y + radius), radius - 2.0f, IM_COL32(255, 255, 255, 255));

    ImGui::SameLine();
    ImGui::Text("%s", str_id);

    return clicked;
}

