#include "Parser.h"
#include <stack>
#include <stdexcept>


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

void Parser::advance() {
    currentToken = scanner.GetNextToken();
}
    
TokenClass Parser::peek() {
    return scanner.PeekNextToken();
}
    
bool Parser::match(TokenType type) {
    return currentToken.GetTokenType() == type;
}
    
void Parser::expect(TokenType type, const std::string& message) {
    if (!match(type)) {
        throw std::runtime_error(message + " at line " + std::to_string(scanner.GetLineNumber()));
    }
    advance();
}
    
// + and -
std::unique_ptr<Node> Parser::parseAdditive() {
    auto left = parseMultiplicative();
    
    while (match(PLUS_TOKEN) || match(MINUS_TOKEN)) {
        TokenType op = currentToken.GetTokenType();
        advance();
        auto right = parseMultiplicative();
        left = makeBinaryNode(op, std::move(left), std::move(right));
    }
    
    return left;
}
    
// * and /
std::unique_ptr<Node> Parser::parseMultiplicative() {
    auto left = parseExponent();
    
    while (match(TIMES_TOKEN) || match(DIVIDE_TOKEN)) {
        TokenType op = currentToken.GetTokenType();
        advance();
        auto right = parseExponent();
        left = makeBinaryNode(op, std::move(left), std::move(right));
    }
    
    return left;
}

// ^
std::unique_ptr<Node> Parser::parseExponent() {
    auto left = parseUnary();
    
    if (match(EXP_TOKEN)) {
        advance();
        auto right = parseExponent();  // Right-associative: recurse on same level
        left = makeBinaryNode(EXP_TOKEN, std::move(left), std::move(right));
    }
    
    return left;
}

// unary minus
std::unique_ptr<Node> Parser::parseUnary() {
    if (match(MINUS_TOKEN)) {
        advance();
        auto operand = parseUnary();
        return makeUnaryNode(MINUS_TOKEN, std::move(operand));
    }
    
    return parsePostfix();
}

// !
std::unique_ptr<Node> Parser::parsePostfix() {
    auto operand = parsePrimary();
    
    while (match(FACTORIAL_TOKEN)) {
        advance();
        operand = makeUnaryNode(FACTORIAL_TOKEN, std::move(operand));
    }
    
    return operand;
}
    
// numbers, variables, constants, functions, parentheses
std::unique_ptr<Node> Parser::parsePrimary() {
    TokenType type = currentToken.GetTokenType();
    
    // Number literal
    if (match(NUMBER_TOKEN)) {
        float value = std::stof(currentToken.GetLexeme());
        advance();
        return std::make_unique<ConstantNode>(value);
    }
    
    // Variable
    if (match(VARIABLE_TOKEN)) {
        std::string name = currentToken.GetLexeme();
        advance();
        return std::make_unique<VariableNode>(name);
    }
    
    // Constants
    if (match(PI_TOKEN)) {
        advance();
        return std::make_unique<ConstantNode>(static_cast<float>(M_PI));
    }
    if (match(EULER_TOKEN)) {
        advance();
        return std::make_unique<ConstantNode>(static_cast<float>(M_E));
    }
    if (match(PHI_TOKEN)) {
        advance();
        return std::make_unique<ConstantNode>(static_cast<float>(M_PHI));
    }
    if (match(INFINITY_TOKEN)) {
        advance();
        return std::make_unique<ConstantNode>(std::numeric_limits<float>::infinity());
    }
    if (match(NAN_TOKEN)) {
        advance();
        return std::make_unique<ConstantNode>(std::numeric_limits<float>::quiet_NaN());
    }
    
    // Functions
    if (isFunction(type)) {
        TokenType funcType = type;
        advance();
        expect(LPAREN_TOKEN, "Expected '(' after function name");
        auto arg = parseAdditive();
        expect(RPAREN_TOKEN, "Expected ')' after function argument");
        return makeUnaryNode(funcType, std::move(arg));
    }
    
    // Parenthesized expression
    if (match(LPAREN_TOKEN)) {
        advance();
        auto expr = parseAdditive();
        expect(RPAREN_TOKEN, "Expected ')' to close parenthesis");
        return expr;
    }
    
    // Pipe for absolute value |x|
    if (match(PIPE_TOKEN)) {
        advance();
        auto expr = parseAdditive();
        expect(PIPE_TOKEN, "Expected '|' to close absolute value");
        return makeUnaryNode(ABS_TOKEN, std::move(expr));
    }
    
    throw std::runtime_error("Unexpected token: " + currentToken.GetLexeme() + 
                                " at line " + std::to_string(scanner.GetLineNumber()));
}
    
Parser::Parser(ScannerClass& sc) : scanner(sc), currentToken(EOF_TOKEN, "") {
    advance();  // Load first token
}
    
std::unique_ptr<Node> Parser::parse() {
    if (match(EOF_TOKEN)) {
        throw std::runtime_error("Empty expression");
    }
    
    auto ast = parseAdditive();
    
    if (!match(EOF_TOKEN)) {
        throw std::runtime_error("Unexpected token after expression: " + currentToken.GetLexeme());
    }
    
    return ast;
}

std::unique_ptr<Node> parseToAST(const std::string& equation) {
    ScannerClass scanner(equation, false);
    Parser parser(scanner);
    return parser.parse();
}
