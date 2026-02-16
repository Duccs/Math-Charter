#include "Preferences-UI.h"
#include "Assist-UI.h"

void PreferencesWindow(bool* show, PreferencesState* state) {
    if (!show || !*show) return;

    ImGui::Begin("Preferences", show);

        ImGui::NewLine();

        // UI elements
        SectionHeader("UI Elements");

        ImGui::SliderInt("UI Scale", &state->uiScaleSlider, 0, 100);
        HelpTooltip("Adjusts the overall size of the interface.");
        if (ImGui::Button("Apply"))
            ChangeUIScale(state);

        ImGui::NewLine();

        // Font selection
        ImGui::Text("Font:");
        ImGui::SameLine();
        if (state->fontCount > 0) {
            ImGui::Combo("##FontSelect", &state->fontCombo, state->fontNames, state->fontCount);
        } else {
            ImGui::TextDisabled("(no fonts loaded)");
        }

        ImGui::NewLine();

        // Colors
        SectionHeader("System Colors");

        // Reusable ColorRow helper — easy to call in a loop for a palette
        ColorRow("Primary",   state->colorA, "Main accent color");
        ColorRow("Secondary", state->colorB, "Highlight / warning color");
        ColorRow("Background", state->bgColor, "Background color for windows and panels");

        ImGui::NewLine();
        ImGui::Text("Presets:"); ImGui::SameLine();
        const char* presets[] = {"Coral Blue", "Coral Blue (Light)"};
        if (ImGui::Combo("##ColorPresets", &state->colorPresetCombo, presets, 2)) {
            // Apply the selected preset
            if (state->colorPresetCombo == 0) {  // Coral Blue
                state->colorA[0] = red(248); state->colorA[1] = green(112); state->colorA[2] = blue(96);
                state->colorB[0] = red(205); state->colorB[1] = green(215); state->colorB[2] = blue(214);
                state->bgColor[0] = red(16); state->bgColor[1] = green(37); state->bgColor[2] = blue(66);
            } else if (state->colorPresetCombo == 1) {  // Coral Blue (Light)
                state->colorA[0] = red(248); state->colorA[1] = green(112); state->colorA[2] = blue(96);
                state->colorB[0] = red(16); state->colorB[1] = green(37); state->colorB[2] = blue(66);
                state->bgColor[0] = red(205); state->bgColor[1] = green(215); state->bgColor[2] = blue(214);
            }
            ApplyPreferencesStyle(state);
        }

        ImGui::NewLine();

        // Collapsing sections
        SectionHeader("Collapsible Sections");

        if (ImGui::CollapsingHeader("Advanced Settings")) {
                ImGui::NewLine();
                ImGui::Text("Debug mode:"); ImGui::SameLine();
                ImGui::RadioButton("OFF##debug", &state->debugRadio, 0); ImGui::SameLine();
                ImGui::RadioButton("ON##debug", &state->debugRadio, 1); 

                ImGui::Text("Warn mode:"); ImGui::SameLine();
                ImGui::RadioButton("OFF##warn", &state->warnRadio, 0); ImGui::SameLine();
                ImGui::RadioButton("ON##warn", &state->warnRadio, 1); 
        }

        if (ImGui::CollapsingHeader("About")) {
            ImGui::TextWrapped(
                "Math-Charter\n"
                "Built as capstone senior project for CS-4600\n");
        }

        ImGui::NewLine();

        // Tabs
        SectionHeader("Tabs");

        if (ImGui::BeginTabBar("MainTabs")) {
            if (ImGui::BeginTabItem("Overview")) {
                ImGui::Text("This is the Overview tab.");
                ImGui::BulletText("Tabs keep related content grouped.");
                ImGui::BulletText("Each tab can hold its own widgets.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings")) {
                static bool opt1 = true, opt2 = false;
                ImGui::Checkbox("Enable feature X", &opt1);
                ImGui::Checkbox("Enable feature Y", &opt2);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Data")) {
                ImGui::Text("Display data, tables, or graphs here.");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End(); // End "Preferences"

}

void ChangeUIScale(const PreferencesState* state) {
    // Apply UI scale globally (ImGui applies this every frame)
    ImGui::GetIO().FontGlobalScale = 1.0f + (state->uiScaleSlider / 100.0f);
}

void ApplyPreferences(const PreferencesState* state) {


    // Apply color scheme to ImGui style
    ApplyPreferencesStyle(state);
}

void ApplyPreferencesStyle(const PreferencesState* state){
    ImGuiStyle& style = ImGui::GetStyle();

    // Base palette from preferences
    ImVec4 primary(state->colorA[0], state->colorA[1], state->colorA[2], 1.0f);
    ImVec4 accent(state->colorB[0], state->colorB[1], state->colorB[2], 1.0f);
    ImVec4 bg(state->bgColor[0], state->bgColor[1], state->bgColor[2], 1.0f);

    // Derived variants
    ImVec4 primaryDark   = ScaleColor(primary, 0.65f);
    ImVec4 primaryLight  = ScaleColor(primary, 1.30f);
    ImVec4 accentDark    = ScaleColor(accent,  0.65f);
    ImVec4 accentLight   = ScaleColor(accent,  1.30f);
    ImVec4 bgLight       = ScaleColor(bg,      1.40f); // slightly lighter panel bg
    ImVec4 bgLighter     = ScaleColor(bg,      1.80f); // for borders / separators
    ImVec4 bgDark        = ScaleColor(bg,      0.75f); // darker child panels

    // Adaptive text color based on background luminance
    float luminance = 0.299f * bg.x + 0.587f * bg.y + 0.114f * bg.z;
    ImVec4 textColor, textDimmed;
    if (luminance < 0.35f) {
        // Dark background → white text
        textColor  = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
        textDimmed = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
    } else if (luminance > 0.65f) {
        // Light background → black text
        textColor  = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
        textDimmed = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    } else {
        // Mid-range background → pick whichever has more contrast
        // Use dark text if bg is brighter than mid, else light text
        if (luminance > 0.5f) {
            textColor  = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
            textDimmed = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        } else {
            textColor  = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
            textDimmed = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        }
    }

    //  Text
    style.Colors[ImGuiCol_Text]                  = textColor;
    style.Colors[ImGuiCol_TextDisabled]          = textDimmed;

    //  Backgrounds
    style.Colors[ImGuiCol_WindowBg]              = bg;
    style.Colors[ImGuiCol_ChildBg]               = bgDark;
    style.Colors[ImGuiCol_PopupBg]               = ScaleColor(bg, 0.90f);

    //  Borders
    style.Colors[ImGuiCol_Border]                = WithAlpha(bgLighter, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

    //  Frames (input fields, checkboxes, combo boxes, sliders bg)
    style.Colors[ImGuiCol_FrameBg]               = WithAlpha(primaryDark, 0.35f);
    style.Colors[ImGuiCol_FrameBgHovered]        = WithAlpha(primary,     0.45f);
    style.Colors[ImGuiCol_FrameBgActive]         = WithAlpha(primary,     0.65f);

    //  Title bars
    style.Colors[ImGuiCol_TitleBg]               = primaryDark;
    style.Colors[ImGuiCol_TitleBgActive]         = primary;
    style.Colors[ImGuiCol_TitleBgCollapsed]      = WithAlpha(primaryDark, 0.50f);

    //  Menu bar
    style.Colors[ImGuiCol_MenuBarBg]             = bgLight;

    //  Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg]           = WithAlpha(bg, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = bgLighter;
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ScaleColor(bgLighter, 1.20f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = primary;

    //  Buttons
    style.Colors[ImGuiCol_Button]                = primary;
    style.Colors[ImGuiCol_ButtonHovered]         = primaryLight;
    style.Colors[ImGuiCol_ButtonActive]          = accent;

    //  Sliders
    style.Colors[ImGuiCol_SliderGrab]            = primaryLight;
    style.Colors[ImGuiCol_SliderGrabActive]      = accent;

    //  Headers (CollapsingHeader, Selectable, TreeNode)
    style.Colors[ImGuiCol_Header]                = WithAlpha(primary, 0.40f);
    style.Colors[ImGuiCol_HeaderHovered]         = WithAlpha(primary, 0.65f);
    style.Colors[ImGuiCol_HeaderActive]          = WithAlpha(primary, 0.85f);

    //  Checkmark / radio
    style.Colors[ImGuiCol_CheckMark]             = accent;

    //  Separators
    style.Colors[ImGuiCol_Separator]             = WithAlpha(bgLighter, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]      = primary;
    style.Colors[ImGuiCol_SeparatorActive]       = accent;

    //  Resize grip
    style.Colors[ImGuiCol_ResizeGrip]            = WithAlpha(primary, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = WithAlpha(primary, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = WithAlpha(accent,  0.90f);

    //  Tabs
    style.Colors[ImGuiCol_Tab]                   = primaryDark;
    style.Colors[ImGuiCol_TabHovered]            = primary;
    style.Colors[ImGuiCol_TabSelected]           = primary;
    style.Colors[ImGuiCol_TabDimmed]             = WithAlpha(primaryDark, 0.60f);
    style.Colors[ImGuiCol_TabDimmedSelected]     = WithAlpha(primary,     0.70f);

    //  Tables
    style.Colors[ImGuiCol_TableHeaderBg]         = bgLight;
    style.Colors[ImGuiCol_TableBorderStrong]     = WithAlpha(bgLighter, 0.60f);
    style.Colors[ImGuiCol_TableBorderLight]      = WithAlpha(bgLighter, 0.30f);
    style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_TableRowBgAlt]         = WithAlpha(bgLight, 0.10f);

    //  Drag-and-drop target highlight
    style.Colors[ImGuiCol_DragDropTarget]        = accent;

    //  Text selection / input cursor
    style.Colors[ImGuiCol_TextSelectedBg]        = WithAlpha(primary, 0.40f);
    style.Colors[ImGuiCol_NavHighlight]          = primary;
}

// Preferences persistence
// -----------------------
bool SavePreferences(const char* filepath, const PreferencesState* state) {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;

    out << "colorA_r=" << state->colorA[0] << "\n";
    out << "colorA_g=" << state->colorA[1] << "\n";
    out << "colorA_b=" << state->colorA[2] << "\n";

    out << "colorB_r=" << state->colorB[0] << "\n";
    out << "colorB_g=" << state->colorB[1] << "\n";
    out << "colorB_b=" << state->colorB[2] << "\n";

    out << "bgColor_r=" << state->bgColor[0] << "\n";
    out << "bgColor_g=" << state->bgColor[1] << "\n";
    out << "bgColor_b=" << state->bgColor[2] << "\n";

    out << "debugRadio=" << state->debugRadio << "\n";
    out << "warnRadio=" << state->warnRadio << "\n";
    out << "uiScaleSlider=" << state->uiScaleSlider << "\n";
    out << "fontCombo=" << state->fontCombo << "\n";

    return out.good();
}

bool LoadPreferences(const char* filepath, PreferencesState* state) {
    std::ifstream in(filepath);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if      (key == "colorA_r")     state->colorA[0]    = std::stof(val);
        else if (key == "colorA_g")     state->colorA[1]    = std::stof(val);
        else if (key == "colorA_b")     state->colorA[2]    = std::stof(val);
        else if (key == "colorB_r")     state->colorB[0]    = std::stof(val);
        else if (key == "colorB_g")     state->colorB[1]    = std::stof(val);
        else if (key == "colorB_b")     state->colorB[2]    = std::stof(val);
        else if (key == "bgColor_r")    state->bgColor[0]   = std::stof(val);
        else if (key == "bgColor_g")    state->bgColor[1]   = std::stof(val);
        else if (key == "bgColor_b")    state->bgColor[2]   = std::stof(val);
        else if (key == "debugRadio")   state->debugRadio   = std::stoi(val);
        else if (key == "warnRadio")    state->warnRadio    = std::stoi(val);
        else if (key == "uiScaleSlider") state->uiScaleSlider = std::stoi(val);
        else if (key == "fontCombo")      state->fontCombo     = std::stoi(val);
    }

    return true;
}
