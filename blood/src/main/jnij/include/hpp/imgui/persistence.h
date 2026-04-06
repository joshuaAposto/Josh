#pragma once

#include "../imgui.h"
#include "custom_theme.h"

#include "../../json.hpp"
using json = nlohmann::json;

#include <fstream>

const char* STYLE_PATH = "/data/data/" PACKAGE_NAME "/imgui/style.json";
const char* COLOR_PATH = "/data/data/" PACKAGE_NAME "/imgui/color.json";
const char* FEATURE_PATH = "/data/data/" PACKAGE_NAME "/imgui/feature.json";

static int current_theme = 0;
static float Rounding = 6.0f;

static const char* themes[] = { "Custom", "Dark", "Light", "Classic" };

static void switch_theme(int current_theme) {
    switch (current_theme) {
        case 0: StyleColorsCustom(); break;
        case 1: ImGui::StyleColorsDark(); break;
        case 2: ImGui::StyleColorsLight(); break;
        case 3: ImGui::StyleColorsClassic(); break;
    }
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.TouchExtraPadding = ImVec2(10.0f, 10.0f);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(10, 6);
    style.FramePadding = ImVec2(12, 6);
}

#define CRED ImColor(1.0f, 0.0f, 0.0f, 1.0f)
#define CGREEN ImColor(0.0f, 1.0f, 0.0f, 1.0f)
#define CBLUE ImColor(0.0f, 0.0f, 1.0f, 1.0f)
#define CWHITE ImColor(1.0f, 1.0f, 1.0f, 1.0f)

static std::map<std::string, ImColor> colors = {
    {"cESP_Line", CRED},
    {"cESP_LineHidden", CBLUE},
    {"cESP_LineBots", CWHITE},
    {"cESP_LineBotsHidden", CWHITE},
    {"cESP_Skeleton", CRED},
    {"cESP_SkeletonHidden", CGREEN},
    {"cESP_Box", CRED},
    {"cESP_BoxHidden", CGREEN},
    {"cESP_Health", CRED},
    {"cESP_HealthHidden", CGREEN},
    {"cESP_Name", CRED},
    {"cESP_NameHidden", CGREEN},
    {"cESP_Distance", CRED},
    {"cESP_DistanceHidden", CGREEN},
};

static void save_colors() {
    json j;

    for (const auto& pair : colors) j[pair.first] = {pair.second.Value.x, pair.second.Value.y, pair.second.Value.z, pair.second.Value.w};

    std::ofstream out(COLOR_PATH);
    out << j.dump(2);
    out.close();
}

static void load_colors() {
    json j = json::object();

    std::ifstream in(COLOR_PATH);
    if (in.is_open()) {
        try { in >> j; }
        catch(...) {}
        in.close();
    }

    for (auto& pair : colors) {
        if (j.contains(pair.first) && j[pair.first].is_array() && j[pair.first].size() == 4) {
            pair.second.Value.x = j[pair.first][0];
            pair.second.Value.y = j[pair.first][1];
            pair.second.Value.z = j[pair.first][2];
            pair.second.Value.w = j[pair.first][3];
        }
    }
}
static std::map<std::string, bool> _features = {
    {"bBetterTouch", true}
};

static std::map<std::string, bool> features = {
    {"bImguiAutoSave", true},
    {"bAlwaysAutoResize", true},
};

static std::map<std::string, float> features_float = {
    {"fFontScale", 30.0f},
};

static std::map<std::string, std::string> features_string = {
    {"key", ""},
};

static void save_features() {
    json j;

    for (const auto& pair : features) j[pair.first] = pair.second;
    for (const auto& pair : features_float) j[pair.first] = pair.second;
    for (const auto& pair : features_string) j[pair.first] = pair.second;

    std::ofstream out(FEATURE_PATH);
    out << j.dump(2);
    out.close();
}

static void load_features() {
    json j = json::object();

    std::ifstream in(FEATURE_PATH);
    if (in.is_open()) {
        try { in >> j; }
        catch(...) {}
        in.close();
    }

    for (auto& pair : features) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : features_float) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : features_string) if (j.contains(pair.first)) pair.second = j[pair.first];
}

static void save_imgui_style(ImGuiStyle* _style = nullptr) {
    json j;

    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    j["BorderSize"] = style.FrameBorderSize;
    j["ScrollbarSize"] = style.ScrollbarSize;
    j["Rounding"] = Rounding;
    j["Alpha"] = style.Alpha;
    j["ThemeIndex"] = current_theme;

    std::ofstream out(STYLE_PATH);
    out << j.dump(2);
    out.close();
}

static void load_imgui_style(ImGuiStyle* _style = nullptr) {
    json j = json::object();

    std::ifstream in(STYLE_PATH);
    if (in.is_open()) {
        try { in >> j; }
        catch(...) {}
        in.close();
    }

    current_theme = j.value("ThemeIndex", current_theme);

    switch_theme(current_theme);

    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    style.FrameBorderSize = j.value("BorderSize", 0.1f);
    style.ScrollbarSize = j.value("ScrollbarSize", 32.0f);
    style.Alpha = j.value("Alpha", 0.94f);
    
    float rounding = j.value("Rounding", Rounding);
    
    style.FrameRounding = rounding;
    style.WindowRounding = rounding;
    style.ChildRounding = rounding;
    style.PopupRounding = rounding;
    style.ScrollbarRounding = rounding;
    style.GrabRounding = rounding;
    style.LogSliderDeadzone = rounding;
    style.TabRounding = rounding;

    style.WindowPadding = ImVec2(1, 0);
    // style.ItemInnerSpacing.x = 8.0f;
}

