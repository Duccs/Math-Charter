#include <gtest/gtest.h>
#include "Expression.h"
#include "Parser.h"

// Test fixture for Parser tests
class ParserTest : public ::testing::Test {
protected:   
    // Helper function to parse and evaluate an expression with given x value
    float ParseAndEvaluate(const std::string& equation, float x) {
        Expression expr = Expression::parse(equation);
        EXPECT_TRUE(expr.isValid()) << "Parsing failed: " << expr.getError();
        return expr.evaluate(x);
    }
};

// Parsing tests
TEST_F(ParserTest, infixToPostfixSimple) {
    auto tokens = toPostfix("3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3");
    std::vector<TokenType> expected = {
        NUMBER_TOKEN, NUMBER_TOKEN, NUMBER_TOKEN, 
        TIMES_TOKEN, NUMBER_TOKEN, NUMBER_TOKEN,
        MINUS_TOKEN, NUMBER_TOKEN, NUMBER_TOKEN,
        EXP_TOKEN, EXP_TOKEN, DIVIDE_TOKEN, PLUS_TOKEN
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].GetTokenType(), expected[i]);
    }
}

TEST_F(ParserTest, infixToPostfixWithFunctions) {
    auto tokens = toPostfix("sin(x) + log(10) * sqrt(4)");
    std::vector<TokenType> expected = {
        VARIABLE_TOKEN, SIN_TOKEN, 
        NUMBER_TOKEN, LOG_TOKEN, 
        NUMBER_TOKEN, SQRT_TOKEN, 
        TIMES_TOKEN, PLUS_TOKEN
    };

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].GetTokenType(), expected[i]);
    }
}