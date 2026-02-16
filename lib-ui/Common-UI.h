#ifndef _COMMON_UI_H_
#define _COMMON_UI_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <fstream>

void LogWindow(bool* show, std::vector<std::string>& logLines);

#endif /* _COMMON_UI_H_ */