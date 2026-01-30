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

double red(int r);
double green(int g);
double blue(int b);

std::filesystem::path getPath();

float mapToScreen(float value, float min, float max);

std::vector<float> generateGraphPoints(const char* equation, int numPoints, GraphView view);

std::vector<float> generateGridLines(GraphView view, float gridSpacing = 1.0f);


#endif /*_HELPER_H_*/