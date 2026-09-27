#include <gtest/gtest.h>
#include "calculator.h"

TEST(CalculatorTest, SimpleAdd) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("3 + 5"), 8.0);
}

TEST(CalculatorTest, ComplexExpr) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("3 + 5 * (2 - 8)"), -27.0);
}

TEST(CalculatorTest, Negative) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("-5 + 3"), -2.0);
}

TEST(CalculatorTest, DivideByZero) {
    Calculator c;
    EXPECT_THROW(c.evaluate("1 / 0"), std::runtime_error);
}

TEST(CalculatorTest, Parentheses) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("(1 + 2) * (3 + 4)"), 21.0);
}

TEST(CalculatorTest, IllegalChar) {
    Calculator c;
    EXPECT_THROW(c.evaluate("3 + a"), std::runtime_error);
}

TEST(CalculatorTest, MissingParen) {
    Calculator c;
    EXPECT_THROW(c.evaluate("(3 + 5"), std::runtime_error);
}

TEST(CalculatorTest, EmptyExpr) {
    Calculator c;
    EXPECT_THROW(c.evaluate(""), std::runtime_error);
}

TEST(CalculatorTest, DecimalNumbers) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("1.5 + 2.5"), 4.0);
}

TEST(CalculatorTest, NestedParen) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("((1+2)*(3+4))"), 21.0);
}

TEST(CalculatorTest, ComplexNegative) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("-3 * -(2 + 1)"), 9.0);
}

TEST(CalculatorTest, IllegalChar) {
    Calculator c;
    EXPECT_THROW(c.evaluate("3 + a"), std::runtime_error);
}

TEST(CalculatorTest, MissingParen) {
    Calculator c;
    EXPECT_THROW(c.evaluate("(3 + 5"), std::runtime_error);
}

TEST(CalculatorTest, EmptyExpr) {
    Calculator c;
    EXPECT_THROW(c.evaluate(""), std::runtime_error);
}

TEST(CalculatorTest, DecimalNumbers) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("1.5 + 2.5"), 4.0);
}

TEST(CalculatorTest, NestedParen) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("((1+2)*(3+4))"), 21.0);
}

TEST(CalculatorTest, ComplexNegative) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.evaluate("-3 * -(2 + 1)"), 9.0);
}
