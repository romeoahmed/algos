#include "solution.hpp"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <rapidcheck.h>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;

    "matches exhaustive substring search"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto text = *rc::gen::resize(
                20, rc::gen::container<std::string>(
                        rc::gen::element('a', 'b', 'c', 'd', 'e')));
            const auto k = *rc::gen::inRange(-1, 23);
            auto expected = std::size_t{0};
            for (auto first = text.begin(); first != text.end(); ++first) {
                auto counts = std::map<char, int>{};
                for (auto last = first; last != text.end(); ++last) {
                    ++counts[*last];
                    if (std::ranges::all_of(counts,
                                            [k](const auto &entry) -> bool {
                                                return entry.second >= k;
                                            })) {
                        expected = std::max(
                            expected, static_cast<std::size_t>(
                                          std::distance(first, last) + 1));
                    }
                }
            }
            RC_ASSERT(algos::longest_substring(text, k) == expected);
        }));
    };

    "empty, nonpositive threshold, and contiguous segments"_test =
        [](const auto &example) -> void {
        const auto &[text, k, expected] = example;
        expect(that % algos::longest_substring(text, k) == expected);
    } | std::vector<std::tuple<std::string_view, int, std::size_t>>{
            {"", 0, 0},
            {"abc", -1, 3},
            {"aaabb", 3, 3},
            {"ababbc", 2, 5},
            {"aabbcc", 3, 0},
            {"aabaa", 3, 0},
            {"abc", std::numeric_limits<int>::min(), 3},
            {"abc", std::numeric_limits<int>::max(), 0}};

    "single letter threshold"_test = [](int size) -> void {
        const auto text = std::string(static_cast<std::size_t>(size), 'z');
        expect(that % algos::longest_substring(text, size) == text.size());
        expect(that % algos::longest_substring(text, size + 1) ==
               std::size_t{0});
    } | std::array{0, 1, 10'000};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
