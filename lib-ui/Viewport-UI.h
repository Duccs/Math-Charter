#ifndef _VIEWPORT_UI_H_
#define _VIEWPORT_UI_H_

#include <GraphViewport.h>
#include <Preferences-UI.h>
#include <imgui.h>
#include <vector>
#include <string>

static const ImVec4 curvePalette[] = {
                    {0.851f, 0.0f, 0.0f, 1.0f},  // Red #D90000
                    {0.0f, 0.86f, 0.0f, 1.0f},  // Green #00DC00
                    {0.0f, 0.325f, 1.0f, 1.0f},  // Blue #0053FF
                    {1.0f, 0.674f, 0.0f, 1.0f}, // Yellow #FFAC00
                    {0.667f, 0.0f, 1.0f, 1.0f},  // Purple #AA00FF
                    {0.0f,  0.0f,  0.0f, 1.0f},   // Black #3c3836
                };

void GraphViewportWindow(bool* show, GraphViewport& viewport);

// Controls for the viewport
void GraphControlPanel(bool* show, GraphViewport& viewport,
                       std::vector<std::string>& logLines, PreferencesState* state);

#endif /* _VIEWPORT_UI_H_ */
