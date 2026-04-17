#ifndef _PARSER_H
#define _PARSER_H

#include "Tokenizer.h"
#include "Scanner.h"
#include "Node.h"
#include <config.h>
#include <vector>
#include <string>
#include <set>
#include <memory>


bool isFunction(TokenType type);

std::unique_ptr<Node> parseToAST(const std::string& equation);

// Extended version that also returns the variables used in the expression
std::unique_ptr<Node> parseToAST(const std::string& equation, std::set<std::string>& outVariables, std::string& outAssignmentTarget);


class Parser {
    private:
        ScannerClass& scanner;
        
        TokenClass currentToken;
        
        std::set<std::string> usedVariables;    // Track variables encountered during parsing
        std::string assignmentTarget;           // Empty if not an assignment
        
        void advance();
        TokenClass peek();
        bool match(TokenType type);
        
        void expect(TokenType type, const std::string& message);

        std::unique_ptr<Node> parseStatement();
        std::unique_ptr<Node> parseAdditive();
        std::unique_ptr<Node> parseMultiplicative();
        std::unique_ptr<Node> parseExponent();
        std::unique_ptr<Node> parseUnary();
        std::unique_ptr<Node> parsePostfix();
        std::unique_ptr<Node> parsePrimary();


    public:
        explicit Parser(ScannerClass& sc);
        std::unique_ptr<Node> parse();
        
        // Accessors for parse results
        const std::set<std::string>& getUsedVariables() const { return usedVariables; }
        const std::string& getAssignmentTarget() const { return assignmentTarget; }

};



#endif /* _PARSER_H */