#pragma once

#include "imgui.h"

void SetElegantSkyGlassTheme(){
    ImGuiStyle* style = &ImGui::GetStyle();

style->WindowRounding = 10.0f;
style->FrameRounding = 8.0f;
style->ScrollbarSize = 20.0f;


style->Colors[ImGuiCol_Text]                 = ImColor(245, 245, 245, 255); //
style->Colors[ImGuiCol_TextDisabled]         = ImColor(150, 150, 150, 200);


style->Colors[ImGuiCol_WindowBg]             = ImColor(15, 15, 20, 200);
style->Colors[ImGuiCol_ChildBg]              = ImColor(20, 20, 25, 180);
style->Colors[ImGuiCol_PopupBg]              = ImColor(25, 25, 30, 230);


style->Colors[ImGuiCol_Border]               = ImColor(80, 90, 110, 100); 
style->Colors[ImGuiCol_BorderShadow]         = ImColor(0, 0, 0, 0);


style->Colors[ImGuiCol_FrameBg]              = ImColor(35, 35, 45, 160);
style->Colors[ImGuiCol_FrameBgHovered]       = ImColor(55, 65, 85, 180);
style->Colors[ImGuiCol_FrameBgActive]        = ImColor(70, 85, 115, 200);


style->Colors[ImGuiCol_TitleBg]               = ImColor(25, 25, 35, 190);
style->Colors[ImGuiCol_TitleBgActive]         = ImColor(40, 45, 60, 210);
style->Colors[ImGuiCol_TitleBgCollapsed]      = ImColor(15, 15, 20, 160);

style->Colors[ImGuiCol_MenuBarBg]            = ImColor(25, 28, 35, 180);


style->Colors[ImGuiCol_ScrollbarBg]          = ImColor(20, 20, 25, 150);
style->Colors[ImGuiCol_ScrollbarGrab]        = ImColor(80, 90, 110, 200);
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(110, 125, 150, 220);
style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImColor(140, 160, 190, 240);


style->Colors[ImGuiCol_Separator]            = ImColor(90, 100, 120, 120);
style->Colors[ImGuiCol_SeparatorHovered]     = ImColor(140, 160, 190, 180);
style->Colors[ImGuiCol_SeparatorActive]      = ImColor(170, 190, 220, 200);


style->Colors[ImGuiCol_CheckMark]            = ImColor(200, 220, 255, 255);

style->Colors[ImGuiCol_SliderGrab]           = ImColor(100, 120, 150, 200);
style->Colors[ImGuiCol_SliderGrabActive]     = ImColor(150, 175, 210, 230);


style->Colors[ImGuiCol_Button]               = ImColor(45, 50, 65, 180);
style->Colors[ImGuiCol_ButtonHovered]        = ImColor(70, 85, 115, 210);
style->Colors[ImGuiCol_ButtonActive]         = ImColor(95, 115, 155, 240);


style->Colors[ImGuiCol_Header]                = ImColor(50, 60, 80, 160);
style->Colors[ImGuiCol_HeaderHovered]         = ImColor(70, 90, 120, 190);
style->Colors[ImGuiCol_HeaderActive]          = ImColor(90, 120, 160, 220);


style->Colors[ImGuiCol_ResizeGrip]           = ImColor(100, 120, 150, 150);
style->Colors[ImGuiCol_ResizeGripHovered]    = ImColor(140, 165, 200, 190);
style->Colors[ImGuiCol_ResizeGripActive]     = ImColor(180, 205, 240, 230);


style->Colors[ImGuiCol_Tab]                  = ImColor(30, 35, 45, 180);
style->Colors[ImGuiCol_TabHovered]           = ImColor(60, 75, 100, 210);
style->Colors[ImGuiCol_TabActive]            = ImColor(80, 100, 135, 240);
style->Colors[ImGuiCol_TabUnfocused]         = ImColor(25, 28, 35, 160);
style->Colors[ImGuiCol_TabUnfocusedActive]   = ImColor(50, 60, 80, 200);


style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.80f, 0.85f, 0.95f, 0.90f);
style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.95f, 0.98f, 1.00f, 1.00f);
style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.70f, 0.80f, 0.95f, 0.85f);
style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.93f, 1.00f, 1.00f);


