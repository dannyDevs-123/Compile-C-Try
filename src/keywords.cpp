#include "keywords.hpp"

#include <cctype>
#include <unordered_map>

bool isIdentifierStart(char character) {
    const unsigned char value = static_cast<unsigned char>(character);
    return std::isalpha(value) != 0 || character == '_';
}

bool isIdentifierPart(char character) {
    const unsigned char value = static_cast<unsigned char>(character);
    return std::isalnum(value) != 0 || character == '_';
}

bool isIdentifier(const std::string& text) {
    if (text.empty() || !isIdentifierStart(text.front())) {
        return false;
    }
    for (std::size_t index = 1; index < text.size(); ++index) {
        if (!isIdentifierPart(text[index])) {
            return false;
        }
    }
    return true;
}

TokenType keywordType(const std::string& text) {
    static const std::unordered_map<std::string, TokenType> keywords{
        {"let", TokenType::Let},
        {"print", TokenType::Print},
    };
    const auto found = keywords.find(text);
    return found == keywords.end() ? TokenType::Identifier : found->second;
}
