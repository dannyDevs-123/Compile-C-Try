#include "parser.hpp"

#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
void printExpression(std::ostream& output, const Expression& expression) {
    if (const auto* number = dynamic_cast<const NumberExpression*>(&expression)) {
        output << number->value;
    } else if (const auto* string = dynamic_cast<const StringExpression*>(&expression)) {
        writeQuotedString(output, string->value);
    } else if (const auto* variable = dynamic_cast<const VariableExpression*>(&expression)) {
        output << variable->name;
    } else if (const auto* unary = dynamic_cast<const UnaryExpression*>(&expression)) {
        output << '(' << unary->operation << ' ';
        printExpression(output, *unary->operand);
        output << ')';
    } else if (const auto* binary = dynamic_cast<const BinaryExpression*>(&expression)) {
        output << '(';
        printExpression(output, *binary->left);
        output << ' ' << binary->operation << ' ';
        printExpression(output, *binary->right);
        output << ')';
    }
}
}

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens) {
    if (tokens_.empty() || tokens_.back().type != TokenType::EndOfFile) {
        throw std::invalid_argument("Parser requires a token stream ending in EOF");
    }
}

Program Parser::parse() {
    Program program;
    while (!isAtEnd()) {
        program.statements.push_back(statement());
    }
    return program;
}

const Token& Parser::peek() const {
    return tokens_[current_];
}

const Token& Parser::previous() const {
    return tokens_[current_ - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EndOfFile;
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (!check(type)) {
        return false;
    }
    ++current_;
    return true;
}

const Token& Parser::consume(TokenType type, const char* message) {
    if (check(type)) {
        return tokens_[current_++];
    }
    fail(peek(), message);
}

std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::Let)) {
        return assignment(true);
    }
    if (match(TokenType::Print)) {
        auto value = expression();
        consume(TokenType::Semicolon, "Expected ';' after print expression");
        return std::make_unique<PrintStatement>(std::move(value));
    }
    if (check(TokenType::Identifier)) {
        return assignment(false);
    }
    fail(peek(), "Expected 'let', 'print', or an assignment");
}

std::unique_ptr<Statement> Parser::assignment(bool declaration) {
    const Token& name = consume(TokenType::Identifier, "Expected a variable name");
    consume(TokenType::Equal, "Expected '=' after variable name");
    auto value = expression();
    consume(TokenType::Semicolon, "Expected ';' after assignment");
    return std::make_unique<AssignmentStatement>(name.lexeme, std::move(value),
                                                  declaration);
}

std::unique_ptr<Expression> Parser::expression() {
    return term();
}

std::unique_ptr<Expression> Parser::term() {
    auto expression = factor();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        const char operation = previous().type == TokenType::Plus ? '+' : '-';
        auto right = factor();
        expression = std::make_unique<BinaryExpression>(std::move(expression), operation,
                                                        std::move(right));
    }
    return expression;
}

std::unique_ptr<Expression> Parser::factor() {
    auto expression = unary();
    while (match(TokenType::Star) || match(TokenType::Slash)) {
        const char operation = previous().type == TokenType::Star ? '*' : '/';
        auto right = unary();
        expression = std::make_unique<BinaryExpression>(std::move(expression), operation,
                                                        std::move(right));
    }
    return expression;
}

std::unique_ptr<Expression> Parser::unary() {
    if (match(TokenType::Minus) || match(TokenType::Plus)) {
        const char operation = previous().type == TokenType::Minus ? '-' : '+';
        return std::make_unique<UnaryExpression>(operation, unary());
    }
    return primary();
}

std::unique_ptr<Expression> Parser::primary() {
    if (match(TokenType::Number)) {
        return std::make_unique<NumberExpression>(previous().lexeme);
    }
    if (match(TokenType::String)) {
        return std::make_unique<StringExpression>(previous().lexeme);
    }
    if (match(TokenType::Identifier)) {
        return std::make_unique<VariableExpression>(previous().lexeme);
    }
    if (match(TokenType::LeftParen)) {
        auto expression = this->expression();
        consume(TokenType::RightParen, "Expected ')' after expression");
        return expression;
    }
    fail(peek(), "Expected an expression");
}

[[noreturn]] void Parser::fail(const Token& token, const char* message) const {
    throw std::runtime_error(std::string(message) + " at " +
                             std::to_string(token.line) + ':' +
                             std::to_string(token.column));
}

void printAst(std::ostream& output, const Program& program) {
    for (const auto& statement : program.statements) {
        if (const auto* assignment =
                dynamic_cast<const AssignmentStatement*>(statement.get())) {
            output << (assignment->declaration ? "let " : "") << assignment->name
                   << " = ";
            printExpression(output, *assignment->value);
            output << ";\n";
        } else if (const auto* print =
                       dynamic_cast<const PrintStatement*>(statement.get())) {
            output << "print ";
            printExpression(output, *print->value);
            output << ";\n";
        }
    }
}
