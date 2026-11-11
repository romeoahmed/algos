#include "solution.hpp"

#include <array>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {
constexpr auto pairs = std::array<std::string_view, 3>{"()", "[]", "{}"};

// Delete adjacent matching pairs until none remain; valid iff empty.
auto cancel_pairs(std::string text) -> bool {
    for (auto found = true; found;) {
        found = false;
        for (const auto pair : pairs) {
            if (const auto at = text.find(pair); at != std::string::npos) {
                text.erase(at, pair.size());
                found = true;
            }
        }
    }
    return text.empty();
}

// Each move opens a bracket of the given kind or closes the innermost one;
// whatever is still open is closed at the end.
auto balanced(const std::vector<std::pair<bool, std::size_t>> &moves)
    -> std::string {
    auto text = std::string{};
    auto open = std::string{};
    for (const auto &[opens, kind] : moves) {
        if (opens || open.empty()) {
            const auto pair = pairs.at(kind % pairs.size());
            text.push_back(pair.front());
            open.push_back(pair.back());
        } else {
            text.push_back(open.back());
            open.pop_back();
        }
    }
    text.append_range(open | std::views::reverse);
    return text;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "accepts balanced strings"_test = [] -> void {
        expect(rc::check(
            [](const std::vector<std::pair<bool, std::size_t>> &moves) -> void {
                RC_ASSERT(algos::valid_parentheses(balanced(moves)));
            }));
    };

    "matches cancelling pairs"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto text = *rc::gen::container<std::string>(
                rc::gen::elementOf(std::string{"()[]{}"}));
            RC_ASSERT(algos::valid_parentheses(text) == cancel_pairs(text));
        }));
    };

    "empty, crossed, unclosed, unopened, and deep nesting"_test =
        [](const auto &example) -> void {
        const auto &[text, expected] = example;
        expect(that % algos::valid_parentheses(text) == expected);
    } | std::vector<std::pair<std::string, bool>>{
            {"", true},
            {"()[]{}", true},
            {"{[()]}", true},
            {"(]", false},
            {"([)]", false},
            {"((", false},
            {")", false},
            {"())", false},
            {std::string(100'000, '(') + std::string(100'000, ')'), true}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
