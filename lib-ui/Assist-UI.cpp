#include "Assist-UI.h"

// Use to visually divide panels into named sections.
// --------------------------------------------------
void SectionHeader(const char* label) {
    ImGui::Spacing();
    ImGui::SeparatorText(label);
}

// Call immediately after a widget to attach a hover tooltip.
// ----------------------------------------------------------
void HelpTooltip(const char* description) {
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
        ImGui::SetTooltip("%s", description);
    }
}

// Draws a color editor with a label and optional tooltip. 
// May be called in a loop to create a list of named color swatches.
// -----------------------------------------------------------------
bool ColorRow(const char* label, float color[3], const char* tooltip) {
    bool changed = ImGui::ColorEdit3(label, color);
    if (tooltip) HelpTooltip(tooltip);
    return changed;
}

// Linearly interpolate two ImVec4 colors
// --------------------------------------
ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}

// Clamp a float to [0, 1]
// -----------------------
float Saturate(float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); }

// Brighten or darken a color by a factor
// --------------------------------------
ImVec4 ScaleColor(const ImVec4& c, float factor) {
    return ImVec4(
        Saturate(c.x * factor),
        Saturate(c.y * factor),
        Saturate(c.z * factor),
        c.w
    );
}

// Return color with overridden alpha
// ----------------------------------
ImVec4 WithAlpha(const ImVec4& c, float a) {
    return ImVec4(c.x, c.y, c.z, a);
}