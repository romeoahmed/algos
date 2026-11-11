#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    using Sets = std::tuple<algos::ChainedHashSet, algos::ProbingHashSet>;

    // True adds and false removes; std::set is the reference.
    "matches std::set"_test = []<class HashSet> -> void {
        expect(rc::check([] -> void {
            // Small keys hit each other, multiples of 64 share low bits, and
            // the rest cover arbitrary integers.
            const auto keys = rc::gen::oneOf(
                rc::gen::inRange(-16, 17),
                rc::gen::map(rc::gen::inRange(-16, 17),
                             [](int key) -> int { return 64 * key; }),
                rc::gen::arbitrary<int>());
            const auto operations =
                *rc::gen::container<std::vector<std::pair<bool, int>>>(
                    rc::gen::pair(rc::gen::arbitrary<bool>(), keys));
            auto table = HashSet{};
            auto model = std::set<int>{};
            for (const auto &[adding, key] : operations) {
                if (adding) {
                    RC_ASSERT(table.add(key) == model.insert(key).second);
                } else {
                    RC_ASSERT(table.remove(key) == (model.erase(key) == 1));
                }
                RC_ASSERT(table.size() == model.size());
            }
            for (const auto &[_, key] : operations) {
                RC_ASSERT(table.contains(key) == model.contains(key));
            }
        }));
    } | Sets{};

    "removing a key keeps the others"_test = []<class HashSet> -> void {
        auto table = HashSet{};
        for (const auto key : {10, 18, 26, 34}) {
            expect(table.add(key));
        }
        expect(table.remove(18));
        expect(table.contains(26));
        expect(!table.contains(18));
        expect(table.add(42));
        expect(!table.add(26));
        expect(that % table.size() == std::size_t{4});
    } | Sets{};

    "churn clears tombstones"_test = []<class HashSet> -> void {
        auto table = HashSet{};
        auto changed = true;
        for (const auto key : std::views::iota(0, 100'000)) {
            changed = changed && table.add(key) && table.remove(key);
        }
        expect(changed);
        expect(that % table.size() == std::size_t{0});
        expect(!table.contains(0));
    } | Sets{};

    "many consecutive keys"_test = []<class HashSet> -> void {
        constexpr auto size = 20'000;
        auto table = HashSet{};
        const auto all = std::views::iota(-size, size);
        expect(std::ranges::all_of(
            all, [&](int key) -> bool { return table.add(key); }));
        expect(std::ranges::all_of(
            all | std::views::stride(2),
            [&](int key) -> bool { return table.remove(key); }));
        expect(that % table.size() == std::size_t{size});
        expect(std::ranges::all_of(all, [&](int key) -> bool {
            return table.contains(key) == (key % 2 != 0);
        }));
    } | Sets{};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
