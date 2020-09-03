#include "parser.hpp"
#include "utils.hpp"
#include "interpreter.hpp"

auto main(int argc, char* argv[]) -> int {
    auto source = std::ifstream("source.txt", std::ifstream::in);
    auto lexer = FileLexer(source);
    auto tokenStream = lexer.lex();
    auto parser = Parser(tokenStream);
    auto root = parser.parse();
    source.close();
    auto& last = root.back();
    std::cout << "CompilationUnit" << std::endl;
    for (auto& statement : root)
        prettyPrint(statement, "", statement == last);

    auto interpreter = Interpreter(root);
    interpreter.interpret();
}
