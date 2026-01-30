#include <helper.h>

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

// Evaluate equation at given x value (hard-coded for now)
float evaluateEquation(const char* equation, float x) {
    switch (equation[0]) {
        case 's': // sin(x)
            return sin(x);
        case 'c': // cos(x)
            return cos(x);
        case 'x': // Parabola
            return x * x;
        case 'n': // inverted Parabola
            return -(x * x);
        case 't': // tan(x)
            return tan(x);
        case 'i': // 1/x (inverse)
            return (x != 0.0f) ? (1.0f / x) : 0.0f;
        default:
            return x;
    }
}

// Recursive adaptive tessellation
void adaptiveTessellate(
    const char* equation,
    float x1, float y1,
    float x2, float y2,
    GraphView view,
    std::vector<float>& vertices,
    float tolerance = 0.005f,  // Max allowed deviation
    int depth = 0,
    int maxDepth = 12          // Prevent infinite recursion
) {
    // Calculate midpoint
    float xMid = (x1 + x2) / 2.0f;
    float yMid = evaluateEquation(equation, xMid);
    
    // What would linear interpolation predict?
    float yLinear = (y1 + y2) / 2.0f;
    
    // How much does the actual curve deviate from straight line?
    float error = std::abs(yMid - yLinear);
    
    // If error is too large and we haven't hit max depth, subdivide
    if (error > tolerance && depth < maxDepth) {
        // Recursively subdivide both halves
        adaptiveTessellate(equation, x1, y1, xMid, yMid, view, vertices, tolerance, depth + 1, maxDepth);
        adaptiveTessellate(equation, xMid, yMid, x2, y2, view, vertices, tolerance, depth + 1, maxDepth);
    } else {
        // Good enough - add the start point (end point added by next segment)
        float glX = mapToScreen(x1, view.minX, view.maxX);
        float glY = mapToScreen(y1, view.minY, view.maxY);
        vertices.push_back(glX);
        vertices.push_back(glY);
        vertices.push_back(0.0f);
    }
}

// Generates vertex data for a given equation using adaptive tessellation
std::vector<float> generateGraphPoints(const char* equation, int numPoints, GraphView view) {
    std::vector<float> vertices;
    
    // Use adaptive tessellation: starts with initial segments based on numPoints
    // then subdivides where needed
    int numSegments = std::max(10, numPoints / 20); // Start with fewer segments
    float step = (view.maxX - view.minX) / numSegments;
    
    for (int i = 0; i < numSegments; ++i) {
        float x1 = view.minX + (i * step);
        float x2 = view.minX + ((i + 1) * step);
        float y1 = evaluateEquation(equation, x1);
        float y2 = evaluateEquation(equation, x2);
        
        // Adaptively tessellate this segment
        adaptiveTessellate(equation, x1, y1, x2, y2, view, vertices);
    }
    
    // Add the final point
    float finalX = view.maxX;
    float finalY = evaluateEquation(equation, finalX);
    float glX = mapToScreen(finalX, view.minX, view.maxX);
    float glY = mapToScreen(finalY, view.minY, view.maxY);
    vertices.push_back(glX);
    vertices.push_back(glY);
    vertices.push_back(0.0f);
    
    return vertices;
}

// Generates vertex data for grid lines. Spacing is adjustable, but should probably stay at 1.0f.
std::vector<float> generateGridLines(GraphView view, float gridSpacing) {
    std::vector<float> vertices;
    
    // Generate vertical lines
    // Start from the first grid line at or before minX
    float startX = std::floor(view.minX / gridSpacing) * gridSpacing;
    for (float x = startX; x <= view.maxX; x += gridSpacing) {
        // Convert to NDC
        float glX = mapToScreen(x, view.minX, view.maxX);
        float glYMin = mapToScreen(view.minY, view.minY, view.maxY);
        float glYMax = mapToScreen(view.maxY, view.minY, view.maxY);
        
        // Line from bottom to top
        vertices.push_back(glX);     // x1
        vertices.push_back(glYMin);  // y1
        vertices.push_back(0.0f);    // z1
        
        vertices.push_back(glX);     // x2
        vertices.push_back(glYMax);  // y2
        vertices.push_back(0.0f);    // z2
    }
    
    // Generate horizontal lines
    // Start from the first grid line at or before minY
    float startY = std::floor(view.minY / gridSpacing) * gridSpacing;
    for (float y = startY; y <= view.maxY; y += gridSpacing) {
        // Convert to NDC
        float glY = mapToScreen(y, view.minY, view.maxY);
        float glXMin = mapToScreen(view.minX, view.minX, view.maxX);
        float glXMax = mapToScreen(view.maxX, view.minX, view.maxX);
        
        // Line from left to right
        vertices.push_back(glXMin);  // x1
        vertices.push_back(glY);     // y1
        vertices.push_back(0.0f);    // z1
        
        vertices.push_back(glXMax);  // x2
        vertices.push_back(glY);     // y2
        vertices.push_back(0.0f);    // z2
    }
    
    return vertices;
}

