#include "Scanner.h"

ScannerClass::ScannerClass(const std::string& inputFileName) {
    Fin.open(inputFileName.c_str(), std::ios::binary);
    if (!Fin) {
        std::cerr << "Error opening input file " << inputFileName;
        std::exit(1);
    }
    LineNumber = 1;
}

ScannerClass::~ScannerClass() {
    if (Fin.is_open()) {
        Fin.close();
    }
}

TokenClass ScannerClass::GetNextToken() {
    StateMachineClass stateMachine;
    std::string lexeme;
    MachineState currentState;
    TokenType previousTokenType;

    do
    {
        int c = Fin.get();
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
        std::cerr << "Error. BAD_TOKEN from lexeme " << lexeme << " at line " << LineNumber;
        std::exit(1);
    }

    lexeme.pop_back();
    Fin.unget();

    return TokenClass(previousTokenType, lexeme);
}