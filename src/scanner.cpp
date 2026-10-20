#include "scanner.hpp"

#include "keywords.hpp"

#include <cctype>
#include <stdexcept>
#include <utility>

Scanner::Scanner(std::string source) : source_(stripComments(source)) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start_ = current_;
        startLine_ = line_;
        startColumn_ = column_;
        scanToken();
    }
    tokens_.push_back({TokenType::EndOfFile, "", line_, column_});
    return tokens_;
}

char Scanner::advance() {
    const char character = source_[current_++];
    if (character == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    return character;
}

bool Scanner::match(char expected) {
    if (isAtEnd() || source_[current_] != expected) {
        return false;
    }
    advance();
    return true;
}

bool Scanner::isAtEnd() const {
    return current_ >= source_.size();
}

char Scanner::peek() const {
    return isAtEnd() ? '\0' : source_[current_];
}

char Scanner::peekNext() const {
    return current_ + 1 >= source_.size() ? '\0' : source_[current_ + 1];
}

void Scanner::scanToken() {
    const char character = advance();
    switch (character) {
    case ' ':
    case '\r':
    case '\t':
    case '\n':
        return;
    case '+': addToken(TokenType::Plus); return;
    case '-': addToken(TokenType::Minus); return;
    case '*': addToken(TokenType::Star); return;
    case '/': addToken(TokenType::Slash); return;
    case '=': addToken(TokenType::Equal); return;
    case '(': addToken(TokenType::LeftParen); return;
    case ')': addToken(TokenType::RightParen); return;
    case ';': addToken(TokenType::Semicolon); return;
    case '"': scanString(); return;
    default:
        if (std::isdigit(static_cast<unsigned char>(character)) != 0) {
            scanNumber();
        } else if (isIdentifierStart(character)) {
            scanIdentifier();
        } else {
            fail("Unexpected character");
        }
    }
}

void Scanner::scanIdentifier() {
    while (isIdentifierPart(peek())) {
        advance();
    }
    const std::string text = source_.substr(start_, current_ - start_);
    if (!isIdentifier(text)) {
        fail("Invalid identifier");
    }
    addToken(keywordType(text));
}

void Scanner::addToken(TokenType type) {
    tokens_.push_back({type, source_.substr(start_, current_ - start_),
                       startLine_, startColumn_});
}

[[noreturn]] void Scanner::fail(const std::string& message) const {
    throw std::runtime_error(message + " at " + std::to_string(startLine_) + ':' +
                             std::to_string(startColumn_));
}
