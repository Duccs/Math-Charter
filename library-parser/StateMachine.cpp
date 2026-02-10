
#include "StateMachine.h"

StateMachineClass::StateMachineClass() {

    currentState = START_STATE;

    // initialize all the LegalMoves to CANTMOVE_STATE
    // -----------------------------------------------
    for (int state = 0; state < LAST_STATE; state++) {
        for (int charType = 0; charType < LAST_CHAR; ++charType) {
            LegalMoves[state][charType] = CANTMOVE_STATE;
        }
    }

    // Define legal transitions
    // ------------------------
    // Basic characters
    LegalMoves[START_STATE][WHITESPACE_CHAR] = START_STATE;
    LegalMoves[START_STATE][NEWLINE_CHAR] = START_STATE;
    LegalMoves[START_STATE][LETTER_CHAR] = IDENTIFIER_STATE;
    LegalMoves[START_STATE][DIGIT_CHAR] = NUMBER_STATE;

    // Arithmetic characters
    LegalMoves[START_STATE][PLUS_CHAR] = PLUS_STATE;
    LegalMoves[START_STATE][MINUS_CHAR] = MINUS_STATE;
    LegalMoves[START_STATE][TIMES_CHAR] = TIMES_STATE;
    LegalMoves[START_STATE][DIVIDE_CHAR] = DIVIDE_STATE;
    LegalMoves[START_STATE][CARET_CHAR] = CARET_STATE;
    LegalMoves[START_STATE][UNDERSCORE_CHAR] = UNDERSCORE_STATE;
    LegalMoves[START_STATE][PIPE_CHAR] = PIPE_STATE;
    LegalMoves[START_STATE][EXCLAMATION_CHAR] = EXCLAMATION_STATE;

    // Comparison characters
    LegalMoves[START_STATE][EQUAL_CHAR] = EQUAL_STATE;
    LegalMoves[START_STATE][LESS_CHAR] = LESS_STATE;
    LegalMoves[START_STATE][GREATER_CHAR] = GREATER_STATE;

    // Separator characters
    LegalMoves[START_STATE][LPAREN_CHAR] = LPAREN_STATE;
    LegalMoves[START_STATE][RPAREN_CHAR] = RPAREN_STATE;
    LegalMoves[START_STATE][LCURLY_CHAR] = LCURLY_STATE;
    LegalMoves[START_STATE][RCURLY_CHAR] = RCURLY_STATE;
    LegalMoves[START_STATE][COMMA_CHAR] = COMMA_STATE;
    LegalMoves[START_STATE][SEMICOLON_CHAR] = SEMICOLON_STATE;

    // Two character comparison operators
    LegalMoves[LESS_STATE][EQUAL_CHAR] = LESS_EQUAL_STATE;
    LegalMoves[GREATER_STATE][EQUAL_CHAR] = GREATER_EQUAL_STATE;

    // Looping States
    LegalMoves[IDENTIFIER_STATE][LETTER_CHAR] = IDENTIFIER_STATE;
    LegalMoves[NUMBER_STATE][DIGIT_CHAR] = NUMBER_STATE;
    LegalMoves[NUMBER_STATE][DOT_CHAR] = NUMBER_STATE;

    // EOF State
    LegalMoves[START_STATE][ENDFILE_CHAR] = ENDFILE_STATE;
 
    // Initialize corresponding token types
    // ------------------------------------
    for(int i = 0; i < LAST_STATE; i++) {
        CorrespondingTokenTypes[i] = BAD_TOKEN;
    }
    
    CorrespondingTokenTypes[IDENTIFIER_STATE] = IDENTIFIER_TOKEN;
    CorrespondingTokenTypes[NUMBER_STATE] = NUMBER_TOKEN;
    
    // Arithmetic token types
    CorrespondingTokenTypes[PLUS_STATE] = PLUS_TOKEN;
    CorrespondingTokenTypes[MINUS_STATE] = MINUS_TOKEN;
    CorrespondingTokenTypes[TIMES_STATE] = TIMES_TOKEN;
    CorrespondingTokenTypes[DIVIDE_STATE] = DIVIDE_TOKEN;
    CorrespondingTokenTypes[CARET_STATE] = EXP_TOKEN;
    CorrespondingTokenTypes[UNDERSCORE_STATE] = SUBSCRIPT_TOKEN;
    CorrespondingTokenTypes[PIPE_STATE] = PIPE_TOKEN;
    CorrespondingTokenTypes[EXCLAMATION_STATE] = FACTORIAL_TOKEN;
    
    // Comparison token types
    CorrespondingTokenTypes[EQUAL_STATE] = EQUAL_TOKEN;
    CorrespondingTokenTypes[LESS_STATE] = LESS_TOKEN;
    CorrespondingTokenTypes[LESS_EQUAL_STATE] = LESSEQUAL_TOKEN;
    CorrespondingTokenTypes[GREATER_STATE] = GREATER_TOKEN;
    CorrespondingTokenTypes[GREATER_EQUAL_STATE] = GREATEREQUAL_TOKEN;
    
    // Separator token types
    CorrespondingTokenTypes[LPAREN_STATE] = LPAREN_TOKEN;
    CorrespondingTokenTypes[RPAREN_STATE] = RPAREN_TOKEN;
    CorrespondingTokenTypes[LCURLY_STATE] = LCURLY_TOKEN;
    CorrespondingTokenTypes[RCURLY_STATE] = RCURLY_TOKEN;
    CorrespondingTokenTypes[COMMA_STATE] = COMMA_TOKEN;
    CorrespondingTokenTypes[SEMICOLON_STATE] = SEMICOLON_TOKEN;
    
    CorrespondingTokenTypes[ENDFILE_STATE] = EOF_TOKEN;
    CorrespondingTokenTypes[CANTMOVE_STATE] = BAD_TOKEN;
    CorrespondingTokenTypes[START_STATE] = BAD_TOKEN;
}

