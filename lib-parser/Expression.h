#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "Parser.h"
#include "SymbolTable.h"
#include "Node.h"
#include <memory>
#include <string>

class Expression {
private:
    std::unique_ptr<Node> root;
    bool valid;
    std::string errorMessage;

public:
    Expression();
    
    // Parse equation string into AST
    static Expression parse(const std::string& equation);
    float evaluate(SymbolTable& symbols) const;
    
    // Check if parsing succeeded
    bool isValid() const { return valid; }
    
    // Get error message if parsing failed
    const std::string& getError() const { return errorMessage; }
};

#endif /* _EXPRESSION_H */
