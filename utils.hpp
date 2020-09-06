#pragma once

#include "ast.hpp"
#include <iostream>
#include <locale>
#include <codecvt>

auto prettyPrintStatement(const std::unique_ptr<Statement>& statement, std::string indent) -> void {
    static std::unordered_map<StatementKind, std::string> kindToText = {
        { StatementKind::Print, "Print" },
        { StatementKind::Input, "Input" },
        { StatementKind::ShiftLeft, "ShiftLeft" },
        { StatementKind::ShiftRight, "ShiftRight" },
        { StatementKind::Loop, "Loop" },
        { StatementKind::Increment, "Increment" },
        { StatementKind::Decrement, "Decrement" }
    };

    std::cout << indent;
    std::cout << kindToText[statement->kind()] << std::endl;

    indent += "  ";

    if (statement->kind() != StatementKind::Loop)
        return;
    
    auto& loop = dynamic_cast<LoopStatement&>(*statement);
    auto& children = loop.statements;
    if (children.empty())
        return;

    for (auto& child : children)
        prettyPrintStatement(child, indent);
}
