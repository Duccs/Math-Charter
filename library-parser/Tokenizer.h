#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <ostream>

enum TokenType {

    // Reserved Words and Constants:
    PI_TOKEN, EULER_TOKEN, PHI_TOKEN,
    INFINITY_TOKEN, NAN_TOKEN,
    
    // Operations Tokens:
        // Trigonometric Tokens:
    SIN_TOKEN, COS_TOKEN, TAN_TOKEN, 
    COT_TOKEN, SEC_TOKEN, CSC_TOKEN,
    ARCSIN_TOKEN, ARCCOS_TOKEN, ARCTAN_TOKEN,
    ARCCOT_TOKEN, ARCSEC_TOKEN, ARCCSC_TOKEN,
        // Function Tokens:
    LOG_TOKEN, EXP_TOKEN, LN_TOKEN, SQRT_TOKEN, FACTORIAL_TOKEN,
    ABS_TOKEN, FLOOR_TOKEN, CEIL_TOKEN,
    SUBSCRIPT_TOKEN, PIPE_TOKEN,

    // Arithmetic Tokens:
    PLUS_TOKEN, MINUS_TOKEN, TIMES_TOKEN, DIVIDE_TOKEN,

    // Relational Tokens:
    EQUAL_TOKEN, LESS_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, 
    GREATEREQUAL_TOKEN,

    // Separator Tokens:
    LPAREN_TOKEN, RPAREN_TOKEN, LCURLY_TOKEN, RCURLY_TOKEN, 
    COMMA_TOKEN, SEMICOLON_TOKEN,

    // Additional Tokens:
    VARIABLE_TOKEN, IDENTIFIER_TOKEN, NUMBER_TOKEN,
    BAD_TOKEN, EOF_TOKEN, LAST_TOKEN

};

// IMPORTANT: The list above and the list below MUST be kept in sync.
const std::string gTokenTypeNames[] = {

    // Reserved Words and Constants:
    "PI", "EULER", "PHI",
    "INFINITY", "NAN",
    
    // Operations Tokens:
        // Trigonometric Tokens:
    "SINE", "COSINE", "TANGENT", 
    "COTANGENT", "SECANT", "COSECANT",
    "ARCSINE", "ARCCOSINE", "ARCTANGENT",
    "ARCCOTANGENT", "ARCSECANT", "ARCCOSECANT",
        // Function Tokens:
    "LOGARITHM", "EXPONENT", "NATURALLOG", "SQUAREROOT", "FACTORIAL",
    "ABSOLUTE", "FLOOR", "CEILING",
    "SUBSCRIPT", "PIPE",

    // Arithmetic Tokens:
    "PLUS", "MINUS", "TIMES", "DIVIDE",

    // Relational Tokens:
    "EQUAL", "LESS", "LESSEQUAL", "GREATER", 
    "GREATEREQUAL",

    // Separator Tokens:
    "LPAREN", "RPAREN", "LCURLY", "RCURLY", 
    "COMMA", "SEMICOLON",

    // Additional Tokens:
    "VARIABLE", "IDENTIFIER", "NUMBER",
    "BAD", "EOF"
};

static_assert(sizeof(gTokenTypeNames)/sizeof(gTokenTypeNames[0]) == LAST_TOKEN,
              "TokenType enum and gTokenTypeNames array are out of sync!");

class TokenClass {
    private:
        TokenType tokenType;
        std::string tokenLexeme;

    public:
        TokenClass(TokenType type, const std::string &lexeme);
        TokenType GetTokenType() const { return tokenType; };
        const std::string& GetTokenTypeName() const {
            return gTokenTypeNames[tokenType];
        };
        std::string GetLexeme() const { return tokenLexeme; };
        static const std::string& GetTokenTypeName(TokenType type) {
            return gTokenTypeNames[type];
        };

    
};

std::ostream & operator<<(std::ostream & out, const TokenClass & tc);

#endif /* _TOKENIZER_H_ */