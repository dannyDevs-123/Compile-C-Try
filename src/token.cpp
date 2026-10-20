#include "token.hpp"

#include <ostream>

const char* tokenTypeName(TokenType type) {
    switch (type) {
    case TokenType::Identifier: return "IDENTIFIER";
    case TokenType::Number: return "NUMBER";
    case TokenType::String: return "STRING";
    case TokenType::Let: return "LET";
    case TokenType::Print: return "PRINT";
    case TokenType::Plus: return "PLUS";
    case TokenType::Minus: return "MINUS";
    case TokenType::Star: return "STAR";
    case TokenType::Slash: return "SLASH";
    case TokenType::Equal: return "EQUAL";
    case TokenType::LeftParen: return "LEFT_PAREN";
    case TokenType::RightParen: return "RIGHT_PAREN";
    case TokenType::Semicolon: return "SEMICOLON";
    case TokenType::EndOfFile: return "EOF";
    }
    return "UNKNOWN";
}

void writeQuotedString(std::ostream& output, const std::string& value) {
    output << '"';
    for (const char character : value) {
        switch (character) {
        case '\\': output << "\\\\"; break;
        case '"': output << "\\\""; break;
        case '\n': output << "\\n"; break;
        case '\r': output << "\\r"; break;
        case '\t': output << "\\t"; break;
        default: output << character; break;
        }
    }
    output << '"';
}

std::ostream& operator<<(std::ostream& output, const Token& token) {
    output << tokenTypeName(token.type) << ' ';
    writeQuotedString(output, token.lexeme);
    return output << " at " << token.line << ':' << token.column;
}

void printTokens(std::ostream& output, const std::vector<Token>& tokens) {
    for (const Token& token : tokens) {
        output << token << '\n';
    }
}
