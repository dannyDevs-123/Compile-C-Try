#ifndef COMPILE_C_TRY_KEYWORDS_HPP
#define COMPILE_C_TRY_KEYWORDS_HPP

#include "token.hpp"

#include <string>

bool isIdentifierStart(char character);
bool isIdentifierPart(char character);
bool isIdentifier(const std::string& text);
TokenType keywordType(const std::string& text);

#endif
