#ifndef COMPILE_C_TRY_SCANNER_HPP
#define COMPILE_C_TRY_SCANNER_HPP

#include "token.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Scanner {
public:
    explicit Scanner(std::string source);
    std::vector<Token> scanTokens();

private:
    char advance();
    bool match(char expected);
    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    void scanToken();
    void scanIdentifier();
    void scanNumber();
    void scanString();
    void addToken(TokenType type);
    [[noreturn]] void fail(const std::string& message) const;

    std::string source_;
    std::vector<Token> tokens_;
    std::size_t start_ = 0;
    std::size_t current_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;
    std::size_t startLine_ = 1;
    std::size_t startColumn_ = 1;
};

std::string stripComments(const std::string& source);

#endif
