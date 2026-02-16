#ifndef _MENUBAR_UI_H_
#define _MENUBAR_UI_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <fstream>

struct PreferencesState {
    float colorA[3] = {0.4f, 0.7f, 1.0f};
    float colorB[3] = {1.0f, 0.3f, 0.3f};
    float bgColor[3] = {0.12f, 0.12f, 0.14f};
    int debugRadio = 0;
    int warnRadio = 0;
    int uiScaleSlider = 50;
};

struct MenuBarState {
    bool showLog = false;
    bool showPreferences = false;
    bool showDemoWindow = false;
    bool showMetrics = false;
};

void InitializeMenuBar(GLFWwindow* window, std::vector<std::string>& loglines, MenuBarState* state);

void PreferencesWindow(bool* show, PreferencesState* state);

void LogWindow(bool* show, std::vector<std::string>& logLines);

void ApplyPreferencesStyle(const PreferencesState* state);

bool SavePreferences(const char* filepath, const PreferencesState* state);
bool LoadPreferences(const char* filepath, PreferencesState* state);

#endif /* _MENUBAR_UI_H_ */