style->Colors[ImGuiCol_TableHeaderBg]        = ImColor(40, 45, 60, 180);
style->Colors[ImGuiCol_TableBorderStrong]    = ImColor(90, 105, 130, 150);
style->Colors[ImGuiCol_TableBorderLight]     = ImColor(60, 70, 90, 100);
style->Colors[ImGuiCol_TableRowBg]           = ImColor(25, 28, 35, 100);
style->Colors[ImGuiCol_TableRowBgAlt]        = ImColor(35, 40, 50, 120);

style->Colors[ImGuiCol_TextSelectedBg]       = ImColor(70, 100, 140, 160);
style->Colors[ImGuiCol_DragDropTarget]       = ImColor(150, 190, 240, 220);

style->Colors[ImGuiCol_NavHighlight]         = ImColor(120, 160, 210, 200);
style->Colors[ImGuiCol_NavWindowingHighlight]= ImColor(200, 210, 230, 200);
style->Colors[ImGuiCol_NavWindowingDimBg]    = ImColor(0, 0, 0, 150);
style->Colors[ImGuiCol_ModalWindowDimBg]     = ImColor(0, 0, 0, 200);
}


void SetElegantBlackAndRedTheme(){
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowRounding = 10.0f;
    style->FrameRounding = 8.0f;
    style->ScrollbarSize = 20;

    

style->Colors[ImGuiCol_Text]                 = ImColor(210, 210, 210, 255);
style->Colors[ImGuiCol_TextDisabled]         = ImColor(95, 95, 95, 255);

style->Colors[ImGuiCol_WindowBg]             = ImColor(8, 8, 8, 255);
style->Colors[ImGuiCol_ChildBg]              = ImColor(10, 10, 10, 255);
style->Colors[ImGuiCol_PopupBg]              = ImColor(12, 12, 12, 255);

style->Colors[ImGuiCol_Border]               = ImColor(30, 30, 30, 255);
style->Colors[ImGuiCol_BorderShadow]         = ImColor(0, 0, 0, 0);

style->Colors[ImGuiCol_FrameBg]              = ImColor(16, 16, 16, 255);
style->Colors[ImGuiCol_FrameBgHovered]       = ImColor(22, 22, 22, 255);
style->Colors[ImGuiCol_FrameBgActive]        = ImColor(28, 28, 28, 255);

style->Colors[ImGuiCol_TitleBg]               = ImColor(6, 6, 6, 255);
style->Colors[ImGuiCol_TitleBgActive]         = ImColor(10, 10, 10, 255);
style->Colors[ImGuiCol_TitleBgCollapsed]      = ImColor(6, 6, 6, 200);

style->Colors[ImGuiCol_MenuBarBg]            = ImColor(10, 10, 10, 255);

style->Colors[ImGuiCol_ScrollbarBg]          = ImColor(6, 6, 6, 255);
style->Colors[ImGuiCol_ScrollbarGrab]        = ImColor(26, 26, 26, 255);
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(34, 34, 34, 255);
style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImColor(42, 42, 42, 255);

style->Colors[ImGuiCol_Separator]            = ImColor(28, 28, 28, 255);
style->Colors[ImGuiCol_SeparatorHovered]     = ImColor(36, 36, 36, 255);
style->Colors[ImGuiCol_SeparatorActive]      = ImColor(44, 44, 44, 255);

style->Colors[ImGuiCol_CheckMark]            = ImColor(160, 160, 160, 255);

style->Colors[ImGuiCol_SliderGrab]           = ImColor(60, 60, 60, 255);
style->Colors[ImGuiCol_SliderGrabActive]     = ImColor(90, 90, 90, 255);

style->Colors[ImGuiCol_Button]               = ImColor(18, 18, 18, 255);
style->Colors[ImGuiCol_ButtonHovered]        = ImColor(26, 26, 26, 255);
style->Colors[ImGuiCol_ButtonActive]         = ImColor(34, 34, 34, 255);

style->Colors[ImGuiCol_Header]               = ImColor(20, 20, 20, 255);
style->Colors[ImGuiCol_HeaderHovered]        = ImColor(28, 28, 28, 255);
style->Colors[ImGuiCol_HeaderActive]         = ImColor(36, 36, 36, 255);

style->Colors[ImGuiCol_ResizeGrip]           = ImColor(26, 26, 26, 255);
style->Colors[ImGuiCol_ResizeGripHovered]    = ImColor(36, 36, 36, 255);
style->Colors[ImGuiCol_ResizeGripActive]     = ImColor(48, 48, 48, 255);

style->Colors[ImGuiCol_Tab]                  = ImColor(14, 14, 14, 255);
style->Colors[ImGuiCol_TabHovered]           = ImColor(22, 22, 22, 255);
style->Colors[ImGuiCol_TabActive]            = ImColor(30, 30, 30, 255);
style->Colors[ImGuiCol_TabUnfocused]         = ImColor(12, 12, 12, 255);
style->Colors[ImGuiCol_TabUnfocusedActive]   = ImColor(20, 20, 20, 255);

style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

style->Colors[ImGuiCol_TableHeaderBg]        = ImColor(14, 14, 14, 255);
style->Colors[ImGuiCol_TableBorderStrong]    = ImColor(28, 28, 28, 255);
style->Colors[ImGuiCol_TableBorderLight]     = ImColor(20, 20, 20, 255);
style->Colors[ImGuiCol_TableRowBg]           = ImColor(12, 12, 12, 255);
style->Colors[ImGuiCol_TableRowBgAlt]        = ImColor(255, 255, 255, 6);

style->Colors[ImGuiCol_TextSelectedBg]       = ImColor(60, 60, 60, 120);
style->Colors[ImGuiCol_DragDropTarget]       = ImColor(90, 90, 90, 200);

style->Colors[ImGuiCol_NavHighlight]         = ImColor(80, 80, 80, 255);
style->Colors[ImGuiCol_NavWindowingHighlight]= ImColor(100, 100, 100, 180);
style->Colors[ImGuiCol_NavWindowingDimBg]    = ImColor(0, 0, 0, 120);
style->Colors[ImGuiCol_ModalWindowDimBg]     = ImColor(0, 0, 0, 160);
}


