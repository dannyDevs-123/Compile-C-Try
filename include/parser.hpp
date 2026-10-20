#ifndef COMPILE_C_TRY_PARSER_HPP
#define COMPILE_C_TRY_PARSER_HPP

#include "ast.hpp"
#include "token.hpp"

#include <cstddef>
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    Program parse();

private:
    const Token& peek() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    const Token& consume(TokenType type, const char* message);
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> assignment(bool declaration);
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> primary();
    [[noreturn]] void fail(const Token& token, const char* message) const;

    const std::vector<Token>& tokens_;
    std::size_t current_ = 0;
};

#endif
