#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "Tokenizer.h"

enum MachineState {
    START_STATE,                // 0

    // BASIC STATES
    IDENTIFIER_STATE,           // "abcd"
    VARIABLE_STATE,             // "x", "y", "z"
    NUMBER_STATE,               // "123", "3.14", ".5",
    NUMBER_DOT_STATE,           // "1.", "3."
    FLOAT_STATE,                // "1.0", "3.14", ".5"

    // ARITHMETIC STATES
    PLUS_STATE,                 // "+"
    MINUS_STATE,                // "-"
    TIMES_STATE,                // "*"
    DIVIDE_STATE,               // "/"
    CARET_STATE,                // "^"
    UNDERSCORE_STATE,           // "_"
    PIPE_STATE,                 // "|"
    EXCLAMATION_STATE,          // "!"

    // COMPARISON STATES
    EQUAL_STATE,                // "="
    LESS_STATE,                 // "<"
    LESS_EQUAL_STATE,           // "<="
    GREATER_STATE,              // ">"
    GREATER_EQUAL_STATE,        // ">="

    // SEPARATOR STATES
    LPAREN_STATE,               // "("
    RPAREN_STATE,               // ")"
    LCURLY_STATE,               // "{"
    RCURLY_STATE,               // "}"
    COMMA_STATE,                // ","
    SEMICOLON_STATE,            // ";"
    DOT_STATE,                  // "."

    // Special States
    CANTMOVE_STATE,
    ENDFILE_STATE,
    LAST_STATE
};

enum CharacterType{
    // Basic characters
    LETTER_CHAR,
    DIGIT_CHAR,
    WHITESPACE_CHAR,
    DOT_CHAR,

    // Arthmetic characters
    PLUS_CHAR,
    MINUS_CHAR,
    TIMES_CHAR,
    DIVIDE_CHAR,
    CARET_CHAR,
    UNDERSCORE_CHAR,
    PIPE_CHAR,
    EXCLAMATION_CHAR, 

    // Comparison characters
    EQUAL_CHAR,
    LESS_CHAR,
    GREATER_CHAR,

    // Separators
    LPAREN_CHAR,
    RPAREN_CHAR,
    LCURLY_CHAR,
    RCURLY_CHAR,
    COMMA_CHAR,
    SEMICOLON_CHAR,

    // Special characters
    BAD_CHAR,
    ENDFILE_CHAR,
    NEWLINE_CHAR,
    LAST_CHAR
};

class StateMachineClass {
    private:
        MachineState currentState;
        
        // The matrix of legal moves:
        MachineState LegalMoves[LAST_STATE][LAST_CHAR];

        // Which end-machine-states correspond to which token types.
        // non end states correspond to the BAD_TOKEN token type
        TokenType CorrespondingTokenTypes[LAST_STATE];

    public:
        StateMachineClass();
        MachineState UpdateState(int currentCharacter, TokenType &previousTokenType);
};

#endif /* _STATE_MACHINE_H_ */