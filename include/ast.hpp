#ifndef COMPILE_C_TRY_AST_HPP
#define COMPILE_C_TRY_AST_HPP

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

struct Expression {
    virtual ~Expression() = default;
};

struct NumberExpression final : Expression {
    explicit NumberExpression(std::string value) : value(std::move(value)) {}
    std::string value;
};

struct StringExpression final : Expression {
    explicit StringExpression(std::string value) : value(std::move(value)) {}
    std::string value;
};

struct VariableExpression final : Expression {
    explicit VariableExpression(std::string name) : name(std::move(name)) {}
    std::string name;
};

struct UnaryExpression final : Expression {
    UnaryExpression(char operation, std::unique_ptr<Expression> operand)
        : operation(operation), operand(std::move(operand)) {}
    char operation;
    std::unique_ptr<Expression> operand;
};

struct BinaryExpression final : Expression {
    BinaryExpression(std::unique_ptr<Expression> left, char operation,
                     std::unique_ptr<Expression> right)
        : left(std::move(left)), operation(operation), right(std::move(right)) {}
    std::unique_ptr<Expression> left;
    char operation;
    std::unique_ptr<Expression> right;
};

struct Statement {
    virtual ~Statement() = default;
};

struct AssignmentStatement final : Statement {
    AssignmentStatement(std::string name, std::unique_ptr<Expression> value,
                        bool declaration)
        : name(std::move(name)), value(std::move(value)), declaration(declaration) {}
    std::string name;
    std::unique_ptr<Expression> value;
    bool declaration;
};

struct PrintStatement final : Statement {
    explicit PrintStatement(std::unique_ptr<Expression> value)
        : value(std::move(value)) {}
    std::unique_ptr<Expression> value;
};

struct Program {
    std::vector<std::unique_ptr<Statement>> statements;
};

void printAst(std::ostream& output, const Program& program);

#endif
