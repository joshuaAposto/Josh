#pragma once

#include "hpp/imgui.h"
#include <string>
#include <vector>

static ImVec2 Centre(ImVec2 pos, std::string str, float size) {
ImVec2 tsize = ImGui::CalcTextSize(str.c_str(), nullptr, false, size);

    return {pos.x - tsize.x / 2, pos.y - tsize.y / 2};
}

#define drawText(str, size, pos, col) AddText(0, size, Centre(pos, str, size), col, str);

static ImVec2 ToImVec2(const std::vector<float>& arr) { return ImVec2(arr[0], arr[1]); }
