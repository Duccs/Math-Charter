#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "Parser.h"
#include "SymbolTable.h"
#include "Node.h"
#include <memory>
#include <string>
#include <set>

// Expression types
enum class ExpressionKind {
    ConstantAssignment,  // a = 3       | not drawable
    CoordinateFunc,      // x = sin(y)  | drawable
    ImplicitFunc,        // x^2         | drawable
    Invalid              // y^2         | not drawable
};

struct ExpressionMetadata {
    ExpressionKind kind;
    std::string definesVariable;      // "y" in y = x^2, "a" in a = 3, empty for implicit
    std::set<std::string> dependsOn;  // {"x"} in y = x^2, {} in x = 5
    bool isDrawable;
    std::string independentVar;       // The variable to iterate over when drawing (x or y)
    std::string dependentVar;         // The coordinate being computed (y or x)
};

class Expression {
private:
    std::unique_ptr<Node> root;
    bool valid;
    std::string errorMessage;
    
    // Metadata populated during parsing
    std::set<std::string> usedVariables;
    std::string assignmentTarget;
    ExpressionMetadata metadata;
    
    // Helper to classify the expression
    void classify();

public:
    Expression();
    
    // Parse equation string into AST
    static Expression parse(const std::string& equation);
    float evaluate(SymbolTable& symbols) const;
    
    // Check if parsing succeeded
    bool isValid() const { return valid; }
    
    // Get error message if parsing failed
    const std::string& getError() const { return errorMessage; }
    
    // Get expression metadata
    const ExpressionMetadata& getMetadata() const { return metadata; }
    
    // Get used variables
    const std::set<std::string>& getUsedVariables() const { return usedVariables; }
    
    // Get assignment target (empty if not an assignment)
    const std::string& getAssignmentTarget() const { return assignmentTarget; }
};

#endif /* _EXPRESSION_H */
