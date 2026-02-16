#ifndef _PREFERENCES_UI_H_
#define _PREFERENCES_UI_H_

#include "Common-UI.h"
#include "assist.h"

struct PreferencesState {
    float colorA[3] = {0.4f, 0.7f, 1.0f};
    float colorB[3] = {1.0f, 0.3f, 0.3f};
    float bgColor[3] = {0.12f, 0.12f, 0.14f};
    int colorPresetCombo = 0;
    int fontCombo = 0;
    int debugRadio = 0;
    int warnRadio = 0;
    int uiScaleSlider = 50;
    float currentUIScale = 1.0f;

    // Font metadata — populated by main after loading fonts
    static constexpr int MAX_FONTS = 8;
    const char* fontNames[MAX_FONTS] = {};
    int fontCount = 0;
};

void PreferencesWindow(bool* show, PreferencesState* state);

void ChangeUIScale(const PreferencesState* state);
void ApplyPreferences(const PreferencesState* state);
void ApplyPreferencesStyle(const PreferencesState* state);

bool SavePreferences(const char* filepath, const PreferencesState* state);
bool LoadPreferences(const char* filepath, PreferencesState* state);

#endif /* _PREFERENCES_UI_H_ */