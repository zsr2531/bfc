#pragma once

#include <string>
#include <functional>
#include <optional>
#include <fstream>

using ulong = unsigned long long;

struct TextSpan {
    ulong begin;
    ulong end;

    TextSpan(ulong begin, ulong end);
};

enum TokenKind {
    Dot,
    Comma,
    LeftAngledBracket,
    RightAngledBracket,
    LeftBracket,
    RightBracket,
    Plus,
    Minus,
    Comment,
    EndOfFile
};

class Token {
public:
    const TextSpan span;
    const TokenKind type;
    const char value;

    Token(const TextSpan& span, TokenKind type, char value);
};

class TokenStream {
public:
    auto lookahead() -> Token;

    auto next() -> Token;

    explicit TokenStream(std::function<Token()> supplier);
private:
    const std::function<Token()> supplier;
    std::optional<Token> stash;
};

class Lexer {
public:
    auto lex() -> TokenStream;
protected:
    ulong position{};

    virtual auto supply() -> char = 0;
};

class TextLexer : public Lexer {
public:
    explicit TextLexer(std::string text);
private:
    const std::string text;

    auto supply() -> char override;
};

class FileLexer : public Lexer {
public:
    explicit FileLexer(std::ifstream& stream);
private:
    std::ifstream& stream;
    bool done;

    auto supply() -> char override;
};
