#include "solution.hpp"

#include <boost/ut.hpp>
#include <exception>
#include <iostream>
#include <queue>
#include <ranges>
#include <rapidcheck.h>
#include <utility>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;

    // Each step pushes its value when the flag is set and pops otherwise;
    // std::queue is the reference.
    "matches std::queue"_test = [] -> void {
        expect(rc::check(
            [](const std::vector<std::pair<bool, int>> &operations) -> void {
                auto queue = algos::StackQueue{};
                auto model = std::queue<int>{};
                for (const auto &[pushes, value] : operations) {
                    if (pushes) {
                        queue.push(value);
                        model.push(value);
                    } else if (!model.empty()) {
                        RC_ASSERT(queue.pop() == model.front());
                        model.pop();
                    }
                    RC_ASSERT(queue.size() == model.size());
                    RC_ASSERT(queue.empty() == model.empty());
                    if (!model.empty()) {
                        RC_ASSERT(queue.peek() == model.front());
                    }
                }
            }));
    };

    "example sequence"_test = [] -> void {
        auto queue = algos::StackQueue{};
        for (const auto value : {1, 2, 3}) {
            queue.push(value);
        }
        auto popped = std::vector{queue.pop()};
        queue.push(4);
        for (auto count = 0; count < 3; ++count) {
            popped.push_back(queue.pop());
        }
        expect(popped == std::vector{1, 2, 3, 4});
        expect(queue.empty());
    };

    "bulk values leave in arrival order"_test = [] -> void {
        constexpr auto size = 100'000;
        auto queue = algos::StackQueue{};
        for (const auto value : std::views::iota(0, size)) {
            queue.push(value);
        }
        const auto copy = queue;
        expect(that % copy.peek() == 0);
        auto in_order = true;
        for (const auto value : std::views::iota(0, size)) {
            in_order = in_order && queue.pop() == value;
        }
        expect(in_order);
        expect(queue.empty());
        expect(that % copy.size() == std::size_t{size});
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
