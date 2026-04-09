#pragma once

// Config structure
struct ESPConfig {
    bool Line = false;
    bool Box = false;
    bool Health = false;
    bool Info = false;
};

struct GameConfig {
    ESPConfig ESP;
} Config;

// Global variables
bool Enable = false;
ImVec4 menu = ImVec4(0.2f, 0.6f, 1.0f, 1.0f); // Warna menu (RGBA)
