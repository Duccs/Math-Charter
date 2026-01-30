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

// Generates vertex data for a given equation. Parsing is outsourced to another method.
std::vector<float> generateGraphPoints(const char* equation, int numPoints, GraphView view) {
    std::vector<float> vertices;
    
    double x; // The variable we will change

    // Generate Points
    float step = (view.maxX - view.minX) / numPoints;
    
    for (int i = 0; i <= numPoints; ++i) {
        // Determine Math X
        float currentX = view.minX + (i * step);
        
        // Calculate Y using expression parser
        x = currentX; // Update the variable pointer
        float currentY;
        // TODO: Implement expression parser
        // Hard code 
        switch (equation[0]) {
            case 's': // sin(x)
                currentY = sin(x);
                break;
            case 'c': // cos(x)
                currentY = cos(x);
                break;
            case 'x': // Parabola
                currentY = x * x;
                break;
            case 'n': // inverted Parabola
                currentY = - (x * x);
                break;
            default:
                currentY = x; //
                break;
        }
        

        // Convert to Screen Coordinates (NDC)
        float glX = mapToScreen(currentX, view.minX, view.maxX);
        float glY = mapToScreen(currentY, view.minY, view.maxY);

        // Add to list (z = 0)
        vertices.push_back(glX);
        vertices.push_back(glY);
        vertices.push_back(0.0f);
    }

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

