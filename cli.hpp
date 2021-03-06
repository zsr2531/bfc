#pragma once

#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>

class Switch {
public:
    friend bool operator ==(const Switch& lhs, const Switch& rhs) {
        auto lhsIds = lhs.identifiers();
        auto rhsIds = rhs.identifiers();

        if (lhsIds.size() != rhsIds.size())
            return false;

        for (auto i = 0; i < lhsIds.size(); i++) {
            if (lhsIds[i] != rhsIds[i])
                return false;
        }

        return true;
    }

    friend bool operator !=(const Switch& lhs, const Switch& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator <(const Switch& lhs, const Switch& rhs) {
        auto lhsIds = lhs.identifiers();
        auto rhsIds = rhs.identifiers();

        if (lhsIds.size() != rhsIds.size())
            return lhsIds.size() < rhsIds.size();

        for (auto i = 0; i < lhsIds.size(); i++) {
            if (lhsIds[i] < rhsIds[i])
                return true;
        }

        return false;
    }

    friend bool operator >(const Switch& lhs, const Switch& rhs) {
        return rhs < lhs;
    }

    friend bool operator <=(const Switch& lhs, const Switch& rhs) {
        return !(lhs > rhs);
    }

    friend bool operator >=(const Switch& lhs, const Switch& rhs) {
        return !(lhs < rhs);
    }

    virtual auto identifiers() const -> std::vector<std::string> = 0;
    virtual auto hasValue() const -> bool = 0;
};

class Flag : public Switch {
public:
    auto identifiers() const -> std::vector<std::string> override { return ids; }
    auto hasValue() const -> bool override { return false; }

    template<typename ...T>
    explicit Flag(const T ...identifier) : ids { identifier... } {
        static_assert(sizeof...(T) >= 1, "At least one identifier must be provided");
    }
private:
    const std::vector<std::string> ids;
};

class Option : public Switch {
public:
    const char* defaultValue;

    auto identifiers() const -> std::vector<std::string> override { return ids; }
    auto hasValue() const -> bool override { return true; }

    template<typename ...T>
    explicit Option(const char* defaultValue, const T ...identifier)
        : defaultValue(defaultValue), ids { identifier... } {
        static_assert(sizeof...(T) >= 1, "At least one identifier must be provided");
    }
private:
    const std::vector<std::string> ids;
};

namespace std {
    template<> struct hash<Switch> {
        std::size_t operator ()(const Switch& s) const noexcept {
            std::size_t hash = 1337;

            for (const std::string& id : s.identifiers()) {
                auto h = std::hash<std::string>{}(id);
                hash = (((h * 97) << 2u) * hash) % 117649312;
            }

            return hash;
        }
    };

    template<> struct hash<Flag> {
        std::size_t operator ()(const Flag& f) const noexcept {
            return std::hash<Switch>{}(f);
        }
    };

    template<> struct hash<Option> {
        std::size_t operator ()(const Option& f) const noexcept {
            return std::hash<Switch>{}(f);
        }
    };
}

class CommandLineParseResult {
public:
    const std::unordered_set<Flag> flags;
    const std::unordered_map<Option, std::string> options;

    auto hasFlag(const Flag& flag) -> bool {
        auto search = flags.find(flag);
        return search != flags.end();
    }

    auto hasOption(const Option& option) -> bool {
        auto search = options.find(option);
        return search != options.end();
    }

    auto getValue(const Option& option) -> std::string {
        auto search = options.find(option);
        if (search == options.end())
            return option.defaultValue;

        return search->second;
    }

    explicit CommandLineParseResult(std::unordered_set<Flag> flags, std::unordered_map<Option, std::string> options)
        : flags(std::move(flags)), options(std::move(options)) { }
};

class CommandLineParser {
public:
    const std::unordered_map<std::string, Switch*>& switches;

    auto parse() -> CommandLineParseResult;

    explicit CommandLineParser(const std::unordered_map<std::string, Switch*>& switches, int count, char** args)
        : switches(switches), count(count), args(args) { }
private:
    const int count;
    char** args;
};
