#include "Parser.h"
#include <stack>
#include <stdexcept>

int getPrecedence(TokenType type) {
    switch (type) {
        case PLUS_TOKEN:
        case MINUS_TOKEN:
            return 2;
        
        case TIMES_TOKEN:
        case DIVIDE_TOKEN:
            return 3;
        
        case FACTORIAL_TOKEN:
            return 4;
        
        // Left paren has lowest precedence (a real mystery)
        case LPAREN_TOKEN:
            return 0;

        default:
            // Debug
            WARN("IN:'Parser.cpp getPrecedence()' Unhandled token type in getPrecedence: " << TokenClass::GetTokenTypeName(type));
            return 0;
    }
}

bool isRightAssociative(TokenType type) {
    // Exponentiation is right-associative: 2^3^4 = 2^(3^4)
    return type == FACTORIAL_TOKEN;
}

bool isOperator(TokenType type) {
    return type == PLUS_TOKEN || 
           type == MINUS_TOKEN || 
           type == TIMES_TOKEN || 
           type == DIVIDE_TOKEN ||
           type == FACTORIAL_TOKEN;
}

bool isFunction(TokenType type) {
    // Manually update for new functions added to TokenType
    switch (type) {
        case SIN_TOKEN:
        case COS_TOKEN:
        case TAN_TOKEN:
        case COT_TOKEN:
        case SEC_TOKEN:
        case CSC_TOKEN:
        case ARCSIN_TOKEN:
        case ARCCOS_TOKEN:
        case ARCTAN_TOKEN:
        case ARCCOT_TOKEN:
        case ARCSEC_TOKEN:
        case ARCCSC_TOKEN:
        case LOG_TOKEN:
        case EXP_TOKEN:
        case LN_TOKEN:
        case SQRT_TOKEN:
        case ABS_TOKEN:
        case FLOOR_TOKEN:
        case CEIL_TOKEN:
            return true;
        default:
            return false;
    }
}

std::vector<TokenClass> toPostfix(ScannerClass& scanner){
    std::vector<TokenClass> output;
    std::stack<TokenClass> operatorStack;

    TokenClass token = scanner.GetNextToken();
    TokenClass prevToken(BAD_TOKEN, "");
    TokenType type;

    while (token.GetTokenType() != EOF_TOKEN)
    {
        type = token.GetTokenType();

        // Numbers and identifiers go straight to output
        if(type == NUMBER_TOKEN || type == IDENTIFIER_TOKEN){
            output.push_back(token);
        }
        // Constants also go to output as numbers
        else if (type == PI_TOKEN || type == EULER_TOKEN || type == PHI_TOKEN) {
            output.push_back(token);
        }
        // Functions get pushed to operator stack
        else if (isFunction(type)) {
            operatorStack.push(token);
        }
        // Left parenthesis
        else if (type == LPAREN_TOKEN) {
            operatorStack.push(token);
        }
        // Right parenthesis: pop until left parenthesis
        else if (type == RPAREN_TOKEN) {
            while (!operatorStack.empty() && 
                   operatorStack.top().GetTokenType() != LPAREN_TOKEN) {
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }

            if (operatorStack.empty()) {
                throw std::runtime_error("Mismatched parentheses");
            }

            // Pop the left parenthesis (don't add to output)
            operatorStack.pop();

            // If there's a function on top, pop it to output
            if (!operatorStack.empty() && isFunction(operatorStack.top().GetTokenType())) {
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }

        // Operators
        else if (isOperator(type)) {
            // Handle unary minus: convert to (0 - x) by pushing 0 first
            if (type == MINUS_TOKEN) {
                TokenType prevType = prevToken.GetTokenType();
                // Unary if at start, after operator, after left paren, or after comma
                if (prevType == BAD_TOKEN || isOperator(prevType) || 
                    prevType == LPAREN_TOKEN || prevType == COMMA_TOKEN) {
                    output.push_back(TokenClass(NUMBER_TOKEN, "0"));
                }
            }
            
            // Pop operators with higher or equal precedence (respecting associativity)
            while (!operatorStack.empty()) {
                TokenType topType = operatorStack.top().GetTokenType();
                
                if (!isOperator(topType)) break;
                
                int topPrec = getPrecedence(topType);
                int curPrec = getPrecedence(type);
                
                // Pop if: top has higher precedence, OR
                // same precedence AND current is left-associative
                bool shouldPop = (topPrec > curPrec) ||
                                 (topPrec == curPrec && !isRightAssociative(type));
                
                if (!shouldPop) break;
                
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }
            
            operatorStack.push(token);
        }

        // Comma (for multi-argument functions - just pop to left paren)
        else if (type == COMMA_TOKEN) {
            while (!operatorStack.empty() && 
                   operatorStack.top().GetTokenType() != LPAREN_TOKEN) {
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }
        
        prevToken = token;
        token = scanner.GetNextToken();
    }
    
    // Pop remaining operators
    while (!operatorStack.empty()) {
        if (operatorStack.top().GetTokenType() == LPAREN_TOKEN) {
            throw std::runtime_error("Mismatched parentheses");
        }
        output.push_back(operatorStack.top());
        operatorStack.pop();
    }
    
    return output;
}

std::vector<TokenClass> toPostfix(const std::string& equation) {
    ScannerClass scanner(equation, false);
    return toPostfix(scanner);
}
