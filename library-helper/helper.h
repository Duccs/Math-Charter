#ifndef _HELPER_H_
#define _HELPER_H_

#include <Windows.h>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>

struct GraphView {
    float minX = -10.0f;
    float maxX =  10.0f;
    float minY = -10.0f;
    float maxY =  10.0f;
};

struct RenderColor {
    float red = 0.0f;
    float green =  0.0f;
    float blue = 0.0f;
};


double red(int r);
double green(int g);
double blue(int b);

std::filesystem::path getPath();

float mapToScreen(float value, float min, float max);

// !!! Remove in future ↓↓↓ !!!
// Evaluate equation at given x value (uses hard-coded switch)
float evaluateEquation(const char* equation, float x);

std::vector<float> generateGraphPoints(const char* equation, int numPoints, GraphView view);

#endif /*_HELPER_H_*/