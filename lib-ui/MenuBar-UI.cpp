#include "MenuBar-UI.h"
#include "Assist-UI.h"

void InitializeMenuBar(GLFWwindow* window, std::vector<std::string>& logLines, MenuBarState* state) {
    if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New"))          { logLines.push_back("[File] New"); }
                if (ImGui::MenuItem("Open", "Ctrl+O")) { logLines.push_back("[File] Open"); }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { logLines.push_back("[File] Save"); }
                if (ImGui::MenuItem("Save As"))       { logLines.push_back("[File] Save As"); }
                if (ImGui::MenuItem("Export"))       { logLines.push_back("[File] Export"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {
                    glfwSetWindowShouldClose(window, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Graph Viewport",    nullptr, &state->showViewport);
                ImGui::MenuItem("Graph Controls",    nullptr, &state->showGraphControls);
                ImGui::Separator();
                ImGui::MenuItem("Log Window",        nullptr, &state->showLog);
                ImGui::MenuItem("Preferences",       nullptr, &state->showPreferences);
                ImGui::Separator();
                ImGui::MenuItem("ImGui Demo Window", nullptr, &state->showDemoWindow);
                ImGui::MenuItem("Metrics / Debug",   nullptr, &state->showMetrics);

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
}