MachineState StateMachineClass::UpdateState(int currentCharacter, TokenType &previousTokenType) {
    CharacterType charType = BAD_CHAR;

    // Determine character type
    if (isalpha(currentCharacter)) {
        charType = LETTER_CHAR;
    } else if (isdigit(currentCharacter)) {
        charType = DIGIT_CHAR;
    } else if (currentCharacter == '\n') {
        charType = NEWLINE_CHAR;
    } else if (isspace(currentCharacter)) {
        charType = WHITESPACE_CHAR;
    } else if (currentCharacter == '.') {
        charType = DOT_CHAR;
    } else if (currentCharacter == '+') {
        charType = PLUS_CHAR;
    } else if (currentCharacter == '-') {
        charType = MINUS_CHAR;
    } else if (currentCharacter == '*') {
        charType = TIMES_CHAR;
    } else if (currentCharacter == '/') {
        charType = DIVIDE_CHAR;
    } else if (currentCharacter == '^') {
        charType = CARET_CHAR;
    } else if (currentCharacter == '_') {
        charType = UNDERSCORE_CHAR;
    } else if (currentCharacter == '|') {
        charType = PIPE_CHAR;
    } else if (currentCharacter == '!') {
        charType = EXCLAMATION_CHAR;
    } else if (currentCharacter == '=') {
        charType = EQUAL_CHAR;
    } else if (currentCharacter == '<') {
        charType = LESS_CHAR;
    } else if (currentCharacter == '>') {
        charType = GREATER_CHAR;
    } else if (currentCharacter == ';') {
        charType = SEMICOLON_CHAR;
    } else if (currentCharacter == '(') {
        charType = LPAREN_CHAR;
    } else if (currentCharacter == ')') {
        charType = RPAREN_CHAR;
    } else if (currentCharacter == '{') {
        charType = LCURLY_CHAR;
    } else if (currentCharacter == '}') {
        charType = RCURLY_CHAR;
    } else if (currentCharacter == ',') {
        charType = COMMA_CHAR;
    } else if (currentCharacter == EOF) {
        charType = ENDFILE_CHAR;
    }

    // Update previous token type
    previousTokenType = CorrespondingTokenTypes[currentState];
    
    // Update state based on legal moves
    currentState = LegalMoves[currentState][charType];

    return currentState;
}