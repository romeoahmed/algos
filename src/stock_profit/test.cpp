#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Try every buy day with every later sell day, or no trade.
auto brute_force(const std::vector<int> &prices) -> std::int64_t {
    auto best = std::int64_t{0};
    for (auto buy = std::size_t{0}; buy < prices.size(); ++buy) {
        for (auto sell = buy + 1; sell < prices.size(); ++sell) {
            best = std::max(best, std::int64_t{prices.at(sell)} -
                                      std::int64_t{prices.at(buy)});
        }
    }
    return best;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    constexpr auto low = std::numeric_limits<int>::min();
    constexpr auto high = std::numeric_limits<int>::max();

    "matches brute force"_test = [] -> void {
        expect(rc::check([](std::vector<int> prices) -> void {
            const auto before = prices;
            RC_ASSERT(algos::stock_profit(prices) == brute_force(prices));
            RC_ASSERT(prices == before);
        }));
    };

    "empty, falling prices, later lows, and integer limits"_test =
        [](const auto &example) -> void {
        const auto &[prices, expected] = example;
        expect(that % algos::stock_profit(prices) == expected);
    } | std::vector<std::pair<std::vector<int>, std::int64_t>>{
            {{}, 0},
            {{5}, 0},
            {{7, 1, 5, 3, 6, 4}, 5},
            {{7, 6, 4, 3, 1}, 0},
            {{2, 4, 1}, 2},
            {{3, 8, 1, 6}, 5},
            {{high, high}, 0},
            {{low, high}, std::int64_t{high} - low}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
