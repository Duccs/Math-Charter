#include "Expression.h"
#include <config.h>

Expression::Expression() : root(nullptr), valid(false) {}

Expression Expression::parse(const std::string& equation) {
    Expression expr;
    
    try {
        expr.root = parseToAST(equation);
        expr.valid = true;
    } catch (const std::exception& e) {
        expr.valid = false;
        expr.errorMessage = e.what();
    }
    
    return expr;
}

float Expression::evaluate(SymbolTable& symbols) const {
    if (!valid || !root) {
        WARN("IN:'Expression.cpp evaluate()' Cannot evaluate invalid or empty expression");
        return 0.0f;
    }
    
    return root->evaluate(symbols);
}
