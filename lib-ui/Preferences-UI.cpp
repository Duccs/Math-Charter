#include "Preferences-UI.h"
#include "Assist-UI.h"

void PreferencesWindow(bool* show, PreferencesState* state) {
    if (!show || !*show) return;

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::SetNextWindowPos(ImVec2(2112, 35), ImGuiCond_FirstUseEver); 
    ImGui::SetNextWindowSize(ImVec2(448, 891), ImGuiCond_FirstUseEver);  // Width: 800px, Height: 200px
    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("Preferences", show);

        ImGui::NewLine();

        // UI elements
        SectionHeader("Font settings");

        ImGui::SliderInt("Font Scale", &state->fontScaleSlider, 0, 100);
        HelpTooltip("Adjusts the size of the font and UI elements.");
        if (ImGui::Button("Apply##FontScale"))
            ChangeFontScale(state);

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

        ColorRow("Primary",    state->primaryColor, "Buttons, title bars, headers, tabs");
        ColorRow("Accent",     state->accentColor,  "Active/pressed states, checkmarks");
        ColorRow("Background", state->bgColor,       "Window backgrounds, panels");
        ColorRow("Border",     state->borderColor,   "Borders, separators, table lines");
        ColorRow("Hover",      state->hoverColor,    "Hovered buttons, frames, headers");

        ImGui::NewLine();
        ImGui::Checkbox("Auto text color", &state->autoTextColor);
        HelpTooltip("When enabled, text color is computed automatically from background luminance.");
        if (!state->autoTextColor) {
            ColorRow("Text", state->textColor, "Manual text color");
        }

        ImGui::NewLine();
        ImGui::Text("Presets:"); ImGui::SameLine();
        const char* presets[] = {"Coral Blue", "Coral Blue (Light)", "Yogurt Purple", "Mandarin Spike"};
        if (ImGui::Combo("##ColorPresets", &state->colorPresetCombo, presets, 4)) {
            // Apply the selected preset
            if (state->colorPresetCombo == 0) {  // Coral Blue
                state->primaryColor[0] = red(248); state->primaryColor[1] = green(112); state->primaryColor[2] = blue(96);
                state->accentColor[0]  = red(205); state->accentColor[1]  = green(215); state->accentColor[2]  = blue(214);
                state->bgColor[0]      = red(16);  state->bgColor[1]      = green(37);  state->bgColor[2]      = blue(66);
                state->borderColor[0]  = red(40);  state->borderColor[1]  = green(60);  state->borderColor[2]  = blue(90);
                state->hoverColor[0]   = red(255); state->hoverColor[1]   = green(140); state->hoverColor[2]   = blue(120);
                state->autoTextColor = true;
            } else if (state->colorPresetCombo == 1) {  // Coral Blue (Light)
                state->primaryColor[0] = red(248); state->primaryColor[1] = green(112); state->primaryColor[2] = blue(96);
                state->accentColor[0]  = red(16);  state->accentColor[1]  = green(37);  state->accentColor[2]  = blue(66);
                state->bgColor[0]      = red(205); state->bgColor[1]      = green(215); state->bgColor[2]      = blue(214);
                state->borderColor[0]  = red(160); state->borderColor[1]  = green(170); state->borderColor[2]  = blue(175);
                state->hoverColor[0]   = red(255); state->hoverColor[1]   = green(140); state->hoverColor[2]   = blue(120);
                state->autoTextColor = true;
            } else if (state->colorPresetCombo == 2) {  // Yogurt Purple
                state->primaryColor[0] = red(190); state->primaryColor[1] = green(166); state->primaryColor[2] = blue(255);
                state->accentColor[0]  = red(153); state->accentColor[1]  = green(20);  state->accentColor[2]  = blue(149);
                state->bgColor[0]      = red(240); state->bgColor[1]      = green(240); state->bgColor[2]      = blue(240);
                state->borderColor[0]  = red(190); state->borderColor[1]  = green(190); state->borderColor[2]  = blue(200);
                state->hoverColor[0]   = red(210); state->hoverColor[1]   = green(190); state->hoverColor[2]   = blue(255);
                state->autoTextColor = true;
            } else if (state->colorPresetCombo == 3) {  // Mandarin Spike
                state->primaryColor[0] = red(255); state->primaryColor[1] = green(128); state->primaryColor[2] = blue(69);
                state->accentColor[0]  = red(255); state->accentColor[1]  = green(69);  state->accentColor[2]  = blue(0);
                state->bgColor[0]      = red(56);  state->bgColor[1]      = green(56);  state->bgColor[2]      = blue(56);
                state->borderColor[0]  = red(90);  state->borderColor[1]  = green(90);  state->borderColor[2]  = blue(90);
                state->hoverColor[0]   = red(255); state->hoverColor[1]   = green(160); state->hoverColor[2]   = blue(100);
                state->autoTextColor = true;
            }
            ApplyPreferencesStyle(state);
        }

        ImGui::NewLine();

        // Collapsing sections
        SectionHeader("Collapsible Sections");

        if (ImGui::CollapsingHeader("Advanced Settings")) {
                ImGui::NewLine();
                ImGui::Checkbox("Enable Advanced Settings", &state->showAdvancedSettings);

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
                ImGui::BulletText("Bullet point #1");
                ImGui::BulletText("Bullet point #2");
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

void ChangeFontScale(const PreferencesState* state) {
    // 0.5x..2.0x  (50 = 1.0x)
    float fontScale = 0.8f + (state->fontScaleSlider / 100.0f) * 1.5f;
    ImGui::GetIO().FontGlobalScale = fontScale;
}


void ApplyPreferences(const PreferencesState* state) {


    // Apply color scheme to ImGui style
    ApplyPreferencesStyle(state);
}

void ApplyPreferencesStyle(const PreferencesState* state){
    ImGuiStyle& style = ImGui::GetStyle();

    // Base palette from preferences
    ImVec4 primary(state->primaryColor[0], state->primaryColor[1], state->primaryColor[2], 1.0f);
    ImVec4 accent(state->accentColor[0],   state->accentColor[1],  state->accentColor[2],  1.0f);
    ImVec4 bg(state->bgColor[0],           state->bgColor[1],      state->bgColor[2],      1.0f);
    ImVec4 border(state->borderColor[0],   state->borderColor[1],  state->borderColor[2],  1.0f);
    ImVec4 hover(state->hoverColor[0],     state->hoverColor[1],   state->hoverColor[2],   1.0f);

    // Derived variants
    ImVec4 primaryDark   = ScaleColor(primary, 0.65f);
    ImVec4 accentDark    = ScaleColor(accent,  0.65f);
    ImVec4 bgLight       = ScaleColor(bg,      1.40f); // slightly lighter panel bg
    ImVec4 bgDark        = ScaleColor(bg,      0.75f); // darker child panels
    ImVec4 borderLight   = ScaleColor(border,  0.60f); // subtle border variant

    // Text color: auto-compute from luminance, or use manual override
    ImVec4 textColor, textDimmed;
    if (state->autoTextColor) {
        float luminance = 0.299f * bg.x + 0.587f * bg.y + 0.114f * bg.z;
        if (luminance < 0.35f) {
            textColor  = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
            textDimmed = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        } else if (luminance > 0.65f) {
            textColor  = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
            textDimmed = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        } else if (luminance > 0.5f) {
            textColor  = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
            textDimmed = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        } else {
            textColor  = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
            textDimmed = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        }
    } else {
        textColor  = ImVec4(state->textColor[0], state->textColor[1], state->textColor[2], 1.0f);
        textDimmed = ScaleColor(textColor, 0.55f);
    }

    //  Text
    style.Colors[ImGuiCol_Text]                  = textColor;
    style.Colors[ImGuiCol_TextDisabled]          = textDimmed;

    //  Backgrounds
    style.Colors[ImGuiCol_WindowBg]              = bg;
    style.Colors[ImGuiCol_ChildBg]               = bgDark;
    style.Colors[ImGuiCol_PopupBg]               = ScaleColor(bg, 0.90f);

    //  Borders
    style.Colors[ImGuiCol_Border]                = WithAlpha(border, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

    //  Frames (input fields, checkboxes, combo boxes, sliders bg)
    style.Colors[ImGuiCol_FrameBg]               = WithAlpha(primaryDark, 0.35f);
    style.Colors[ImGuiCol_FrameBgHovered]        = WithAlpha(hover,       0.45f);
    style.Colors[ImGuiCol_FrameBgActive]         = WithAlpha(accent,      0.65f);

    //  Title bars
    style.Colors[ImGuiCol_TitleBg]               = primaryDark;
    style.Colors[ImGuiCol_TitleBgActive]         = primary;
    style.Colors[ImGuiCol_TitleBgCollapsed]      = WithAlpha(primaryDark, 0.50f);

    //  Menu bar
    style.Colors[ImGuiCol_MenuBarBg]             = bgLight;

    //  Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg]           = WithAlpha(bg, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = border;
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ScaleColor(border, 1.20f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = accent;

    //  Buttons
    style.Colors[ImGuiCol_Button]                = primary;
    style.Colors[ImGuiCol_ButtonHovered]         = hover;
    style.Colors[ImGuiCol_ButtonActive]          = accent;

    //  Sliders
    style.Colors[ImGuiCol_SliderGrab]            = hover;
    style.Colors[ImGuiCol_SliderGrabActive]      = accent;

    //  Headers (CollapsingHeader, Selectable, TreeNode)
    style.Colors[ImGuiCol_Header]                = WithAlpha(primary, 0.40f);
    style.Colors[ImGuiCol_HeaderHovered]         = WithAlpha(hover,   0.65f);
    style.Colors[ImGuiCol_HeaderActive]          = WithAlpha(accent,  0.85f);

    //  Checkmark / radio
    style.Colors[ImGuiCol_CheckMark]             = accent;

    //  Separators
    style.Colors[ImGuiCol_Separator]             = WithAlpha(border, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]      = hover;
    style.Colors[ImGuiCol_SeparatorActive]       = accent;

    //  Resize grip
    style.Colors[ImGuiCol_ResizeGrip]            = WithAlpha(primary, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = WithAlpha(hover,   0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = WithAlpha(accent,  0.90f);

    //  Tabs
    style.Colors[ImGuiCol_Tab]                   = primaryDark;
    style.Colors[ImGuiCol_TabHovered]            = primary;
    style.Colors[ImGuiCol_TabSelected]           = primary;
    style.Colors[ImGuiCol_TabDimmed]             = WithAlpha(primaryDark, 0.60f);
    style.Colors[ImGuiCol_TabDimmedSelected]     = WithAlpha(primary,     0.70f);

    //  Tables
    style.Colors[ImGuiCol_TableHeaderBg]         = bgLight;
    style.Colors[ImGuiCol_TableBorderStrong]     = WithAlpha(border, 0.60f);
    style.Colors[ImGuiCol_TableBorderLight]      = borderLight;
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

    out << "primary_r=" << state->primaryColor[0] << "\n";
    out << "primary_g=" << state->primaryColor[1] << "\n";
    out << "primary_b=" << state->primaryColor[2] << "\n";

    out << "accent_r=" << state->accentColor[0] << "\n";
    out << "accent_g=" << state->accentColor[1] << "\n";
    out << "accent_b=" << state->accentColor[2] << "\n";

    out << "bg_r=" << state->bgColor[0] << "\n";
    out << "bg_g=" << state->bgColor[1] << "\n";
    out << "bg_b=" << state->bgColor[2] << "\n";

    out << "text_r=" << state->textColor[0] << "\n";
    out << "text_g=" << state->textColor[1] << "\n";
    out << "text_b=" << state->textColor[2] << "\n";

    out << "border_r=" << state->borderColor[0] << "\n";
    out << "border_g=" << state->borderColor[1] << "\n";
    out << "border_b=" << state->borderColor[2] << "\n";

    out << "hover_r=" << state->hoverColor[0] << "\n";
    out << "hover_g=" << state->hoverColor[1] << "\n";
    out << "hover_b=" << state->hoverColor[2] << "\n";

    out << "autoTextColor=" << (state->autoTextColor ? 1 : 0) << "\n";

    out << "debugRadio=" << state->debugRadio << "\n";
    out << "warnRadio=" << state->warnRadio << "\n";
    out << "fontScaleSlider=" << state->fontScaleSlider << "\n";
    out << "fontCombo=" << state->fontCombo << "\n";
    out << "showAdvancedSettings=" << (state->showAdvancedSettings ? 1 : 0) << "\n";

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

        if      (key == "primary_r")    state->primaryColor[0] = std::stof(val);
        else if (key == "primary_g")    state->primaryColor[1] = std::stof(val);
        else if (key == "primary_b")    state->primaryColor[2] = std::stof(val);
        else if (key == "accent_r")     state->accentColor[0]  = std::stof(val);
        else if (key == "accent_g")     state->accentColor[1]  = std::stof(val);
        else if (key == "accent_b")     state->accentColor[2]  = std::stof(val);
        else if (key == "bg_r")         state->bgColor[0]      = std::stof(val);
        else if (key == "bg_g")         state->bgColor[1]      = std::stof(val);
        else if (key == "bg_b")         state->bgColor[2]      = std::stof(val);
        else if (key == "text_r")       state->textColor[0]    = std::stof(val);
        else if (key == "text_g")       state->textColor[1]    = std::stof(val);
        else if (key == "text_b")       state->textColor[2]    = std::stof(val);
        else if (key == "border_r")     state->borderColor[0]  = std::stof(val);
        else if (key == "border_g")     state->borderColor[1]  = std::stof(val);
        else if (key == "border_b")     state->borderColor[2]  = std::stof(val);
        else if (key == "hover_r")      state->hoverColor[0]   = std::stof(val);
        else if (key == "hover_g")      state->hoverColor[1]   = std::stof(val);
        else if (key == "hover_b")      state->hoverColor[2]   = std::stof(val);
        else if (key == "autoTextColor") state->autoTextColor  = std::stoi(val) != 0;
        else if (key == "debugRadio")   state->debugRadio   = std::stoi(val);
        else if (key == "warnRadio")    state->warnRadio    = std::stoi(val);
        else if (key == "fontScaleSlider") state->fontScaleSlider = std::stoi(val);
        else if (key == "fontCombo")      state->fontCombo     = std::stoi(val);
        else if (key == "showAdvancedSettings") state->showAdvancedSettings = std::stoi(val) != 0;
    }

    return true;
}
