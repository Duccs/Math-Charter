#include "Line2d.h"

// Constructor
Line2D::Line2D(float lineWidth, RenderColor color) 
    : VAO(0), VBO(0), lineWidth(lineWidth), color(color) {
    
    // Configure VAO and VBO
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

// Destructor
Line2D::~Line2D() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    VAO = 0;
    VBO = 0;
}

// Generate vertex data in relation to GraphView
void Line2D::generate(GraphView view) {
    strips = generateGraphPoints("x", view); // Default to y=x line
}

// Upload vertex data — flatten all sub-strips into a single VBO
// and record draw ranges for each strip.
void Line2D::upload() {
    vboData.clear();
    drawRanges.clear();

    int vertexOffset = 0;
    for (const auto& strip : strips) {
        int vertexCount = static_cast<int>(strip.size()) / 3;
        if (vertexCount < 2) continue;  // need at least 2 points for a line
        drawRanges.push_back({vertexOffset, vertexCount});
        vboData.insert(vboData.end(), strip.begin(), strip.end());
        vertexOffset += vertexCount;
    }

    if (vboData.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(float),
                 vboData.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Render each sub-strip as an independent GL_LINE_STRIP
void Line2D::render() {
    if (drawRanges.empty() || !visible) return;

    glBindVertexArray(VAO);
    glLineWidth(lineWidth);
    for (const auto& [start, count] : drawRanges) {
        glDrawArrays(GL_LINE_STRIP, start, count);
    }
    glBindVertexArray(0);
}

// Regenerate and re-upload, Rinse and repeat
void Line2D::update(GraphView view) {
    generate(view);
    upload();
}


// Setter and Getters
// ------------------
void Line2D::setColor(float r, float g, float b) {
    if (r < 0.0f || r > 1.0f) r = 0.0f;
    if (g < 0.0f || g > 1.0f) g = 0.0f;
    if (b < 0.0f || b > 1.0f) b = 0.0f;
    color.red = r;
    color.green = g;
    color.blue = b;
}

RenderColor Line2D::getColor() const {
    return color;
}

float Line2D::getLineWidth() const {
    return lineWidth;
}

void Line2D::setLineWidth(float width) {
    if (width <= 0.0f) throw std::invalid_argument("Line width must be positive.");
    lineWidth = width;
}

LineType Line2D::getLineType() const {
    return lineType;
}

void Line2D::setLineType(LineType type) {
    lineType = type;
}

bool Line2D::isVisible() const {
    return visible;
}

void Line2D::setVisible(bool v) {
    visible = v;
}