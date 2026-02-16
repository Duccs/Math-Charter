#include "Scanner.h"

ScannerClass::ScannerClass(const std::string& inputFileName, bool isFileName) {
    if (isFileName) {
        input = new std::ifstream(inputFileName.c_str(), std::ios::binary);
        ownsStream = true;
    } else {
        input = new std::istringstream(inputFileName);
        ownsStream = true;
    }
    if (!input->good()) {
        std::cerr << "Error opening input file " << inputFileName;
        std::exit(1);
    }
    LineNumber = 1;
}

ScannerClass::~ScannerClass() {
    if (ownsStream) {
        delete input;
    }
}

TokenClass ScannerClass::GetNextToken() {
    StateMachineClass stateMachine;
    std::string lexeme;
    MachineState currentState;
    TokenType previousTokenType;

    do
    {
        int c = input->get();
        if (c == '\n') {
            LineNumber++;
        }
        lexeme += c;
        currentState = stateMachine.UpdateState(c, previousTokenType);
        if (currentState == START_STATE || currentState == ENDFILE_STATE)
            lexeme = "";
    } while (currentState != CANTMOVE_STATE);

    if (previousTokenType == BAD_TOKEN)
    {
        throw std::runtime_error("Error. BAD_TOKEN from lexeme " + lexeme + " at line " + std::to_string(LineNumber));
    }

    lexeme.pop_back();
    input->unget();

    TokenClass token(previousTokenType, lexeme);
    
    // Handle multi-character identifiers that aren't reserved words
    // These need to be split into individual variables or reserved word tokens
    if (token.GetTokenType() == IDENTIFIER_TOKEN && lexeme.length() > 1) {
        // Try to find the longest reserved word prefix
        std::string reservedPrefix = "";
        
        for (size_t i = lexeme.length(); i >= 1; i--) {
            std::string prefix = lexeme.substr(0, i);
            TokenClass testToken(IDENTIFIER_TOKEN, prefix);
            
            // If the token type changed, this prefix is a reserved word
            if (testToken.GetTokenType() != IDENTIFIER_TOKEN) {
                reservedPrefix = prefix;
                break;
            }
        }
        
        if (!reservedPrefix.empty()) {
            // Found a reserved word prefix - put back the remaining characters
            for (int i = lexeme.length() - 1; i >= (int)reservedPrefix.length(); i--) {
                input->unget();
                if (lexeme[i] == '\n') {
                    LineNumber--;
                }
            }
            return TokenClass(IDENTIFIER_TOKEN, reservedPrefix);
        } else {
            // No reserved word found - return first character as VARIABLE_TOKEN
            for (int i = lexeme.length() - 1; i >= 1; i--) {
                input->unget();
                if (lexeme[i] == '\n') {
                    LineNumber--;
                }
            }
            return TokenClass(VARIABLE_TOKEN, lexeme.substr(0, 1));
        }
    }

    return token;
}