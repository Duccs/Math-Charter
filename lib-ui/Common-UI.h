#ifndef _COMMON_UI_H_
#define _COMMON_UI_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <fstream>

struct DockLayout {
    ImGuiID right_upper;
    ImGuiID right_lower;
};

// Only sets up lower right and upper right docks.
DockLayout SetupDockingLayout(ImGuiID dockspace_id);

void LogWindow(bool* show, std::vector<std::string>& logLines);

#endif /* _COMMON_UI_H_ */