#include "calculator.h"
#include <cctype>
#include <cmath>

void Calculator::skip_whitespace() {
    while (pos_ < expr_.size() && std::isspace(expr_[pos_])) {
        ++pos_;
    }
}

Calculator::Token Calculator::next_token() {
    skip_whitespace();
    if (pos_ >= expr_.size()) {
        return {TokenType::END, 0};
    }

    char c = expr_[pos_];
    switch (c) {
        case '+': ++pos_; return {TokenType::PLUS, 0};
        case '-': ++pos_; return {TokenType::MINUS, 0};
        case '*': ++pos_; return {TokenType::MUL, 0};
        case '/': ++pos_; return {TokenType::DIV, 0};
        case '(': ++pos_; return {TokenType::LPAREN, 0};
        case ')': ++pos_; return {TokenType::RPAREN, 0};
    }

    if (std::isdigit(c) || c == '.') {
        size_t start = pos_;
        while (pos_ < expr_.size() && (std::isdigit(expr_[pos_]) || expr_[pos_] == '.')) {
            ++pos_;
        }
        return {TokenType::NUMBER, std::stod(expr_.substr(start, pos_ - start))};
    }

    throw std::runtime_error(std::string("非法字符: ") + c);
}

double Calculator::parse_factor() {
    Token tok = next_token();
    if (tok.type == TokenType::NUMBER) {
        return tok.value;
    }
    if (tok.type == TokenType::MINUS) {
        return -parse_factor();
    }
    if (tok.type == TokenType::LPAREN) {
        double val = parse_expression();
        Token close = next_token();
        if (close.type != TokenType::RPAREN) {
            throw std::runtime_error("缺少右括号");
        }
        return val;
    }
    throw std::runtime_error("语法错误：期望数字或括号");
}

double Calculator::parse_term() {
    double left = parse_factor();
    while (true) {
        size_t save = pos_;
        Token tok = next_token();
        if (tok.type == TokenType::MUL) {
            left *= parse_factor();
        } else if (tok.type == TokenType::DIV) {
            double right = parse_factor();
            if (right == 0) throw std::runtime_error("除零错误");
            left /= right;
        } else {
            pos_ = save;
            break;
        }
    }
    return left;
}

double Calculator::parse_expression() {
    double left = parse_term();
    while (true) {
        size_t save = pos_;
        Token tok = next_token();
        if (tok.type == TokenType::PLUS) {
            left += parse_term();
        } else if (tok.type == TokenType::MINUS) {
            left -= parse_term();
        } else {
            pos_ = save;
            break;
        }
    }
    return left;
}

double Calculator::evaluate(const std::string& expr) {
    expr_ = expr;
    pos_ = 0;
    double result = parse_expression();
    if (pos_ < expr_.size()) {
        throw std::runtime_error("表达式末尾有非法字符");
    }
    return result;
}
