#include "Expression.h"

// Mathematical constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// Golden ratio
#define M_PHI 1.61803398874989484820

Expression::Expression() : valid(false) {}

Expression Expression::parse(const std::string& equation) {
    Expression expr;
    
    try {
        expr.postfix = toPostfix(equation);
        expr.valid = true;
    } catch (const std::exception& e) {
        expr.valid = false;
        expr.errorMessage = e.what();
    }
    
    return expr;
}

float Expression::evaluate(float x) const {
    
    if (!valid || postfix.empty()) {
        WARN("IN:'Expression.cpp evaluate()' Cannot evaluate invalid or empty expression");
        return 0.0f;
    }
    
    std::stack<float> evalStack;
    
    for (const TokenClass& token : postfix) {
        TokenType type = token.GetTokenType();
        
        // Numbers: parse and push
        if (type == NUMBER_TOKEN) {
            evalStack.push(std::stof(token.GetLexeme()));
        }
        // Warn about unhandled identifier type
        else if (type == IDENTIFIER_TOKEN) {
            WARN("IN:'Expression.cpp evaluate()' Unhandled identifier token: " + token.GetLexeme());
        }
        // Variable x: push the x value
        else if (type == VARIABLE_TOKEN) {
            // Assume all variables are x for now
            // Could be extended to support multiple variables
            evalStack.push(x);
        }
        // Constants
        else if (type == PI_TOKEN) {
            evalStack.push(static_cast<float>(M_PI));
        }
        else if (type == EULER_TOKEN) {
            evalStack.push(static_cast<float>(M_E));
        }
        else if (type == PHI_TOKEN) {
            evalStack.push(static_cast<float>(M_PHI));
        }
        // Binary operators: pop two, compute, push result
        else if (type == PLUS_TOKEN) {
            if (evalStack.size() < 2) throw std::runtime_error("Stack underflow");
            float b = evalStack.top(); evalStack.pop();
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(a + b);
        }
        else if (type == MINUS_TOKEN) {
            if (evalStack.size() < 2) throw std::runtime_error("Stack underflow");
            float b = evalStack.top(); evalStack.pop();
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(a - b);
        }
        else if (type == TIMES_TOKEN) {
            if (evalStack.size() < 2) throw std::runtime_error("Stack underflow");
            float b = evalStack.top(); evalStack.pop();
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(a * b);
        }
        else if (type == DIVIDE_TOKEN) {
            if (evalStack.size() < 2) throw std::runtime_error("Stack underflow");
            float b = evalStack.top(); evalStack.pop();
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(b != 0.0f ? a / b : 0.0f);
        }
        else if (type == EXP_TOKEN) {
            // Using ^ as exponentiation operator
            if (evalStack.size() < 2) throw std::runtime_error("Stack underflow");
            float b = evalStack.top(); evalStack.pop();
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::pow(a, b));
        }
        // Unary functions: pop one, compute, push result
        else if (type == SIN_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::sin(a));
        }
        else if (type == COS_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::cos(a));
        }
        else if (type == TAN_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::tan(a));
        }
        else if (type == COT_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            float tanVal = std::tan(a);
            evalStack.push(tanVal != 0.0f ? 1.0f / tanVal : 0.0f);
        }
        else if (type == SEC_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            float cosVal = std::cos(a);
            evalStack.push(cosVal != 0.0f ? 1.0f / cosVal : 0.0f);
        }
        else if (type == CSC_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            float sinVal = std::sin(a);
            evalStack.push(sinVal != 0.0f ? 1.0f / sinVal : 0.0f);
        }
        else if (type == ARCSIN_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::asin(a));
        }
        else if (type == ARCCOS_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::acos(a));
        }
        else if (type == ARCTAN_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::atan(a));
        }
        else if (type == ARCCOT_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(static_cast<float>(M_PI / 2.0) - std::atan(a));
        }
        else if (type == ARCSEC_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::acos(1.0f / a));
        }
        else if (type == ARCCSC_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::asin(1.0f / a));
        }
        else if (type == LOG_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::log10(a));
        }
        else if (type == LN_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::log(a));
        }
        else if (type == FACTORIAL_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::tgamma(a + 1.0f));
        }
        else if (type == SQRT_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::sqrt(a));
        }
        else if (type == ABS_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::abs(a));
        }
        else if (type == FLOOR_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::floor(a));
        }
        else if (type == CEIL_TOKEN) {
            if (evalStack.empty()) throw std::runtime_error("Stack underflow");
            float a = evalStack.top(); evalStack.pop();
            evalStack.push(std::ceil(a));
        }
    }
    
    if (evalStack.empty()) {
        return 0.0f;
    }
    
    return evalStack.top();
}

std::string Expression::toPostfixString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < postfix.size(); ++i) {
        if (i > 0) oss << " ";
        oss << postfix[i].GetLexeme();
    }
    return oss.str();
}
