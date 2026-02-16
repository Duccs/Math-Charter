#ifndef _MENUBAR_UI_H_
#define _MENUBAR_UI_H_

#include "Common-UI.h"

struct MenuBarState {
    bool showLog = false;
    bool showPreferences = false;
    bool showViewport = false;
    bool showGraphControls = false;
    bool showDemoWindow = false;
    bool showMetrics = false;
};

void InitializeMenuBar(GLFWwindow* window, std::vector<std::string>& loglines, MenuBarState* state);

#endif /* _MENUBAR_UI_H_ */