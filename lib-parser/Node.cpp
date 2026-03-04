#include "Node.h"
#include <unordered_map>
#include <stdexcept>


// Unary operation implementations
// -------------------------------
static float op_sin(float a) { return std::sin(a); }
static float op_cos(float a) { return std::cos(a); }
static float op_tan(float a) { return std::tan(a); }
static float op_cot(float a) { 
    float t = std::tan(a);
    return t != 0.0f ? 1.0f / t : 0.0f;
}
static float op_sec(float a) {
    float c = std::cos(a);
    return c != 0.0f ? 1.0f / c : 0.0f;
}
static float op_csc(float a) {
    float s = std::sin(a);
    return s != 0.0f ? 1.0f / s : 0.0f;
}
static float op_arcsin(float a) { return std::asin(a); }
static float op_arccos(float a) { return std::acos(a); }
static float op_arctan(float a) { return std::atan(a); }
static float op_arccot(float a) { return static_cast<float>(M_PI / 2.0) - std::atan(a); }
static float op_arcsec(float a) { return std::acos(1.0f / a); }
static float op_arccsc(float a) { return std::asin(1.0f / a); }
static float op_log(float a) { return std::log10(a); }
static float op_ln(float a) { return std::log(a); }
static float op_sqrt(float a) { return std::sqrt(a); }
static float op_abs(float a) { return std::abs(a); }
static float op_floor(float a) { return std::floor(a); }
static float op_ceil(float a) { return std::ceil(a); }
static float op_factorial(float a) { return std::tgamma(a + 1.0f); }
static float op_negate(float a) { return -a; }


// Binary operation implementations
// --------------------------------
static float op_add(float a, float b) { return a + b; }
static float op_sub(float a, float b) { return a - b; }
static float op_mul(float a, float b) { return a * b; }
static float op_div(float a, float b) {
    if (b != 0.0f) {
        return a / b;
    }
    // Handle division by zero - return appropriate infinity or NaN
    if (a > 0.0f) return std::numeric_limits<float>::infinity();
    if (a < 0.0f) return -std::numeric_limits<float>::infinity();
    return std::numeric_limits<float>::quiet_NaN();
}
static float op_pow(float a, float b) { return std::pow(a, b); }


// Dispatch tables
// ---------------
static const std::unordered_map<TokenType, UnaryFunc> unaryOps = {
    {SIN_TOKEN, op_sin},
    {COS_TOKEN, op_cos},
    {TAN_TOKEN, op_tan},
    {COT_TOKEN, op_cot},
    {SEC_TOKEN, op_sec},
    {CSC_TOKEN, op_csc},
    {ARCSIN_TOKEN, op_arcsin},
    {ARCCOS_TOKEN, op_arccos},
    {ARCTAN_TOKEN, op_arctan},
    {ARCCOT_TOKEN, op_arccot},
    {ARCSEC_TOKEN, op_arcsec},
    {ARCCSC_TOKEN, op_arccsc},
    {LOG_TOKEN, op_log},
    {LN_TOKEN, op_ln},
    {SQRT_TOKEN, op_sqrt},
    {ABS_TOKEN, op_abs},
    {FLOOR_TOKEN, op_floor},
    {CEIL_TOKEN, op_ceil},
    {FACTORIAL_TOKEN, op_factorial},
    {MINUS_TOKEN, op_negate}  // For unary minus
};

static const std::unordered_map<TokenType, BinaryFunc> binaryOps = {
    {PLUS_TOKEN, op_add},
    {MINUS_TOKEN, op_sub},
    {TIMES_TOKEN, op_mul},
    {DIVIDE_TOKEN, op_div},
    {EXP_TOKEN, op_pow}
};


// Factory functions
// -----------------
UnaryFunc getUnaryOp(TokenType type) {
    auto it = unaryOps.find(type);
    if (it != unaryOps.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown unary operator: " + TokenClass::GetTokenTypeName(type));
}

BinaryFunc getBinaryOp(TokenType type) {
    auto it = binaryOps.find(type);
    if (it != binaryOps.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown binary operator: " + TokenClass::GetTokenTypeName(type));
}

std::unique_ptr<Node> makeUnaryNode(TokenType type, std::unique_ptr<Node> operand) {
    return std::make_unique<UnaryOpNode>(std::move(operand), getUnaryOp(type));
}

std::unique_ptr<Node> makeBinaryNode(TokenType type, std::unique_ptr<Node> left, std::unique_ptr<Node> right) {
    return std::make_unique<BinaryOpNode>(std::move(left), std::move(right), getBinaryOp(type));
}
