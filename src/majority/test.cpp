#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <rapidcheck.h>
#include <utility>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches frequency count after arbitrary interleaving"_test = [] -> void {
        expect(
            rc::check([](const std::vector<int> &noise, int candidate) -> void {
                const auto copies =
                    *rc::gen::inRange(noise.size() + 1, noise.size() + 3);
                auto values = std::vector<int>(copies, candidate);
                for (const auto value : noise) {
                    const auto position = *rc::gen::inRange(
                        std::ptrdiff_t{0}, std::ssize(values) + 1);
                    values.insert(std::next(values.begin(), position), value);
                }
                const auto before = values;
                RC_ASSERT(std::ranges::count(values, algos::majority(values)) >
                          std::ssize(values) / 2);
                RC_ASSERT(values == before);
            }));
    };

    "singleton and repeated cancellation"_test =
        [](const auto &example) -> void {
        const auto &[values, expected] = example;
        expect(that % algos::majority(values) == expected);
    } | std::vector<std::pair<std::vector<int>, int>>{
            {{low}, low},
            {{1, 2, 2, 2}, 2},
            {{2, 2, 1, 1, 1, 2, 2}, 2},
            {{low, high, low, high, high}, high}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
