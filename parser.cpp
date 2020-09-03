#include "parser.hpp"

auto Parser::parse() -> std::vector<std::unique_ptr<Statement>> {
    auto statements = std::vector<std::unique_ptr<Statement>>();

    while (true) {
        auto lookaheadType = tokenStream.lookahead().type;

        if (lookaheadType == TokenKind::EndOfFile)
            break;

        if (lookaheadType == TokenKind::LeftBracket)
            statements.push_back(loop());
        else statements.push_back(single());
    }

    return statements;
}

auto Parser::single() -> std::unique_ptr<Statement> {
    auto token = tokenStream.next();
    auto type = token.type;

    switch (type) {
        case TokenKind::Dot:
            return std::make_unique<PrintStatement>();
        case TokenKind::Comma:
            return std::make_unique<InputStatement>();
        case TokenKind::LeftAngledBracket:
            return std::make_unique<ShiftLeftStatement>();
        case TokenKind::RightAngledBracket:
            return std::make_unique<ShiftRightStatement>();
        case TokenKind::Plus:
            return std::make_unique<IncrementStatement>();
        case TokenKind::Minus:
            return std::make_unique<DecrementStatement>();
        default: throw std::logic_error("Bad input."); // TODO: Improve message
    }
}

auto Parser::loop() -> std::unique_ptr<LoopStatement> {
    auto statements = std::vector<std::unique_ptr<Statement>>();
    tokenStream.next(); // Consume left bracket token.

    TokenKind lookahead;
    while ((lookahead = tokenStream.lookahead().type) != TokenKind::RightBracket) {
        if (lookahead == TokenKind::EndOfFile)
            throw std::logic_error("Unexpected end of input");

        if (lookahead == TokenKind::LeftBracket)
            statements.push_back(loop());
        else statements.push_back(single());
    }

    tokenStream.next(); // Consume right bracket token.
    return std::make_unique<LoopStatement>(std::move(statements));
}

Parser::Parser(TokenStream& tokenStream) : tokenStream(tokenStream) { }
