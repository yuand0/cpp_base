#pragma once
#include <string>
#include <stdexcept>

class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;

    // 计算表达式，支持加减乘除、括号、负数
    double evaluate(const std::string& expr);

private:
    // 词法分析
    enum class TokenType { NUMBER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END };
    struct Token {
        TokenType type;
        double value;
    };

    std::string expr_;
    size_t pos_;

    Token next_token();
    double parse_expression();
    double parse_term();
    double parse_factor();
    void skip_whitespace();
};
