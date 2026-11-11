#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <string>
#include <utility>
#include <vector>

namespace {
// Put each word in the first group whose first word sorts the same.
auto group_by_sorting(const std::vector<std::string> &words) -> algos::Groups {
    const auto sorted = [](std::string word) -> std::string {
        std::ranges::sort(word);
        return word;
    };
    auto groups = algos::Groups{};
    for (const auto &word : words) {
        const auto group =
            std::ranges::find_if(groups, [&](const auto &candidate) -> bool {
                return sorted(candidate.front()) == sorted(word);
            });
        if (group == groups.end()) {
            groups.push_back({word});
        } else {
            group->push_back(word);
        }
    }
    return groups;
}

// Short words over a few symbols, so anagrams are common.
auto words_over(std::vector<char> symbols)
    -> rc::Gen<std::vector<std::string>> {
    return rc::gen::container<std::vector<std::string>>(
        rc::gen::resize(4, rc::gen::container<std::string>(
                               rc::gen::elementOf(std::move(symbols)))));
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "both signatures match sorting on lowercase words"_test = [] -> void {
        expect(rc::check([] -> void {
            auto words = *words_over({'a', 'b', 'c'});
            const auto before = words;
            const auto expected = group_by_sorting(words);
            RC_ASSERT(algos::group_anagrams(words) == expected);
            RC_ASSERT(algos::group_anagrams_hashed(words) == expected);
            RC_ASSERT(words == before);
        }));
    };

    "byte counts handle any bytes"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto words = *words_over({'a', 'Z', '\0', '\xff'});
            RC_ASSERT(algos::group_anagrams_hashed(words) ==
                      group_by_sorting(words));
        }));
    };

    "example, empty words, repeats, and same letters"_test =
        [](const auto &example) -> void {
        const auto &[words, expected] = example;
        expect(algos::group_anagrams(words) == expected);
        expect(algos::group_anagrams_hashed(words) == expected);
    } | std::vector<std::pair<std::vector<std::string>, algos::Groups>>{
            {{"eat", "tea", "tan", "ate", "nat", "bat"},
             {{"eat", "tea", "ate"}, {"tan", "nat"}, {"bat"}}},
            {{""}, {{""}}},
            {{}, {}},
            {{"ab", "ba", "ab"}, {{"ab", "ba", "ab"}}},
            {{"aab", "abb"}, {{"aab"}, {"abb"}}}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
