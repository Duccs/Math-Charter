#ifndef _VIEWPORT_UI_H_
#define _VIEWPORT_UI_H_

#include <GraphViewport.h>
#include <Preferences-UI.h>
#include <imgui.h>
#include <vector>
#include <string>

static const ImVec4 curvePalette[] = {
                    {red(217), green(0), blue(0), 1.0f},  // Red #D90000
                    {red(0), green(220), blue(0), 1.0f},  // Green #00DC00
                    {red(0),  green(83), blue(255), 1.0f},  // Blue #0053FF
                    {red(255), green(172), blue(0), 1.0f}, // Yellow #FFAC00
                    {red(170), green(0), blue(255), 1.0f},  // Purple #AA00FF
                    {red(0),  green(0),  blue(0), 1.0f},   // Black #3c3836
                };

void GraphViewportWindow(bool* show, GraphViewport& viewport);

// Controls for the viewport
void GraphControlPanel(bool* show, GraphViewport& viewport,
                       std::vector<std::string>& logLines, PreferencesState* state);

#endif /* _VIEWPORT_UI_H_ */
