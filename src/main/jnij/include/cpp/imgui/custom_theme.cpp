#pragma once

#include "../../hpp/imgui/custom_theme.h"

void StyleColorsCustom(ImGuiStyle* _style) {
    ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle *style = &ImGui::GetStyle();
style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 225);//字体颜色
style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 255);//背景色
	
style->Colors[ImGuiCol_Button] = ImColor(255,0,0,255);//按钮颜色
style->Colors[ImGuiCol_ButtonHovered] = ImColor(255,10,6,235);//按钮点击后悬停颜色
style->Colors[ImGuiCol_ButtonActive] = ImColor(255,18,8,255);//按钮点击颜色一
	
style->Colors[ImGuiCol_FrameBg] = ImColor(30,30,30,255);//控件内背景色
style->Colors[ImGuiCol_FrameBgActive] = ImColor(30,30,30,255);//控件内点击背景色
style->Colors[ImGuiCol_FrameBgHovered] = ImColor(30,30,30,255);//控件内点击后悬停背景色
style->Colors[ImGuiCol_PopupBg] = ImVec4(255,0,0,60);//弹出窗口背景颜色

style->Colors[ImGuiCol_CheckMark] = ImColor(255,0,0,255);//控件Check勾子颜色
	
style->Colors[ImGuiCol_SliderGrab] = ImColor(255,0,0,255);
style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255,0,0,255);
	
style->Colors[ImGuiCol_Header] = ImColor(255,0,0,255);
style->Colors[ImGuiCol_HeaderHovered] = ImColor(255,0,0,255);
style->Colors[ImGuiCol_HeaderActive] = ImColor(255,0,0,255);

style->Colors[ImGuiCol_ScrollbarBg] = ImColor(42,42,42,225);//滑动条背景颜色
style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(255,0,0,255);//滑动条颜色
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(255,0,0,255);//滚动条点击后悬停
style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255,0,0,255);//滑动条点击
	
style->Colors[ImGuiCol_Separator] = ImColor(80,80,80,255);

style->GrabRounding = 5.0f;
style->GrabMinSize = 35.0f;

style->FrameRounding = 4.0f;//框架舍入
style->FrameBorderSize = 0.0f;//边框大小
style->FramePadding = ImVec2(5, 5);//框架填充

style->ScrollbarSize = 10.0f;//滚动条大小
style->ScrollbarRounding = 5.0f;//滚动条舍入

style->WindowRounding = 4.0f;//窗口舍入
style->WindowBorderSize = 0.0f;//窗口边框大小
style->WindowTitleAlign = ImVec2(0.5, 0.5);//窗口标题对齐
}
