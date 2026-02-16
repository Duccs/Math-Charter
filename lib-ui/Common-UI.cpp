#include "Common-UI.h"
#include "Assist-UI.h"

void LogWindow(bool* show, std::vector<std::string>& logLines) {
    if (!show || !*show) return;

    // ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    // (Uncomment to dock this window to the bottom automatically)

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