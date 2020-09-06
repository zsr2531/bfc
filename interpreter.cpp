#include <iostream>
#include "interpreter.hpp"

auto Interpreter::interpret() -> void {
    for (auto& statement : statements)
        statement->accept(*this);
}

Interpreter::Interpreter(const std::vector<std::unique_ptr<Statement>>& statements) : statements(statements) {
    cellPointer = 0;
    cells = std::unordered_map<int64_t, byte>();
}

auto Interpreter::visit(const PrintStatement& printStatement) -> void {
    std::cout << cells[cellPointer];
}

auto Interpreter::visit(const InputStatement& inputStatement) -> void {
    cells[cellPointer] = getchar();
}

auto Interpreter::visit(const ShiftLeftStatement& shiftLeftStatement) -> void {
    cellPointer -= shiftLeftStatement.by;
}

auto Interpreter::visit(const ShiftRightStatement& shiftRightStatement) -> void {
    cellPointer += shiftRightStatement.by;
}

auto Interpreter::visit(const LoopStatement& loopStatement) -> void {
    while (cells[cellPointer] != 0) {
        for (auto& statement : loopStatement.statements)
            statement->accept(*this);
    }
}

auto Interpreter::visit(const IncrementStatement& incrementStatement) -> void {
    cells[cellPointer] += incrementStatement.by;
}

auto Interpreter::visit(const DecrementStatement& decrementStatement) -> void {
    cells[cellPointer] -= decrementStatement.by;
}
