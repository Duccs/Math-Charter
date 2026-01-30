#include "curve2d.h"

// Constructor
Curve2D::Curve2D(const char* eq, int points) 
    : equation(eq), numPoints(points), VAO(0), VBO(0) {
    color = {0.0f, 0.0f, 0.0f}; // Default black
    
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
Curve2D::~Curve2D() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
}

// Generate vertex data in relation to GraphView
void Curve2D::generate(GraphView view) {
    points = generateGraphPoints(equation.c_str(), numPoints, view);
}

// Upload vertex data
void Curve2D::upload() {
    if (points.empty()) return;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), 
                 points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Render the curve
void Curve2D::render() {
    if (points.empty()) return;
    
    glBindVertexArray(VAO);
    glLineWidth(4.0f);
    glDrawArrays(GL_LINE_STRIP, 0, points.size() / 3);
    glBindVertexArray(0);
}

// Regenerate and re-upload, Rinse and repeat
void Curve2D::update(GraphView view) {
    generate(view);
    upload();
}

// Setters and Getters
// -------------------
void Curve2D::setEquation(const char* eq) {
    equation = eq;
}

const std::string& Curve2D::getEquation() const {
    return equation;
}

int Curve2D::getNumPoints() const {
    return numPoints;
}

void Curve2D::setColor(float r, float g, float b) {
    if (r < 0.0f || r > 1.0f) r = 0.0f;
    if (g < 0.0f || g > 1.0f) g = 0.0f;
    if (b < 0.0f || b > 1.0f) b = 0.0f;
    color.red = r;
    color.green = g;
    color.blue = b;
}

RenderColor Curve2D::getColor() const {
    return color;
}

