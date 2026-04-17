#include "Curve2d.h"

// Constructor
Curve2D::Curve2D(const char* eq, float lineWidth, RenderColor color) 
    : Line2D(lineWidth, color), equation(eq) {
    Line2D(lineWidth, color); // Initialize base class

    MSG("Creating Curve2D with equation: %s" << eq);
    Expression expr = Expression::parse(equation);
    if (!expr.isValid()) {
        throw std::runtime_error("Invalid equation: " + expr.getError());
    }
}

// Destructor
Curve2D::~Curve2D() {
    // Base class destructor will be called automatically
}

// Generate vertex data in relation to GraphView
void Curve2D::generate(GraphView view) {
    strips = generateGraphPoints(equation.c_str(), view, cachedConstants);
}

// Generate vertex data with global constants
void Curve2D::generate(GraphView view, const SymbolTable* globalConstants) {
    cachedConstants = globalConstants;
    strips = generateGraphPoints(equation.c_str(), view, globalConstants);
}

// Setters and Getters
// -------------------
void Curve2D::setEquation(const char* eq) {
    equation = eq;
}

const std::string& Curve2D::getEquation() const {
    return equation;
}
