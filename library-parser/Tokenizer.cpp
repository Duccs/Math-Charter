#include "Tokenizer.h"

TokenClass::TokenClass(TokenType type, const std::string &lexeme)
    : tokenType(type), tokenLexeme(lexeme) {
        //Check for reserved words
        if (lexeme == "pi"){
            tokenType = PI_TOKEN;
        } else if (lexeme == "e"){
            tokenType = EULER_TOKEN;
        } else if (lexeme == "phi"){
            tokenType = PHI_TOKEN;
        } else if (lexeme == "inf"){
            tokenType = INFINITY_TOKEN;
        } else if (lexeme == "nan"){
            tokenType = NAN_TOKEN;
        } else if (lexeme == "sin"){
            tokenType = SIN_TOKEN;
        } else if (lexeme == "cos"){
            tokenType = COS_TOKEN;
        } else if (lexeme == "tan"){
            tokenType = TAN_TOKEN;
        } else if (lexeme == "cot"){
            tokenType = COT_TOKEN;
        } else if (lexeme == "sec"){
            tokenType = SEC_TOKEN;
        } else if (lexeme == "csc"){
            tokenType = CSC_TOKEN;
        } else if (lexeme == "arcsin"){
            tokenType = ARCSIN_TOKEN;
        } else if (lexeme == "arccos"){
            tokenType = ARCCOS_TOKEN;
        } else if (lexeme == "arctan"){
            tokenType = ARCTAN_TOKEN;
        } else if (lexeme == "arccot"){
            tokenType = ARCCOT_TOKEN;
        } else if (lexeme == "arcsec"){
            tokenType = ARCSEC_TOKEN;
        } else if (lexeme == "arccsc"){
            tokenType = ARCCSC_TOKEN;
        } else if (lexeme == "log"){
            tokenType = LOG_TOKEN;
        } else if (lexeme == "ln"){
            tokenType = LN_TOKEN;
        } else if (lexeme == "sqrt"){
            tokenType = SQRT_TOKEN;
        } else if (lexeme == "abs"){
            tokenType = ABS_TOKEN;
        } else if (lexeme == "floor"){
            tokenType = FLOOR_TOKEN;
        } else if (lexeme == "ceil"){
            tokenType = CEIL_TOKEN;
        }
    }

std::ostream & operator<<(std::ostream & out, const TokenClass & tc) {
    out << "Type=" << static_cast<int>(tc.GetTokenType())
        << " Name=" << tc.GetTokenTypeName()
        << " Lexeme=\"" << tc.GetLexeme() << "\"";
    return out;
}
