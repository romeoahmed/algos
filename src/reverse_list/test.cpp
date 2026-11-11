#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <utility>
#include <vector>

namespace {
// Record node addresses from head to the end.
auto addresses(const algos::ListNode *head)
    -> std::vector<const algos::ListNode *> {
    auto result = std::vector<const algos::ListNode *>{};
    for (auto node = head; node != nullptr; node = node->next.get()) {
        result.push_back(node);
    }
    return result;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "relinks the same nodes backwards"_test = [] -> void {
        expect(rc::check([](const std::vector<int> &values) -> void {
            auto list = algos::make_list(values);
            RC_ASSERT(algos::list_values(list.get()) == values);
            auto before = addresses(list.get());
            const auto reversed = algos::reverse_list(std::move(list));
            RC_ASSERT(algos::list_values(reversed.get()) ==
                      (values | std::views::reverse |
                       std::ranges::to<std::vector>()));
            std::ranges::reverse(before);
            RC_ASSERT(addresses(reversed.get()) == before);
        }));
    };

    "empty list"_test = [] -> void {
        expect(algos::reverse_list(nullptr) == nullptr);
    };

    "a million nodes reverse and free without recursion"_test = [] -> void {
        constexpr auto size = 1'000'000;
        const auto values =
            std::views::iota(0, size) | std::ranges::to<std::vector>();
        const auto reversed = algos::reverse_list(algos::make_list(values));
        const auto result = algos::list_values(reversed.get());
        expect(that % result.size() == std::size_t{size});
        expect(std::ranges::equal(result, values | std::views::reverse));
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
