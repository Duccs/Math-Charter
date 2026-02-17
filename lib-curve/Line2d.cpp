#include "Line2d.h"

// Constructor
Line2D::Line2D(int points, float lineWidth, RenderColor color) 
    : numPoints(points), VAO(0), VBO(0), lineWidth(lineWidth), color(color) {
    
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
    points = generateGraphPoints("x", numPoints, view); // Default to y=x line
}

// Upload vertex data
void Line2D::upload() {
    if (points.empty()) return;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), 
                 points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Render the line/curve
void Line2D::render() {
    if (points.empty() || !visible) return;
    
    glBindVertexArray(VAO);
    glLineWidth(lineWidth);
    glDrawArrays(GL_LINE_STRIP, 0, points.size() / 3);
    glBindVertexArray(0);
}

// Regenerate and re-upload, Rinse and repeat
void Line2D::update(GraphView view) {
    generate(view);
    upload();
}


// Setter and Getters
// ------------------
int Line2D::getNumPoints() const {
    return numPoints;
}

void Line2D::setNumPoints(int points) {
    if (points < 0) throw std::invalid_argument("Number of points cannot be negative.");
    numPoints = points;
}

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