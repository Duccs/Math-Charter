#include "Expression.h"
#include <Exceptions.h>
#include <config.h>

Expression::Expression() : root(nullptr), valid(false) {}

void Expression::classify() {
    // Separate used variables into coordinates and non-coordinates
    // TODO: When implementing t, rename variables.
    std::set<std::string> coordinateVars;
    std::set<std::string> constantVars;
    
    for (const auto& var : usedVariables) {
        if (SymbolTable::IsReserved(var)) {
            coordinateVars.insert(var);
        } else {
            constantVars.insert(var);
        }
    }
    
    // If this is an assignment
    if (!assignmentTarget.empty()) {
        bool targetIsCoordinate = SymbolTable::IsReserved(assignmentTarget);
        
        if (!targetIsCoordinate) {
            // This defines a constant, not a curve
            metadata.kind = ExpressionKind::ConstantAssignment;
            metadata.definesVariable = assignmentTarget;
            metadata.dependsOn = usedVariables;  // All used vars are dependencies
            metadata.isDrawable = false;
            metadata.independentVar = "";
            metadata.dependentVar = "";
        } else {
            // This defines a coordinate function
            metadata.kind = ExpressionKind::CoordinateFunc;
            metadata.definesVariable = assignmentTarget;
            metadata.isDrawable = true;
            
            // Determine independent variable
            coordinateVars.erase(assignmentTarget);
            
            if (coordinateVars.empty()) {
                if (assignmentTarget == "x") {
                    metadata.independentVar = "y";
                    metadata.dependentVar = "x";
                } else if (assignmentTarget == "y") {
                    metadata.independentVar = "x";
                    metadata.dependentVar = "y";
                } else {
                    // z = 5 or similar - for now, default to x
                    metadata.independentVar = "x";
                    metadata.dependentVar = assignmentTarget;
                }
            } else if (coordinateVars.size() == 1) {
                // One other coordinate var
                metadata.independentVar = *coordinateVars.begin();
                metadata.dependentVar = assignmentTarget;
            } else {
                // Multiple coordinate vars (z = x + y) - for future 3D support
                // Default to x as independent for now
                metadata.independentVar = "x";
                metadata.dependentVar = assignmentTarget;
            }
            
            metadata.dependsOn = coordinateVars;
            // Also include non-coordinate dependencies
            for (const auto& cv : constantVars) {
                metadata.dependsOn.insert(cv);
            }
        }
    } else {
        // implicit expression
        if (coordinateVars.empty()) {
            // No coordinates used
            metadata.kind = ExpressionKind::ImplicitFunc;
            metadata.definesVariable = "";
            metadata.dependsOn = constantVars;
            metadata.isDrawable = true;
            metadata.independentVar = "x";
            metadata.dependentVar = "y";
        } else if (coordinateVars.count("x") > 0 && coordinateVars.count("y") == 0) {
            // Uses x but not y
            metadata.kind = ExpressionKind::ImplicitFunc;
            metadata.definesVariable = "";
            metadata.dependsOn = usedVariables;
            metadata.isDrawable = true;
            metadata.independentVar = "x";
            metadata.dependentVar = "y";
        } else if (coordinateVars.count("y") > 0 && coordinateVars.count("x") == 0) {
            // Uses y but not x
            metadata.kind = ExpressionKind::Invalid;
            metadata.definesVariable = "";
            metadata.dependsOn = usedVariables;
            metadata.isDrawable = false;
            metadata.independentVar = "";
            metadata.dependentVar = "";
            
            // Set error
            errorMessage = "Cannot draw: expression uses 'y' but has no independent variable. Try 'x = " + std::string("y^2") + "' instead.";
            valid = false;
        } else {
            // Uses both x and y - this is an implicit relation
            // For now, treat as invalid since we don't support implicit curves
            metadata.kind = ExpressionKind::Invalid;
            metadata.definesVariable = "";
            metadata.dependsOn = usedVariables;
            metadata.isDrawable = false;
            metadata.independentVar = "";
            metadata.dependentVar = "";
            
            errorMessage = "Cannot draw implicit relation with both x and y. Use explicit form like 'y = f(x)' or 'x = f(y)'.";
            valid = false;
        }
    }
}

Expression Expression::parse(const std::string& equation) {
    Expression expr;
    
    try {
        expr.root = parseToAST(equation, expr.usedVariables, expr.assignmentTarget);
        expr.valid = true;
        expr.classify();
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
