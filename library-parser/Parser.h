#ifndef _PARSER_H
#define _PARSER_H

#include "Tokenizer.h"
#include "Scanner.h"
#include <config.h>
#include <vector>
#include <string>

int getPrecedence(TokenType type);

// Check if operator is right-associative (e.g., ^)
bool isRightAssociative(TokenType type);
bool isOperator(TokenType type);
bool isFunction(TokenType type);

// Convert infix token stream to postfix
std::vector<TokenClass> toPostfix(ScannerClass& scanner);
// Parse from string directly
std::vector<TokenClass> toPostfix(const std::string& equation);

#endif /* _PARSER_H */
