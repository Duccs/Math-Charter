#ifndef _VIEWPORT_UI_H_
#define _VIEWPORT_UI_H_

#include <GraphViewport.h>
#include <imgui.h>
#include <vector>
#include <string>

void GraphViewportWindow(bool* show, GraphViewport& viewport);

// Controls for the viewport
void GraphControlPanel(bool* show, GraphViewport& viewport,
                       std::vector<std::string>& logLines);

#endif /* _VIEWPORT_UI_H_ */
