#ifndef _PARSER_H
#define _PARSER_H

#include "Tokenizer.h"
#include "Scanner.h"
#include "Node.h"
#include <config.h>
#include <vector>
#include <string>
#include <memory>


bool isFunction(TokenType type);

std::unique_ptr<Node> parseToAST(const std::string& equation);


class Parser {
    private:
        ScannerClass& scanner;
        TokenClass currentToken;
        
        void advance();
        TokenClass peek();
        bool match(TokenType type);
        
        void expect(TokenType type, const std::string& message);

        std::unique_ptr<Node> parseAdditive();
        std::unique_ptr<Node> parseMultiplicative();
        std::unique_ptr<Node> parseExponent();
        std::unique_ptr<Node> parseUnary();
        std::unique_ptr<Node> parsePostfix();
        std::unique_ptr<Node> parsePrimary();


    public:
        explicit Parser(ScannerClass& sc);
        std::unique_ptr<Node> parse();

};



#endif /* _PARSER_H */
