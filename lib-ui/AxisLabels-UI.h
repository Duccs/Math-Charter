#ifndef _AXIS_LABELS_UI_H_
#define _AXIS_LABELS_UI_H_

#include <assist.h>
#include <imgui.h>

void DrawAxisTickOverlay(ImDrawList* drawList,
                         ImVec2 origin, ImVec2 size,
                         const GraphView& view,
                         float spacing);

#endif /* _AXIS_LABELS_UI_H_ */
