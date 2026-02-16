#include <gtest/gtest.h>
#include "Scanner.h"
#include "Tokenizer.h"

// Test fixture for Scanner tests
class ScannerTest : public ::testing::Test {
protected:
    // Helper function to get all tokens from input
    std::vector<TokenClass> GetAllTokens(const std::string& input) {
        ScannerClass scanner(input, false);
        std::vector<TokenClass> tokens;
        
        TokenClass token = scanner.GetNextToken();
        while (token.GetTokenType() != EOF_TOKEN) {
            tokens.push_back(token);
            token = scanner.GetNextToken();
        }
        
        return tokens;
    }
};

// Tokenization tests
TEST_F(ScannerTest, SingleNumber) {
    ScannerClass scanner("9876543210", false);
    TokenClass token = scanner.GetNextToken();
    
    EXPECT_EQ(token.GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(token.GetLexeme(), "9876543210");
}

TEST_F(ScannerTest, FloatingPointNumber) {
    ScannerClass scanner("3.14159", false);
    TokenClass token = scanner.GetNextToken();
    
    EXPECT_EQ(token.GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(token.GetLexeme(), "3.14159");
}

TEST_F(ScannerTest, BadFloatingPointNumber) {
    ScannerClass scanner("3.14.15.9", false);
    TokenClass token = scanner.GetNextToken();
    
    EXPECT_ANY_THROW(scanner.GetNextToken());
}

TEST_F(ScannerTest, DoubleDotFloatingPoint) {
    ScannerClass scanner("3..14", false);
    
    EXPECT_ANY_THROW(scanner.GetNextToken());
}

TEST_F(ScannerTest, SimpleAddition) {
    auto tokens = GetAllTokens("3 + 5");
    
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(tokens[0].GetLexeme(), "3");
    EXPECT_EQ(tokens[1].GetTokenType(), PLUS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(tokens[2].GetLexeme(), "5");
}

TEST_F(ScannerTest, ParenthesesExpression) {
    auto tokens = GetAllTokens("(2 + 3)");
    
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].GetTokenType(), LPAREN_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), PLUS_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), NUMBER_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), RPAREN_TOKEN);
}

TEST_F(ScannerTest, MathConstants) {
    auto tokens = GetAllTokens("pi e phi inf nan");
    
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].GetTokenType(), PI_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), EULER_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), PHI_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), INFINITY_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), NAN_TOKEN);
}

TEST_F(ScannerTest, TrigonometricFunctions) {
    auto tokens = GetAllTokens("sin cos tan cot sec csc arcsin arccos arctan arccot arcsec arccsc");
    
    ASSERT_EQ(tokens.size(), 12);
    EXPECT_EQ(tokens[0].GetTokenType(), SIN_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), COS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), TAN_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), COT_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), SEC_TOKEN);
    EXPECT_EQ(tokens[5].GetTokenType(), CSC_TOKEN);
    EXPECT_EQ(tokens[6].GetTokenType(), ARCSIN_TOKEN);
    EXPECT_EQ(tokens[7].GetTokenType(), ARCCOS_TOKEN);
    EXPECT_EQ(tokens[8].GetTokenType(), ARCTAN_TOKEN);
    EXPECT_EQ(tokens[9].GetTokenType(), ARCCOT_TOKEN);
    EXPECT_EQ(tokens[10].GetTokenType(), ARCSEC_TOKEN);
    EXPECT_EQ(tokens[11].GetTokenType(), ARCCSC_TOKEN);
}

TEST_F(ScannerTest, MathFunctions) {
    auto tokens = GetAllTokens("log ln sqrt abs floor ceil");
    
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].GetTokenType(), LOG_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), LN_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), SQRT_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), ABS_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), FLOOR_TOKEN);
    EXPECT_EQ(tokens[5].GetTokenType(), CEIL_TOKEN);
}

TEST_F(ScannerTest, ArithmeticOperators) {
    auto tokens = GetAllTokens("+ - * / ^ _ | !");
    
    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].GetTokenType(), PLUS_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), MINUS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), TIMES_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), DIVIDE_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), EXP_TOKEN);
    EXPECT_EQ(tokens[5].GetTokenType(), SUBSCRIPT_TOKEN);
    EXPECT_EQ(tokens[6].GetTokenType(), PIPE_TOKEN);
    EXPECT_EQ(tokens[7].GetTokenType(), FACTORIAL_TOKEN);
}

TEST_F(ScannerTest, RelationalOperators) {
    auto tokens = GetAllTokens("= < > <= >=");
    
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].GetTokenType(), EQUAL_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), LESS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), GREATER_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), LESSEQUAL_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), GREATEREQUAL_TOKEN);
}

TEST_F(ScannerTest, Separators) {
    auto tokens = GetAllTokens("() {} , ;");
    
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].GetTokenType(), LPAREN_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), RPAREN_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), LCURLY_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), RCURLY_TOKEN);
    EXPECT_EQ(tokens[4].GetTokenType(), COMMA_TOKEN);
    EXPECT_EQ(tokens[5].GetTokenType(), SEMICOLON_TOKEN);
}

TEST_F(ScannerTest, Variables) {
    auto tokens = GetAllTokens("x y xy xsin()");
    
    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].GetTokenType(), VARIABLE_TOKEN);
    EXPECT_EQ(tokens[0].GetLexeme(), "x");
    EXPECT_EQ(tokens[1].GetTokenType(), VARIABLE_TOKEN);
    EXPECT_EQ(tokens[1].GetLexeme(), "y");
    EXPECT_EQ(tokens[2].GetTokenType(), VARIABLE_TOKEN);
    EXPECT_EQ(tokens[2].GetLexeme(), "x");
    EXPECT_EQ(tokens[3].GetTokenType(), VARIABLE_TOKEN);
    EXPECT_EQ(tokens[3].GetLexeme(), "y");
    EXPECT_EQ(tokens[4].GetTokenType(), VARIABLE_TOKEN);
    EXPECT_EQ(tokens[4].GetLexeme(), "x");
    EXPECT_EQ(tokens[5].GetTokenType(), SIN_TOKEN);
    EXPECT_EQ(tokens[6].GetTokenType(), LPAREN_TOKEN);
    EXPECT_EQ(tokens[7].GetTokenType(), RPAREN_TOKEN);
}

TEST_F(ScannerTest, LineNumberTracking) {
    ScannerClass scanner("1\n2\n3", false);
    
    EXPECT_EQ(scanner.GetLineNumber(), 1);
    scanner.GetNextToken(); // Read "1"
    
    EXPECT_EQ(scanner.GetLineNumber(), 2);
    scanner.GetNextToken(); // Read "2"
    
    EXPECT_EQ(scanner.GetLineNumber(), 3);
}


TEST_F(ScannerTest, MultipleStatementsWithSemicolon) {
    auto tokens = GetAllTokens("x = 5; y = 10;");
    
    EXPECT_GT(tokens.size(), 0);
    // Verify semicolons are present
    int semicolonCount = 0;
    for (const auto& token : tokens) {
        if (token.GetTokenType() == SEMICOLON_TOKEN) {
            semicolonCount++;
        }
    }
    EXPECT_EQ(semicolonCount, 2);
}
