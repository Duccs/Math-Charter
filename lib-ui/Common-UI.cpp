#include "Common-UI.h"
#include "Assist-UI.h"
#include <imgui_internal.h>

DockLayout SetupDockingLayout(ImGuiID dockspace_id) {
    DockLayout layout;
    
    // Clear any existing layout
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
    
    // Split off right side (25% width)
    ImGuiID dock_id_right;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, &dock_id_right, nullptr);
    
    // Split the right side into upper (50%) and lower (50%) sections
    ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.50f, &layout.right_lower, &layout.right_upper);
    
    ImGui::DockBuilderFinish(dockspace_id);
    
    return layout;
}

void LogWindow(bool* show, std::vector<std::string>& logLines) {
    if (!show || !*show) return;

    ImGui::Begin("Log", show);

    if (ImGui::Button("Add Entry")) {
        logLines.push_back("[Log] Entry #" + std::to_string(logLines.size()));
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        logLines.clear();
    }

    ImGui::Separator();
    ImGui::BeginChild("LogScroll", ImVec2(0, 0), ImGuiChildFlags_None,
                        ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line : logLines)
        ImGui::TextUnformatted(line.c_str());
    // Auto-scroll to bottom when new content added
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::End(); // End "Log"
        
}