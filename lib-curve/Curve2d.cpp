#include "Curve2d.h"

// Constructor
Curve2D::Curve2D(const char* eq, float lineWidth, RenderColor color) 
    : Line2D(lineWidth, color), equation(eq) {
    // Set up in parent constructor
}

// Destructor
Curve2D::~Curve2D() {
    // Base class destructor will be called automatically
}

// TODO: Plug in function from helper if unused by other classes
// Generate vertex data in relation to GraphView
void Curve2D::generate(GraphView view) {
    strips = generateGraphPoints(equation.c_str(), view);
}

// Setters and Getters
// -------------------
void Curve2D::setEquation(const char* eq) {
    equation = eq;
}

const std::string& Curve2D::getEquation() const {
    return equation;
}
