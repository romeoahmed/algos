#include "solution.hpp"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <ranges>
#include <rapidcheck.h>
#include <set>
#include <string>
#include <string_view>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;

    "matches depth and sibling paths"_test = [](bool breadth_first) -> void {
        expect(rc::check([breadth_first] -> void {
            const auto size = *rc::gen::inRange(1, 31);
            auto tree = algos::Tree{{"0", {}}};
            auto parents = std::vector<int>{0};
            for (auto node = 1; node < size; ++node) {
                const auto parent = *rc::gen::inRange(0, node);
                parents.push_back(parent);
                auto &children = tree[std::to_string(parent)];
                const auto position = *rc::gen::inRange(
                    std::ptrdiff_t{0}, std::ssize(children) + 1);
                children.insert(std::next(children.begin(), position),
                                std::to_string(node));
                tree.emplace(std::to_string(node), algos::Frontier{});
            }
            auto paths = std::vector<std::vector<std::ptrdiff_t>>(
                static_cast<std::size_t>(size));
            for (auto node = 1; node < size; ++node) {
                const auto index = static_cast<std::size_t>(node);
                const auto parent = parents.at(index);
                const auto &children = tree.at(std::to_string(parent));
                paths.at(index) = paths.at(static_cast<std::size_t>(parent));
                paths.at(index).push_back(std::distance(
                    children.begin(),
                    std::ranges::find(children, std::to_string(node))));
            }
            auto order =
                std::views::iota(0, size) | std::ranges::to<std::vector>();
            std::ranges::sort(order, [&](int a, int b) -> bool {
                const auto &left = paths.at(static_cast<std::size_t>(a));
                const auto &right = paths.at(static_cast<std::size_t>(b));
                if (breadth_first && left.size() != right.size()) {
                    return left.size() < right.size();
                }
                return left < right;
            });
            const auto goal = *rc::gen::inRange(0, size + 1);
            auto visited = std::set<int>{};
            auto expected = algos::Trace{};
            for (const auto node : order) {
                auto rest = algos::Frontier{};
                for (const auto other : order) {
                    if (other != 0 && other != node &&
                        !visited.contains(other) &&
                        visited.contains(
                            parents.at(static_cast<std::size_t>(other)))) {
                        rest.push_back(std::to_string(other));
                    }
                }
                expected.push_back({.node = std::to_string(node),
                                    .frontier = rest,
                                    .found = node == goal});
                if (node == goal) {
                    break;
                }
                visited.insert(node);
            }
            const auto before = tree;
            const auto search = breadth_first ? algos::bfs : algos::dfs;
            auto result = search(tree, "0", std::to_string(goal));
            RC_ASSERT(result == expected);
            result.clear();
            RC_ASSERT(search(tree, "0", std::to_string(goal)) == expected);
            RC_ASSERT(tree == before);
        }));
    } | std::array{true, false};

    "omitted and explicit leaves"_test = [](auto search) -> void {
        for (const auto goal :
             std::array<std::string_view, 2>{"leaf", "missing"}) {
            const auto expected = algos::Trace{
                {.node = "leaf", .frontier = {}, .found = goal == "leaf"}};
            expect(std::ranges::equal(search({}, "leaf", goal), expected));
            expect(std::ranges::equal(search({{"leaf", {}}}, "leaf", goal),
                                      expected));
        }
    } | std::array{algos::bfs, algos::dfs};

    "stops before expanding the goal"_test = [] -> void {
        const auto tree =
            algos::Tree{{"A", {"B", "C"}}, {"B", {"D", "E"}}, {"C", {"F"}}};
        expect(std::ranges::equal(
            algos::bfs(tree, "A", "C"),
            algos::Trace{
                {.node = "A", .frontier = {}, .found = false},
                {.node = "B", .frontier = {"C"}, .found = false},
                {.node = "C", .frontier = {"D", "E"}, .found = true}}));
        expect(std::ranges::equal(
            algos::dfs(tree, "A", "C"),
            algos::Trace{{.node = "A", .frontier = {}, .found = false},
                         {.node = "B", .frontier = {"C"}, .found = false},
                         {.node = "D", .frontier = {"E", "C"}, .found = false},
                         {.node = "E", .frontier = {"C"}, .found = false},
                         {.node = "C", .frontier = {}, .found = true}}));
        for (const auto search : std::array{algos::bfs, algos::dfs}) {
            expect(std::ranges::equal(
                search(tree, "B", "E"),
                algos::Trace{{.node = "B", .frontier = {}, .found = false},
                             {.node = "D", .frontier = {"E"}, .found = false},
                             {.node = "E", .frontier = {}, .found = true}}));
        }
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
