#ifndef COMPILE_C_TRY_TOKEN_HPP
#define COMPILE_C_TRY_TOKEN_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,
    String,
    Let,
    Print,
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    LeftParen,
    RightParen,
    Semicolon,
    EndOfFile
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::size_t line;
    std::size_t column;
};

const char* tokenTypeName(TokenType type);
void writeQuotedString(std::ostream& output, const std::string& value);
std::ostream& operator<<(std::ostream& output, const Token& token);
void printTokens(std::ostream& output, const std::vector<Token>& tokens);

#endif
