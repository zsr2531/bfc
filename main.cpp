#include "cli.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include "interpreter.hpp"

auto mapOption(std::unordered_map<const char*, Switch*>& coll, Switch* sw) -> void {
    for (auto identifier : sw->identifiers())
        coll[identifier] = sw;
}

template<class T, typename ...Args>
auto addOption(std::unordered_map<const char*, Switch*>& coll, Args&& ...args) -> std::unique_ptr<T> {
    static_assert(std::is_base_of_v<Switch, T>, "T must be derived from Switch");

    auto instance = std::make_unique<T>(std::forward<Args>(args)...);
    mapOption(coll, instance.get());

    return instance;
}

auto createLexer(CommandLineParseResult parseResult, Option* eval, Option* file) -> std::unique_ptr<Lexer> {
    if (parseResult.hasOption(*eval)) {
        return std::make_unique<TextLexer>(parseResult.getValue(*eval));
    } else {
        auto stream = std::ifstream(parseResult.getValue(*file), std::ios::in);
        if (!stream.good())
            throw std::runtime_error("Invalid path");

        return std::make_unique<FileLexer>(stream);
    }
}

auto main(int argc, char* argv[]) -> int {
    std::unordered_map<const char*, Switch*> switches {};
    auto help = addOption<Flag>(switches, "Shows this help message.", "-h", "--help");
    auto prettyPrint = addOption<Flag>(switches, "Pretty prints the AST.", "--pretty-print-ast");
    auto eval = addOption<Option>(switches, "Evaluates the provided brainfuck program.", "", "-e", "--eval");
    auto file = addOption<Option>(switches, "Evaluates a brainfuck program from a file.", "", "-f", "--file");

    auto cliParser = CommandLineParser(switches, argc, argv);
    auto result = cliParser.parse();

    if (result.hasFlag(*help)) // TODO: Print usage
        return 0;

    if (!(result.hasOption(*eval) ^ result.hasOption(*file)))
        return -1;

    auto lexer = createLexer(result, eval.get(), file.get());
    auto tokenStream = lexer->lex();
    auto parser = Parser(tokenStream);
    auto statements = parser.parse();

    if (result.hasFlag(*prettyPrint)) {
        std::cout << "CompilationUnit";
        if (statements.empty())
            return 0;

        auto& last = statements.back();
        for (auto& statement : statements)
            prettyPrintStatement(statement, "", statement == last);
    }

    auto interpreter = Interpreter(statements);
    interpreter.interpret();
}
