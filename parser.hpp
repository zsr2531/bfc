#pragma once

#include "lexer.hpp"
#include "ast.hpp"

class Parser {
public:
    auto parse() -> std::vector<std::unique_ptr<Statement>>;
    auto single() -> std::unique_ptr<Statement>;
    auto loop() -> std::unique_ptr<LoopStatement>;

    explicit Parser(TokenStream& tokenStream);
private:
    TokenStream& tokenStream;
};
