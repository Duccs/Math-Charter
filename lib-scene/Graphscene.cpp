#include "Graphscene.h"

// Constructor
GraphScene::GraphScene(GraphView initialView)
    : view(initialView), axisVAO(0), axisVBO(0),
      majorGridVAO(0), majorGridVBO(0),
      minorGridVAO(0), minorGridVBO(0) {
    
    // Configure grid VBOs and VAOs
    initVAOnVBO(axisVAO, axisVBO);
    initVAOnVBO(majorGridVAO, majorGridVBO);
    initVAOnVBO(minorGridVAO, minorGridVBO);
    
    // Calculate adaptive spacing based on initial view
    float viewRange = std::max(view.maxX - view.minX, view.maxY - view.minY);
    gridSpacing = calculateAdaptiveSpacing(viewRange);
    
    // Generate initial grid
    axisGridLines = generateAxisLines(view);
    majorGridLines = generateMajorLines(view, gridSpacing);
    minorGridLines = generateMinorLines(view, gridSpacing);
    
    // Upload grid data
    // Axis
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, axisGridLines.size() * sizeof(float),
                 axisGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Major Lines
    glBindBuffer(GL_ARRAY_BUFFER, majorGridVBO);
    glBufferData(GL_ARRAY_BUFFER, majorGridLines.size() * sizeof(float),
                 majorGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Minor Lines
    glBindBuffer(GL_ARRAY_BUFFER, minorGridVBO);
    glBufferData(GL_ARRAY_BUFFER, minorGridLines.size() * sizeof(float),
                 minorGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Destructor
GraphScene::~GraphScene() {
    cleanup();
}

// Initialize VAO and VBO for grid lines
void GraphScene::initVAOnVBO(unsigned int& VAO, unsigned int& VBO) {
    // Configure VBO and VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Configure vertex attribute positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GraphScene::renderGrid(Shader& shader) {

    // Order as such for correct layering: minor -> major -> axis
    if (!minorGridLines.empty()) {
        shader.setVec3("color", 0.85f, 0.85f, 0.85f);  // Very light gray minor grid
        glBindVertexArray(minorGridVAO);
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, minorGridLines.size() / 3);
        glBindVertexArray(0);
    }

    if (!majorGridLines.empty()) {
        shader.setVec3("color", 0.7f, 0.7f, 0.7f);  // Light gray major grid
        glBindVertexArray(majorGridVAO);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINES, 0, majorGridLines.size() / 3);
        glBindVertexArray(0);
    }

    if (!axisGridLines.empty()) {
        shader.setVec3("color", 0.0f, 0.0f, 0.0f);  // Black grid
        glBindVertexArray(axisVAO);
        glLineWidth(3.0f);
        glDrawArrays(GL_LINES, 0, axisGridLines.size() / 3);
        glBindVertexArray(0);
    }

}

// Calculate adaptive spacing based on view range
float GraphScene::calculateAdaptiveSpacing(float viewRange) {
    // Further test but about ~8-15 major grid lines visible at any zoom level
    float idealSpacing = viewRange / 10.0f;
    
    // Sequence: ...0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50, 100...
    float power = std::floor(std::log10(idealSpacing));
    float base = std::pow(10.0f, power);
    float normalized = idealSpacing / base;  // Value between 1 and 10
    
    // Snap to nearest value in 1-2-5 sequence
    float spacing;
    if (normalized < 1.5f) {
        spacing = base * 1.0f;       // Use 1
    } else if (normalized < 3.5f) {
        spacing = base * 2.0f;       // Use 2
    } else if (normalized < 7.5f) {
        spacing = base * 5.0f;       // Use 5
    } else {
        spacing = base * 10.0f;      // Use 10 (next decade)
    }
    
    // Clamp to reasonable bounds
    spacing = std::max(0.0001f, std::min(spacing, 10000.0f));
    
    return spacing;
}

// Generate grid lines
std::vector<float> GraphScene::generateGridLines(GraphView view, GridConfig config) {
    std::vector<float> vertices;
    
    // Precompute screen bounds
    float glYMin = mapToScreen(view.minY, view.minY, view.maxY);
    float glYMax = mapToScreen(view.maxY, view.minY, view.maxY);
    float glXMin = mapToScreen(view.minX, view.minX, view.maxX);
    float glXMax = mapToScreen(view.maxX, view.minX, view.maxX);
    
    // Axis-only mode (spacing = 0)
    if (config.spacing == 0.0f) {
        // Generate Y-axis (x = 0) if visible
        if (view.minX <= 0.0f && view.maxX >= 0.0f) {
            float glX = mapToScreen(0.0f, view.minX, view.maxX);
            vertices.push_back(glX);      vertices.push_back(glYMin);   vertices.push_back(0.0f);
            vertices.push_back(glX);      vertices.push_back(glYMax);   vertices.push_back(0.0f);
        }
        // Generate X-axis (y = 0) if visible
        if (view.minY <= 0.0f && view.maxY >= 0.0f) {
            float glY = mapToScreen(0.0f, view.minY, view.maxY);
            vertices.push_back(glXMin);   vertices.push_back(glY);      vertices.push_back(0.0f);
            vertices.push_back(glXMax);   vertices.push_back(glY);      vertices.push_back(0.0f);
        }
        return vertices;
    }
    
    // Helper lambda to check if a value falls on a major line
    auto isOnMajorLine = [&](float val) -> bool {
        if (config.skipMajorSpacing <= 0.0f) return false;
        float remainder = std::fmod(std::abs(val), config.skipMajorSpacing);
        return (remainder < config.skipMajorSpacing * 0.001f) || 
               (remainder > config.skipMajorSpacing * 0.999f);
    };
    
    // Generate vertical lines
    float startX = std::floor(view.minX / config.spacing) * config.spacing;
    for (float x = startX; x <= view.maxX; x += config.spacing) {
        // Skip axis lines
        if (std::abs(x) < config.skipTolerance) continue;
        // Skip major lines if configured
        if (isOnMajorLine(x)) continue;
        
        float glX = mapToScreen(x, view.minX, view.maxX);
        vertices.push_back(glX);      vertices.push_back(glYMin);   vertices.push_back(0.0f);
        vertices.push_back(glX);      vertices.push_back(glYMax);   vertices.push_back(0.0f);
    }
    
    // Generate horizontal lines
    float startY = std::floor(view.minY / config.spacing) * config.spacing;
    for (float y = startY; y <= view.maxY; y += config.spacing) {
        // Skip axis lines
        if (std::abs(y) < config.skipTolerance) continue;
        // Skip major lines if configured
        if (isOnMajorLine(y)) continue;
        
        float glY = mapToScreen(y, view.minY, view.maxY);
        vertices.push_back(glXMin);   vertices.push_back(glY);      vertices.push_back(0.0f);
        vertices.push_back(glXMax);   vertices.push_back(glY);      vertices.push_back(0.0f);
    }
    
    return vertices;
}

std::vector<float> GraphScene::generateAxisLines(GraphView view) {
    // Spacing=0 triggers axis-only generation
    return generateGridLines(view, {0.0f, false, 0.0f, 0.0f});
}

std::vector<float> GraphScene::generateMajorLines(GraphView view, float spacing) {
    // Skip axis
    return generateGridLines(view, {spacing, true, spacing * 0.001f, 0.0f});
}

std::vector<float> GraphScene::generateMinorLines(GraphView view, float spacing) {
    // Subdivide major spacing by 5, skip both axis and major lines
    float minorSpacing = spacing / 5.0f;
    return generateGridLines(view, {minorSpacing, true, minorSpacing * 0.001f, spacing});
}

// Add a new curve to the scene
Curve2D* GraphScene::addCurve(const char* equation, int numPoints, float lineWidth, RenderColor color) {
    curves.push_back(std::make_unique<Curve2D>(equation, numPoints, lineWidth, color));
    Curve2D* newCurve = curves.back().get();
    
    newCurve->generate(view);
    newCurve->upload();
    return newCurve;
}

// Remove a curve from the scene
void GraphScene::removeCurve(Curve2D* curve) {
    for (auto it = curves.begin(); it != curves.end(); ++it) {
        if (it->get() == curve) {
            curves.erase(it);
            break;
        }
    }
}

// Update view and regenerate all geometry
void GraphScene::updateView(GraphView newView) {
    view = newView;
    
    // Update all curves
    for (auto& curve : curves) {
        curve->update(view);
    }
    
    // Recalculate adaptive spacing based on new view
    float viewRange = std::max(view.maxX - view.minX, view.maxY - view.minY);
    gridSpacing = calculateAdaptiveSpacing(viewRange);
    
    // Regenerate grid
    axisGridLines = generateAxisLines(view);
    majorGridLines = generateMajorLines(view, gridSpacing);
    minorGridLines = generateMinorLines(view, gridSpacing);
    
    // Re-upload grid data
    // Axis
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, axisGridLines.size() * sizeof(float),
                 axisGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Major Lines
    glBindBuffer(GL_ARRAY_BUFFER, majorGridVBO);
    glBufferData(GL_ARRAY_BUFFER, majorGridLines.size() * sizeof(float),
                 majorGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Minor Lines
    glBindBuffer(GL_ARRAY_BUFFER, minorGridVBO);
    glBufferData(GL_ARRAY_BUFFER, minorGridLines.size() * sizeof(float),
                 minorGridLines.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Pan the view by dx, dy in world coordinates
void GraphScene::pan(float dx, float dy) {
    GraphView newView = view;
    newView.minX += dx;
    newView.maxX += dx;
    newView.minY += dy;
    newView.maxY += dy;
    updateView(newView);
}

// Zoom the view by a factor (e.g., 0.9 = zoom in, 1.1 = zoom out)
void GraphScene::zoom(float factor) {
    GraphView newView = view;
    
    // Calculate center point
    float centerX = (view.minX + view.maxX) / 2.0f;
    float centerY = (view.minY + view.maxY) / 2.0f;
    
    // Calculate half-widths
    float halfWidth = (view.maxX - view.minX) / 2.0f;
    float halfHeight = (view.maxY - view.minY) / 2.0f;
    
    // Apply zoom factor
    halfWidth *= factor;
    halfHeight *= factor;
    
    // Update view bounds
    newView.minX = centerX - halfWidth;
    newView.maxX = centerX + halfWidth;
    newView.minY = centerY - halfHeight;
    newView.maxY = centerY + halfHeight;
    
    updateView(newView);
}

// Zoom the view by a factor, centered on a specific world coordinate
void GraphScene::zoomAt(float worldX, float worldY, float factor) {
    GraphView newView = view;

    // Scale each edge relative to the focal point
    newView.minX = worldX + (view.minX - worldX) * factor;
    newView.maxX = worldX + (view.maxX - worldX) * factor;
    newView.minY = worldY + (view.minY - worldY) * factor;
    newView.maxY = worldY + (view.maxY - worldY) * factor;

    updateView(newView);
}

// Render the entire scene (grid + all curves)
void GraphScene::render(Shader& shader, float aspectRatio) {
    shader.use();
    shader.setFloat("wAspect", aspectRatio);

    // Render grid
    renderGrid(shader);
        
    // Render all curves
    for (auto& curve : curves) {
        if (!curve->isVisible()) continue;
        RenderColor color = curve->getColor();
        shader.setVec3("color", color.red, color.green, color.blue);
        curve->render();
    }
    
    glBindVertexArray(0);
}

// Clean up OpenGL resources
void GraphScene::cleanup() {
    if (axisVAO != 0) glDeleteVertexArrays(1, &axisVAO);
    if (axisVBO != 0) glDeleteBuffers(1, &axisVBO);
    if (majorGridVAO != 0) glDeleteVertexArrays(1, &majorGridVAO);
    if (majorGridVBO != 0) glDeleteBuffers(1, &majorGridVBO);
    if (minorGridVAO != 0) glDeleteVertexArrays(1, &minorGridVAO);
    if (minorGridVBO != 0) glDeleteBuffers(1, &minorGridVBO);
    axisVAO = 0;
    axisVBO = 0;
    majorGridVAO = 0;
    majorGridVBO = 0;
    minorGridVAO = 0;
    minorGridVBO = 0;
    
    // Curves clean up themselves in their destructors
}
