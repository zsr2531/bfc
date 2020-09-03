#include "lexer.hpp"
#include <utility>

TextSpan::TextSpan(const ulong begin, const ulong end) : begin(begin), end(end) { }

Token::Token(const TextSpan& span, const TokenKind type, const char value)
    : span(span), type(type), value(value) { }

auto TokenStream::lookahead() -> Token {
    if (stash)
        return stash.value();

    auto token = supplier();
    stash.emplace(token);
    return token;
}

auto TokenStream::next() -> Token {
    if (!stash)
        return supplier();

    auto temp = stash.value();
    stash = std::nullopt;
    return temp;
}

TokenStream::TokenStream(std::function<Token()> supplier) : supplier(std::move(supplier)) {
    stash = std::nullopt;
}

auto Lexer::lex() -> TokenStream {
    static std::unordered_map<char, TokenKind> tokenMap = {
        { '.', TokenKind::Dot },
        { ',', TokenKind::Comma },
        { '<', TokenKind::LeftAngledBracket },
        { '>', TokenKind::RightAngledBracket },
        { '[', TokenKind::LeftBracket },
        { ']', TokenKind::RightBracket },
        { '+', TokenKind::Plus },
        { '-', TokenKind::Minus }
    };

    return TokenStream([&]() {
        char character;
        TextSpan span = TextSpan(0, 0);
        TokenKind type;

        do {
            character = supply();
            if (character == '\0')
                return Token(TextSpan(position, position), TokenKind::EndOfFile, '\0');

            span = TextSpan(position - 1, position);
            auto search = tokenMap.find(character);
            type = search == tokenMap.end() ? TokenKind::Comment : search->second;
        } while (type == TokenKind::Comment);

        return Token(span, type, character);
    });
}

TextLexer::TextLexer(std::string text) : text(std::move(text)) {
    position = 0;
}

auto TextLexer::supply() -> char {
    if (position >= text.length())
        return '\0';

    return text[position++];
}

FileLexer::FileLexer(std::ifstream& stream) : stream(stream) {
    position = 0;
    done = false;
}

auto FileLexer::supply() -> char {
    if (done)
        return '\0';

    position++;
    auto data = stream.get();
    if (data == -1) {
        done = true;
        return '\0';
    }

    return data;
}
