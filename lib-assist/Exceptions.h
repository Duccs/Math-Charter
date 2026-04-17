#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdexcept>
#include <string>

// Exception thrown when evaluating an undefined variable
class UndefinedVariableException : public std::runtime_error {
public:
    explicit UndefinedVariableException(const std::string& varName)
        : std::runtime_error("Undefined variable '" + varName + "'"),
          variableName(varName) {}

    const std::string& getVariableName() const { return variableName; }

private:
    std::string variableName;
};

// Exception thrown when an expression cannot be drawn
class InvalidExpressionException : public std::runtime_error {
public:
    explicit InvalidExpressionException(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception thrown for syntax errors during parsing
class SyntaxException : public std::runtime_error {
public:
    SyntaxException(const std::string& message, int lineNumber)
        : std::runtime_error(message + " at line " + std::to_string(lineNumber)),
          line(lineNumber) {}

    int getLineNumber() const { return line; }

private:
    int line;
};

#endif /* _EXCEPTIONS_H_ */
