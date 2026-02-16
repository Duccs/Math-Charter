#ifndef _ASSIST_UI_H_
#define _ASSIST_UI_H_

#include "Common-UI.h"

void SectionHeader(const char* label);

void HelpTooltip(const char* description);

bool ColorRow(const char* label, float color[3], const char* tooltip = nullptr);

ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t);

float Saturate(float v);

ImVec4 ScaleColor(const ImVec4& c, float factor);

ImVec4 WithAlpha(const ImVec4& c, float a);

#endif /* _ASSIST_UI_H_ */