#include <gtest/gtest.h>
#include "Expression.h"
#include "SymbolTable.h"
#include "Parser.h"

// Test fixture for Parser tests
class ParserTest : public ::testing::Test {
protected:   
    // Helper function to parse and evaluate an expression with given x value
    float ParseAndEvaluate(const std::string& equation, float x) {
        Expression expr = Expression::parse(equation);
        EXPECT_TRUE(expr.isValid()) << "Parsing failed: " << expr.getError();
        SymbolTable symbols;
        symbols.AddEntry("x");
        symbols.SetValue("x", x);
        return expr.evaluate(symbols);
    }
};