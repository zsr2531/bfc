#include "codegen.hpp"

auto CodeGen::visitPrintStatement(const PrintStatement& printStatement) -> void {
    indentation();
    builder << "putchar(memory[current]);\n";
}

auto CodeGen::visitInputStatement(const InputStatement& inputStatement) -> void {
    indentation();
    builder << "memory[current] = getchar();\n";
}

auto CodeGen::visitShiftLeftStatement(const ShiftLeftStatement& shiftLeftStatement) -> void {
    indentation();
    if (shiftLeftStatement.by == 1)
        builder << "current--;\n";
    else builder << "current -= " << shiftLeftStatement.by << ";\n";
}

auto CodeGen::visitShiftRightStatement(const ShiftRightStatement& shiftRightStatement) -> void {
    indentation();
    if (shiftRightStatement.by == 1)
        builder << "current++;\n";
    else builder << "current += " << shiftRightStatement.by << ";\n";
}

auto CodeGen::visitIncrementStatement(const IncrementStatement& incrementStatement) -> void {
    indentation();
    if (incrementStatement.by == 1)
        builder << "memory[current]++;\n";
    else builder << "memory[current] += " << incrementStatement.by << ";\n";
}

auto CodeGen::visitDecrementStatement(const DecrementStatement& decrementStatement) -> void {
    indentation();
    if (decrementStatement.by == 1)
        builder << "memory[current]--;\n";
    else builder << "memory[current] -= " << decrementStatement.by << ";\n";
}

auto CodeGen::enterLoopStatement(const LoopStatement& loopStatement) -> void {
    indentation();
    builder << "while (memory[current] != 0) {\n";
    indentLevel++;
}

auto CodeGen::exitLoopStatement(const LoopStatement& loopStatement) -> void {
    indentLevel--;
    indentation();
    builder << "}\n";
}
