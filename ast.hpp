#pragma once

#include <utility>
#include <vector>
#include <memory>

enum StatementKind {
    Print,
    Input,
    ShiftLeft,
    ShiftRight,
    Loop,
    Increment,
    Decrement
};

class Visitor;

class Statement {
public:
    virtual auto kind() const -> StatementKind = 0;
    virtual auto accept(Visitor& visitor) -> void = 0;
};

class PrintStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::Print; }
    auto accept(Visitor& visitor) -> void override;
};

class InputStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::Input; }
    auto accept(Visitor& visitor) -> void override;
};

class ShiftLeftStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::ShiftLeft; }
    auto accept(Visitor& visitor) -> void override;
};

class ShiftRightStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::ShiftRight; }
    auto accept(Visitor& visitor) -> void override;
};

class LoopStatement : public Statement {
public:
    const std::vector<std::unique_ptr<Statement>> statements;

    auto kind() const -> StatementKind override { return StatementKind::Loop; }
    auto accept(Visitor& visitor) -> void override;

    explicit LoopStatement(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) { }
};

class IncrementStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::Increment; }
    auto accept(Visitor& visitor) -> void override;
};

class DecrementStatement : public Statement {
public:
    auto kind() const -> StatementKind override { return StatementKind::Decrement; }
    auto accept(Visitor& visitor) -> void override;
};

class Visitor {
public:
    virtual auto visit(const PrintStatement& printStatement) -> void { };
    virtual auto visit(const InputStatement& inputStatement) -> void { };
    virtual auto visit(const ShiftLeftStatement& shiftLeftStatement) -> void { };
    virtual auto visit(const ShiftRightStatement& shiftRightStatement) -> void { };
    virtual auto visit(const LoopStatement& loopStatement) -> void { };
    virtual auto visit(const IncrementStatement& incrementStatement) -> void { };
    virtual auto visit(const DecrementStatement& decrementStatement) -> void { };
};
