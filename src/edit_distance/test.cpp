#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace {
// Search single edits breadth-first, never longer than the longer word.
auto search_edits(const std::string &source, const std::string &target)
    -> std::size_t {
    const auto alphabet = std::set<char>(std::from_range, source + target);
    const auto limit = std::max(source.size(), target.size());
    auto frontier = std::set<std::string>{source};
    auto seen = frontier;
    auto distance = std::size_t{0};
    while (!frontier.contains(target)) {
        auto next = std::set<std::string>{};
        const auto visit = [&](std::string word) -> void {
            if (seen.insert(word).second) {
                next.insert(std::move(word));
            }
        };
        for (const auto &word : frontier) {
            for (auto i = std::size_t{0}; i <= word.size(); ++i) {
                for (const auto letter : alphabet) {
                    if (word.size() < limit) {
                        visit(std::string{word}.insert(i, 1, letter));
                    }
                    if (i < word.size()) {
                        visit(std::string{word}.replace(i, 1, 1, letter));
                    }
                }
                if (i < word.size()) {
                    visit(std::string{word}.erase(i, 1));
                }
            }
        }
        frontier = std::move(next);
        ++distance;
    }
    return distance;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "matches edit search in either order"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto word = [] -> std::string {
                const auto size = *rc::gen::inRange<std::size_t>(0, 6);
                return *rc::gen::container<std::string>(
                    size, rc::gen::element('a', 'b', 'c'));
            };
            const auto source = word();
            const auto target = word();
            const auto expected = search_edits(source, target);
            RC_ASSERT(algos::edit_distance(source, target) == expected);
            RC_ASSERT(algos::edit_distance(target, source) == expected);
        }));
    };

    "empty, identical, swapped, mixed, and long words"_test =
        [](const auto &example) -> void {
        const auto &[source, target, expected] = example;
        expect(that % algos::edit_distance(source, target) == expected);
    } | std::vector<std::tuple<std::string, std::string, std::size_t>>{
            {"", "", 0},
            {"", "abc", 3},
            {"abc", "", 3},
            {"abc", "abc", 0},
            {"ab", "ba", 2},
            {"horse", "ros", 3},
            {"intention", "execution", 5},
            {"kitten", "sitting", 3},
            {std::string(300, 'a'), std::string(200, 'b'), 300}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
