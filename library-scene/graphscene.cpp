#include "graphscene.h"

// Constructor
GraphScene::GraphScene(GraphView initialView, float spacing)
    : view(initialView), gridSpacing(spacing), gridVAO(0), gridVBO(0) {
    
    // Configure grid VBO and VAO
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    
    // Bind
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    
    // Configure vertex attribute positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Generate initial grid
    gridLines = generateGridLines(view, gridSpacing);
    
    // Upload grid data
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridLines.size() * sizeof(float),
                 gridLines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Destructor
GraphScene::~GraphScene() {
    cleanup();
}

// Add a new curve to the scene
Curve2D* GraphScene::addCurve(const char* equation, int numPoints) {
    curves.push_back(std::make_unique<Curve2D>(equation, numPoints));
    Curve2D* newCurve = curves.back().get();
    
    // Generate and upload curve data for current view
    newCurve->generate(view);
    newCurve->upload();
    
    return newCurve;
}

// Remove a curve from the scene
void GraphScene::removeCurve(Curve2D* curve) {
    // Find and remove the curve
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
    
    // Regenerate grid
    gridLines = generateGridLines(view, gridSpacing);
    
    // Re-upload grid data
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridLines.size() * sizeof(float),
                 gridLines.data(), GL_DYNAMIC_DRAW);
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

// Render the entire scene (grid + all curves)
void GraphScene::render(Shader& shader, float aspectRatio) {
    shader.use();
    shader.setFloat("wAspect", aspectRatio);
    
    // Render grid first
    if (!gridLines.empty()) {
        shader.setVec3("color", 0.0f, 0.0f, 0.0f);  // Black grid
        glBindVertexArray(gridVAO);
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, gridLines.size() / 3);
        glBindVertexArray(0);
    }
    
    // Render all curves
    for (auto& curve : curves) {
        RenderColor color = curve->getColor();
        shader.setVec3("color", color.red, color.green, color.blue);
        curve->render();
    }
    
    glBindVertexArray(0);
}

// Clean up OpenGL resources
void GraphScene::cleanup() {
    if (gridVAO != 0) glDeleteVertexArrays(1, &gridVAO);
    if (gridVBO != 0) glDeleteBuffers(1, &gridVBO);
    gridVAO = 0;
    gridVBO = 0;
    
    // Curves clean up themselves in their destructors
}
