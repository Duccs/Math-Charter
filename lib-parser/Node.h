#ifndef _NODE_H_
#define _NODE_H_

#include "Tokenizer.h"
#include "SymbolTable.h"
#include <memory>
#include <cmath>
#include <limits>

// Mathematical constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#ifndef M_PHI
#define M_PHI 1.61803398874989484820
#endif

// Forward declarations
class Node;

// Type aliases for function pointers
using UnaryFunc = float(*)(float);
using BinaryFunc = float(*)(float, float);

// Abstract base class
class Node {
public:
    virtual ~Node() = default;
    virtual float evaluate(SymbolTable& symbols) const = 0;
};

class ConstantNode : public Node {
private:
    float value;
public:
    explicit ConstantNode(float val) : value(val) {}
    float evaluate(SymbolTable& symbols) const override { return value; }
};

class VariableNode : public Node {
private:
    std::string name;
public:
    explicit VariableNode(const std::string& varName) : name(varName) {}
    float evaluate(SymbolTable& symbols) const override {
        return symbols.GetValue(name);
    }
    const std::string& getName() const { return name; }
};

class UnaryOpNode : public Node {
private:
    std::unique_ptr<Node> operand;
    UnaryFunc operation;
public:
    UnaryOpNode(std::unique_ptr<Node> op, UnaryFunc func)
        : operand(std::move(op)), operation(func) {}
    
    float evaluate(SymbolTable& symbols) const override {
        return operation(operand->evaluate(symbols));
    }
};

class BinaryOpNode : public Node {
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    BinaryFunc operation;
public:
    BinaryOpNode(std::unique_ptr<Node> l, std::unique_ptr<Node> r, BinaryFunc func)
        : left(std::move(l)), right(std::move(r)), operation(func) {}
    
    float evaluate(SymbolTable& symbols) const override {
        return operation(left->evaluate(symbols), right->evaluate(symbols));
    }
};

// Factory functions
UnaryFunc getUnaryOp(TokenType type);
BinaryFunc getBinaryOp(TokenType type);

std::unique_ptr<Node> makeUnaryNode(TokenType type, std::unique_ptr<Node> operand);
std::unique_ptr<Node> makeBinaryNode(TokenType type, std::unique_ptr<Node> left, std::unique_ptr<Node> right);

#endif /* _NODE_H_ */
