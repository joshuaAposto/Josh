// gui_helpers.h
#pragma once

#include <string>

// Helper functions untuk GUI
namespace GuiHelpers {
    void drawText(const std::string& text, int x, int y);
    void drawRect(int x, int y, int width, int height);
    void showNotification(const std::string& message);
}
