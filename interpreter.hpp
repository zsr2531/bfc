#pragma once

#include <unordered_map>
#include "ast.hpp"

using byte = unsigned char;

class Interpreter : public Visitor {
public:
    auto interpret() -> void;

    explicit Interpreter(const std::vector<std::unique_ptr<Statement>>& statements);

    auto visit(const PrintStatement& printStatement) -> void override;
    auto visit(const InputStatement& inputStatement) -> void override;
    auto visit(const ShiftLeftStatement& shiftLeftStatement) -> void override;
    auto visit(const ShiftRightStatement& shiftRightStatement) -> void override;
    auto visit(const LoopStatement& loopStatement) -> void override;
    auto visit(const IncrementStatement& incrementStatement) -> void override;
    auto visit(const DecrementStatement& decrementStatement) -> void override;
private:
    const std::vector<std::unique_ptr<Statement>>& statements;

    int64_t cellPointer;
    std::unordered_map<int64_t, byte> cells;
};
