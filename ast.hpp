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
    long long by;

    explicit ShiftLeftStatement(long long by = 1) : by(by) { }

    auto kind() const -> StatementKind override { return StatementKind::ShiftLeft; }
    auto accept(Visitor& visitor) -> void override;
};

class ShiftRightStatement : public Statement {
public:
    long long by;

    explicit ShiftRightStatement(long long by = 1) : by(by) { }

    auto kind() const -> StatementKind override { return StatementKind::ShiftRight; }
    auto accept(Visitor& visitor) -> void override;
};

class LoopStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    auto kind() const -> StatementKind override { return StatementKind::Loop; }
    auto accept(Visitor& visitor) -> void override;

    explicit LoopStatement(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) { }
};

class IncrementStatement : public Statement {
public:
    long long by;

    explicit IncrementStatement(long long by = 1) : by(by) { }

    auto kind() const -> StatementKind override { return StatementKind::Increment; }
    auto accept(Visitor& visitor) -> void override;
};

class DecrementStatement : public Statement {
public:
    long long by;

    explicit DecrementStatement(long long by = 1) : by(by) { }

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

class Listener : public Visitor {
protected:
    virtual auto visitPrintStatement(const PrintStatement& printStatement) -> void { };
    virtual auto visitInputStatement(const InputStatement& inputStatement) -> void { };
    virtual auto visitShiftLeftStatement(const ShiftLeftStatement& shiftLeftStatement) -> void { };
    virtual auto visitShiftRightStatement(const ShiftRightStatement& shiftRightStatement) -> void { };
    virtual auto visitIncrementStatement(const IncrementStatement& incrementStatement) -> void { };
    virtual auto visitDecrementStatement(const DecrementStatement& decrementStatement) -> void { };
    virtual auto enterLoopStatement(const LoopStatement& loopStatement) -> void { };
    virtual auto exitLoopStatement(const LoopStatement& loopStatement) -> void { };
private:
    auto visit(const PrintStatement& printStatement) -> void override { visitPrintStatement(printStatement); };
    auto visit(const InputStatement& inputStatement) -> void override { visitInputStatement(inputStatement); };
    auto visit(const ShiftLeftStatement& shiftLeftStatement) -> void override { visitShiftLeftStatement(shiftLeftStatement); };
    auto visit(const ShiftRightStatement& shiftRightStatement) -> void override { visitShiftRightStatement(shiftRightStatement); };
    auto visit(const IncrementStatement& incrementStatement) -> void override { visitIncrementStatement(incrementStatement); };
    auto visit(const DecrementStatement& decrementStatement) -> void override { visitDecrementStatement(decrementStatement); };
    auto visit(const LoopStatement& loopStatement) -> void override {
        enterLoopStatement(loopStatement);

        for (auto& statement : loopStatement.statements) {
            statement->accept(*this);
        }

        exitLoopStatement(loopStatement);
    }
};
