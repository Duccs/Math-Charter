#ifndef _PREFERENCES_UI_H_
#define _PREFERENCES_UI_H_

#include "Common-UI.h"
#include "assist.h"

struct PreferencesState {
    // 6-color palette
    float primaryColor[3] = {0.4f, 0.7f, 1.0f};    // Buttons, title bars, headers, tabs
    float accentColor[3]  = {1.0f, 0.3f, 0.3f};     // Active/pressed states, checkmarks
    float bgColor[3]      = {0.12f, 0.12f, 0.14f};  // Window backgrounds, panels
    float textColor[3]    = {0.95f, 0.95f, 0.95f};   // Text (manual or auto)
    float borderColor[3]  = {0.30f, 0.30f, 0.35f};   // Borders, separators, table lines
    float hoverColor[3]   = {0.55f, 0.80f, 1.0f};    // Hovered buttons, frames, headers
    bool  autoTextColor   = true;                     // Auto-compute text from bg luminance
    int colorPresetCombo = 0;
    int fontCombo = 0;
    int debugRadio = 0;
    int warnRadio = 0;
    int fontScaleSlider = 50;
    bool showAdvancedSettings = false;
    // Font metadata — populated by main after loading fonts
    static constexpr int MAX_FONTS = 32;
    const char* fontNames[MAX_FONTS] = {};
    int fontCount = 0;
};

void PreferencesWindow(bool* show, PreferencesState* state);

void ChangeFontScale(const PreferencesState* state);
void ApplyPreferences(const PreferencesState* state);
void ApplyPreferencesStyle(const PreferencesState* state);

bool SavePreferences(const char* filepath, const PreferencesState* state);
bool LoadPreferences(const char* filepath, PreferencesState* state);

#endif /* _PREFERENCES_UI_H_ */