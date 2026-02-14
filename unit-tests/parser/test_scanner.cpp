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

// Basic tokenization tests
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

TEST_F(ScannerTest, ArithmeticOperators) {
    auto tokens = GetAllTokens("+ - * /");
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].GetTokenType(), PLUS_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), MINUS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), TIMES_TOKEN);
    EXPECT_EQ(tokens[3].GetTokenType(), DIVIDE_TOKEN);
}

TEST_F(ScannerTest, LineNumberTracking) {
    ScannerClass scanner("1\n2\n3", false);
    
    EXPECT_EQ(scanner.GetLineNumber(), 1);
    scanner.GetNextToken(); // Read "1"
    
    EXPECT_EQ(scanner.GetLineNumber(), 2);
    scanner.GetNextToken(); // Read "2"
    
    EXPECT_EQ(scanner.GetLineNumber(), 3);
}

TEST_F(ScannerTest, ReservedWordPi) {
    ScannerClass scanner("pi", false);
    TokenClass token = scanner.GetNextToken();
    
    EXPECT_EQ(token.GetTokenType(), PI_TOKEN);
}

TEST_F(ScannerTest, TrigonometricFunctions) {
    auto tokens = GetAllTokens("sin cos tan");
    
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].GetTokenType(), SIN_TOKEN);
    EXPECT_EQ(tokens[1].GetTokenType(), COS_TOKEN);
    EXPECT_EQ(tokens[2].GetTokenType(), TAN_TOKEN);
}

TEST_F(ScannerTest, MultipleStatementsWithSemicolon) {
    auto tokens = GetAllTokens("x = 5; y = 10;");
    
    // Should contain: identifier, equal, number, semicolon, identifier, equal, number, semicolon
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
