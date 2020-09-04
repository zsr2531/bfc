#include "cli.hpp"

auto CommandLineParser::parse() -> CommandLineParseResult {
    std::unordered_set<Flag> flags {};
    std::unordered_map<Option, std::string> options {};

    for (auto i = 1; i < count; i++) {
        auto current = args[i];
        auto search = switches.find(current);
        if (search == switches.end())
            throw std::runtime_error("Unknown flag or option");

        auto sw = search->second;
        if (!sw->hasValue()) {
            flags.insert(*dynamic_cast<Flag*>(sw));
            continue;
        }

        if ((i + 1) >= count)
            throw std::runtime_error("No value provided for option");

        options[*dynamic_cast<Option*>(sw)] = args[++i];
    }

    return CommandLineParseResult(flags, options);
}
