#pragma once

#include "ast.hpp"
#include <iostream>
#include <locale>
#include <codecvt>

// Thank you Windows for being a pile of shit and having to need this workaround (:
inline static auto charFromValue(int value, int count = 1) -> std::string {
    return std::string(count, value);
}

auto prettyPrintStatement(const std::unique_ptr<Statement>& statement, std::string indent, bool isLast = true) -> void {
    static std::unordered_map<StatementKind, std::string> kindToText = {
        { StatementKind::Print, "Print" },
        { StatementKind::Input, "Input" },
        { StatementKind::ShiftLeft, "ShiftLeft" },
        { StatementKind::ShiftRight, "ShiftRight" },
        { StatementKind::Loop, "Loop" },
        { StatementKind::Increment, "Increment" },
        { StatementKind::Decrement, "Decrement" }
    };

    static auto twoLines = charFromValue(196, 2);
    static auto corner = charFromValue(192);
    static auto middleLine = charFromValue(195);
    static auto verticalLine = charFromValue(179);
    static auto lastEdge = corner + twoLines;
    static auto notLastEdge = middleLine + twoLines;

    auto marker = isLast ? lastEdge : notLastEdge;
    std::cout << indent;
    std::cout << marker;
    std::cout << kindToText[statement->kind()] << std::endl;

    indent += isLast ? "    " : verticalLine + "   ";

    if (statement->kind() != StatementKind::Loop)
        return;
    
    auto& loop = dynamic_cast<LoopStatement&>(*statement);
    auto& children = loop.statements;
    if (children.empty())
        return;

    auto& last = children.back();
    for (auto& child : children)
        prettyPrintStatement(child, indent, child == last);
}
