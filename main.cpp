#include "cli.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include "interpreter.hpp"
#include "codegen.hpp"

auto mapOption(std::unordered_map<std::string, Switch*>& coll, Switch* sw) -> void {
    for (const std::string& identifier : sw->identifiers())
        coll[identifier] = sw;
}

template<class T, typename ...Args>
auto addOption(std::unordered_map<std::string, Switch*>& coll, Args&& ...args) -> std::unique_ptr<T> {
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

        return std::make_unique<FileLexer>(std::move(stream));
    }
}

auto main(int argc, char* argv[]) -> int {
    std::unordered_map<std::string, Switch*> switches {};
    auto help = addOption<Flag>(switches, "-h", "--help");
    auto prettyPrint = addOption<Flag>(switches, "--pretty-print-ast");
    auto eval = addOption<Option>(switches, "", "-e", "--eval");
    auto file = addOption<Option>(switches, "", "-f", "--file");
    auto pseudoCode = addOption<Option>(switches, "", "-o", "--output");

    auto cliParser = CommandLineParser(switches, argc, argv);
    auto result = cliParser.parse();

    if (result.hasFlag(*help)) {
        std::cout << "bfc by zsr2531\n";
        std::cout << "A simple brainfuck interpreter\nhttps://github.com/zsr2531/bfc licensed under MIT\n\n";
        std::cout << "Usage:\n";

        std::cout << "   " << "-h --help          " << "Shows this help message\n";
        std::cout << "   " << "--pretty-print-ast " << "Pretty prints the AST\n";
        std::cout << "   " << "-e --eval          " << "Evaluates the provided brainfuck program\n";
        std::cout << "   " << "-f --file          " << "Evaluates a brainfuck program from a file\n";
        std::cout << "   " << "-o --output        " << "Dumps C pseudocode to a file\n";
        return 0;
    }

    if (!(result.hasOption(*eval) ^ result.hasOption(*file))) {
        std::cerr << "You must provide either (-e|--eval) or (-f|--file)\n";
        return -1;
    }

    auto lexer = createLexer(result, eval.get(), file.get());
    auto tokenStream = lexer->lex();
    auto parser = Parser(tokenStream);
    auto statements = parser.parse();

    if (result.hasFlag(*prettyPrint)) {
        if (statements.empty())
            return 0;

        std::cout << "\033[31;40m";
        for (auto& statement : statements)
            prettyPrintStatement(statement, "");
        std::cout << "\033[0m";
    }

    if (result.hasOption(*pseudoCode)) {
        auto output = std::ofstream(result.getValue(*pseudoCode));
        auto generator = CodeGen();
        for (auto& stmt : statements)
            stmt->accept(generator);


        output << generator.toString();
        output.close();

        return 0;
    }

    auto interpreter = Interpreter(statements);
    interpreter.interpret();
}
