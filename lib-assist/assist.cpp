#include <assist.h>
#include <config.h>
#include <Windows.h>

// Color conversion for visual purposes and ease
// You are absolutely correct! They all do the same thing and are redundant asl.
double red(int r) { return (double)r / 255.0; }
double green(int g) { return (double)g / 255.0; }
double blue(int b) { return (double)b / 255.0; }

// Get the exe directory path
std::filesystem::path getPath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
}

// Helper to map Math -> NDC (stands for Normalized Device Coordinates btw)
float mapToScreen(float value, float min, float max) {
    // Returns value between -1.0 and 1.0
    return ((value - min) / (max - min)) * 2.0f - 1.0f;
}
