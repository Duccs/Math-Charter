#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "Parser.h"
#include <cmath>
#include <stack>
#include <stdexcept>
#include <sstream>

class Expression {
private:
    // Hold expression in postfix
    std::vector<TokenClass> postfix;
    bool valid;
    std::string errorMessage;

public:
    Expression();
    
    // Parse equation string into postfix representation
    static Expression parse(const std::string& equation);
    
    // Evaluate the expression with given x value
    float evaluate(float x) const;

    //TODO: Add support for multiple variables (e.g., y, z) 
    //      and pass in a map of variable values to evaluate()
    
    // Check if parsing succeeded
    bool isValid() const { return valid; }
    
    // Get error message if parsing failed
    const std::string& getError() const { return errorMessage; }
    
    // Get the postfix tokens (for debugging)
    const std::vector<TokenClass>& getPostfix() const { return postfix; }
    
    // Convert postfix to string representation (for debugging)
    std::string toPostfixString() const;
};

#endif /* _EXPRESSION_H */
