#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {
auto exhaustive_partition(std::string_view text) -> std::vector<std::size_t> {
    if (text.empty()) {
        return {};
    }
    auto best = std::vector<std::size_t>{};
    for (auto mask = 0U; mask < (1U << (text.size() - 1)); ++mask) {
        auto seen = std::set<char>{};
        auto current = std::set<char>{};
        auto lengths = std::vector<std::size_t>{};
        auto start = std::size_t{0};
        auto valid = true;
        for (auto i = std::size_t{0}; i < text.size(); ++i) {
            valid = valid && !seen.contains(text.at(i));
            current.insert(text.at(i));
            if (i + 1 == text.size() || (mask & (1U << i)) != 0) {
                lengths.push_back(i + 1 - start);
                seen.insert(current.begin(), current.end());
                current.clear();
                start = i + 1;
            }
        }
        if (valid && lengths.size() > best.size()) {
            best = std::move(lengths);
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "matches all cut patterns"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(0, 11);
            auto text = *rc::gen::container<std::string>(
                size, rc::gen::inRange('a', 'e'));
            const auto before = text;
            const auto result = algos::partition_labels(text);
            const auto expected = exhaustive_partition(text);
            RC_ASSERT(result == expected);
            RC_ASSERT(text == before);
            text.clear();
            RC_ASSERT(result == expected);
        }));
    };
    "empty, chained closures, and distinct letters"_test =
        [](const auto &example) -> void {
        const auto &[text, expected] = example;
        expect(that % algos::partition_labels(text) == expected);
    } | std::vector<std::pair<std::string, std::vector<std::size_t>>>{
            {"", {}},
            {"a", {1}},
            {"ababcbacadefegdehijhklij", {9, 7, 8}},
            {"eccbbbbdec", {10}},
            {"abac", {3, 1}},
            {"abcdefghijklmnopqrstuvwxyz", std::vector<std::size_t>(26, 1)},
            {std::string(2000, 'a'), {2000}}};
    "reads only the view and owns its result"_test = [] -> void {
        auto text = std::string{"xabacy"};
        auto result =
            algos::partition_labels(std::string_view{text}.substr(1, 4));
        expect(that % result == std::vector<std::size_t>{3, 1});
        text.clear();
        expect(that % result == std::vector<std::size_t>{3, 1});
        result.clear();
        expect(that % algos::partition_labels("abac") ==
               std::vector<std::size_t>{3, 1});
        expect(that % algos::partition_labels("abc") ==
               std::vector<std::size_t>{1, 1, 1});
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