void SetDripClientTheme(){
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowRounding = 10.0f;
    style->FrameRounding = 8.0f;
    style->ScrollbarSize = 20;

style->Colors[ImGuiCol_Text]                 = ImColor(220, 220, 220, 255);
style->Colors[ImGuiCol_TextDisabled]         = ImColor(110, 110, 110, 255);

style->Colors[ImGuiCol_WindowBg]             = ImColor(7, 7, 9, 255);
style->Colors[ImGuiCol_ChildBg]              = ImColor(10, 10, 14, 255);
style->Colors[ImGuiCol_PopupBg]              = ImColor(12, 12, 16, 255);

style->Colors[ImGuiCol_Border]               = ImColor(60, 20, 90, 120);
style->Colors[ImGuiCol_BorderShadow]         = ImColor(0, 0, 0, 0);

style->Colors[ImGuiCol_FrameBg]              = ImColor(18, 16, 22, 255);
style->Colors[ImGuiCol_FrameBgHovered]       = ImColor(40, 20, 60, 255);
style->Colors[ImGuiCol_FrameBgActive]        = ImColor(55, 25, 85, 255);

style->Colors[ImGuiCol_TitleBg]              = ImColor(10, 8, 14, 255);
style->Colors[ImGuiCol_TitleBgActive]        = ImColor(30, 10, 50, 255);
style->Colors[ImGuiCol_TitleBgCollapsed]     = ImColor(10, 8, 14, 200);

style->Colors[ImGuiCol_MenuBarBg]            = ImColor(12, 10, 16, 255);

style->Colors[ImGuiCol_ScrollbarBg]          = ImColor(8, 8, 10, 255);
style->Colors[ImGuiCol_ScrollbarGrab]        = ImColor(70, 35, 100, 255);
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(95, 45, 135, 255);
style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImColor(120, 55, 170, 255);

style->Colors[ImGuiCol_Separator]            = ImColor(45, 20, 70, 255);
style->Colors[ImGuiCol_SeparatorHovered]     = ImColor(70, 30, 110, 255);
style->Colors[ImGuiCol_SeparatorActive]      = ImColor(95, 40, 140, 255);

style->Colors[ImGuiCol_CheckMark]            = ImColor(170, 100, 255, 255);

style->Colors[ImGuiCol_SliderGrab]           = ImColor(120, 60, 190, 255);
style->Colors[ImGuiCol_SliderGrabActive]     = ImColor(170, 90, 255, 255);

style->Colors[ImGuiCol_Button]               = ImColor(40, 18, 60, 255);
style->Colors[ImGuiCol_ButtonHovered]        = ImColor(75, 35, 120, 255);
style->Colors[ImGuiCol_ButtonActive]         = ImColor(110, 50, 170, 255);

style->Colors[ImGuiCol_Header]               = ImColor(35, 16, 55, 255);
style->Colors[ImGuiCol_HeaderHovered]        = ImColor(70, 30, 110, 255);
style->Colors[ImGuiCol_HeaderActive]         = ImColor(100, 45, 150, 255);

style->Colors[ImGuiCol_ResizeGrip]           = ImColor(70, 30, 110, 120);
style->Colors[ImGuiCol_ResizeGripHovered]    = ImColor(110, 50, 170, 200);
style->Colors[ImGuiCol_ResizeGripActive]     = ImColor(150, 70, 230, 255);

style->Colors[ImGuiCol_Tab]                  = ImColor(20, 14, 28, 255);
style->Colors[ImGuiCol_TabHovered]           = ImColor(70, 30, 110, 255);
style->Colors[ImGuiCol_TabActive]            = ImColor(95, 40, 150, 255);
style->Colors[ImGuiCol_TabUnfocused]         = ImColor(16, 12, 22, 255);
style->Colors[ImGuiCol_TabUnfocusedActive]   = ImColor(50, 25, 75, 255);

style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.8f, 0.6f, 1.0f, 1.0f);
style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.9f, 0.7f, 1.0f, 1.0f);
style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.85f, 0.6f, 1.0f, 1.0f);

style->Colors[ImGuiCol_TableHeaderBg]        = ImColor(22, 14, 30, 255);
style->Colors[ImGuiCol_TableBorderStrong]    = ImColor(60, 25, 95, 255);
style->Colors[ImGuiCol_TableBorderLight]     = ImColor(40, 18, 65, 255);
style->Colors[ImGuiCol_TableRowBg]           = ImColor(12, 10, 16, 255);
style->Colors[ImGuiCol_TableRowBgAlt]        = ImColor(160, 90, 255, 20);

style->Colors[ImGuiCol_TextSelectedBg]       = ImColor(140, 80, 255, 120);
style->Colors[ImGuiCol_DragDropTarget]       = ImColor(200, 120, 255, 200);

style->Colors[ImGuiCol_NavHighlight]         = ImColor(150, 80, 255, 255);
style->Colors[ImGuiCol_NavWindowingHighlight]= ImColor(170, 100, 255, 200);
style->Colors[ImGuiCol_NavWindowingDimBg]    = ImColor(0, 0, 0, 120);
style->Colors[ImGuiCol_ModalWindowDimBg]     = ImColor(0, 0, 0, 160);
}