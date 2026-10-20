#include "scanner.hpp"

#include <cctype>
#include <cstdlib>
#include <string>
#include <utility>

void Scanner::scanNumber() {
    while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
        advance();
    }

    if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peekNext())) != 0) {
        advance();
        while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
            advance();
        }
    }

    if (peek() == 'e' || peek() == 'E') {
        advance();
        if (peek() == '+' || peek() == '-') {
            advance();
        }
        if (std::isdigit(static_cast<unsigned char>(peek())) == 0) {
            fail("Expected a digit in the number exponent");
        }
        while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
            advance();
        }
    }

    const std::string value = source_.substr(start_, current_ - start_);
    char* end = nullptr;
    std::strtod(value.c_str(), &end);
    if (end != value.c_str() + value.size()) {
        fail("Invalid numeric literal");
    }
    addToken(TokenType::Number);
}

void Scanner::scanString() {
    std::string value;
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n' || peek() == '\r') {
            fail("String literals cannot contain a newline");
        }
        if (peek() == '\\') {
            advance();
            if (isAtEnd()) {
                fail("Unterminated escape sequence in string literal");
            }
            switch (advance()) {
            case 'n': value.push_back('\n'); break;
            case 'r': value.push_back('\r'); break;
            case 't': value.push_back('\t'); break;
            case '\\': value.push_back('\\'); break;
            case '"': value.push_back('"'); break;
            default: fail("Unsupported escape sequence in string literal");
            }
        } else {
            value.push_back(advance());
        }
    }

    if (isAtEnd()) {
        fail("Unterminated string literal");
    }
    advance();
    tokens_.push_back({TokenType::String, std::move(value), startLine_, startColumn_});
}
