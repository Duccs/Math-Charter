#include "VertexGenerator.h"

// Recursive adaptive tessellation
void adaptiveTessellate(
    Expression& expr,
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
    float yMid = expr.evaluate(xMid);
    
    // What would linear interpolation predict?
    float yLinear = (y1 + y2) / 2.0f;
    
    // How much does the actual curve deviate from straight line?
    float error = std::abs(yMid - yLinear);
    
    // If error is too large and we haven't hit max depth, subdivide
    if (error > tolerance && depth < maxDepth) {
        // Recursively subdivide both halves
        adaptiveTessellate(expr, x1, y1, xMid, yMid, view, vertices, tolerance, depth + 1, maxDepth);
        adaptiveTessellate(expr, xMid, yMid, x2, y2, view, vertices, tolerance, depth + 1, maxDepth);
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
    Expression expr = Expression::parse(equation);
    if (!expr.isValid()) {
        throw std::runtime_error("Invalid equation: " + expr.getError());
    }
    
    // Use adaptive tessellation: starts with initial segments based on numPoints
    // then subdivides where needed
    int numSegments = std::max(10, numPoints / 20); // Start with fewer segments
    float step = (view.maxX - view.minX) / numSegments;
    
    for (int i = 0; i < numSegments; ++i) {
        float x1 = view.minX + (i * step);
        float x2 = view.minX + ((i + 1) * step);
        float y1 = expr.evaluate(x1);
        float y2 = expr.evaluate(x2);
        
        // Adaptively tessellate this segment
        adaptiveTessellate(expr, x1, y1, x2, y2, view, vertices);
    }
    
    // Add the final point
    float finalX = view.maxX;
    float finalY = expr.evaluate(finalX);
    float glX = mapToScreen(finalX, view.minX, view.maxX);
    float glY = mapToScreen(finalY, view.minY, view.maxY);
    vertices.push_back(glX);
    vertices.push_back(glY);
    vertices.push_back(0.0f);
    
    return vertices;
}
