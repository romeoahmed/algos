#include "solution.hpp"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <rapidcheck.h>
#include <set>
#include <string>
#include <tuple>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    using algos::Heuristic;
    using algos::Roads;
    using algos::Route;

    "matches Floyd-Warshall"_test = [](bool zero_heuristic) -> void {
        expect(rc::check([zero_heuristic] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(1, 8);
            auto roads = Roads{};
            auto costs = std::vector(
                size, std::vector<std::optional<std::int64_t>>(size));
            for (auto node = std::size_t{0}; node < size; ++node) {
                roads.emplace(std::to_string(node), Roads::mapped_type{});
                costs.at(node).at(node) = 0;
            }
            const auto vertex = rc::gen::inRange(std::size_t{0}, size);
            const auto edges = *rc::gen::resize(
                35, rc::gen::container<std::vector<
                        std::tuple<std::size_t, std::size_t, std::int64_t>>>(
                        rc::gen::tuple(vertex, vertex,
                                       rc::gen::inRange<std::int64_t>(0, 31))));
            for (const auto &[from, to, distance] : edges) {
                roads.at(std::to_string(from))
                    .emplace_back(std::to_string(to), distance);
                auto &cost = costs.at(from).at(to);
                cost = std::min(cost.value_or(distance), distance);
            }
            for (auto middle = std::size_t{0}; middle < size; ++middle) {
                for (auto from = std::size_t{0}; from < size; ++from) {
                    for (auto to = std::size_t{0}; to < size; ++to) {
                        const auto left = costs.at(from).at(middle);
                        const auto right = costs.at(middle).at(to);
                        if (left && right) {
                            const auto candidate = *left + *right;
                            auto &cost = costs.at(from).at(to);
                            cost =
                                std::min(cost.value_or(candidate), candidate);
                        }
                    }
                }
            }
            const auto goal = *vertex;
            auto heuristic = Heuristic{};
            for (auto node = std::size_t{0}; node < size; ++node) {
                const auto bound = costs.at(node).at(goal).value_or(30);
                heuristic.emplace(
                    std::to_string(node),
                    zero_heuristic
                        ? 0
                        : *rc::gen::inRange(std::int64_t{0}, bound + 1));
            }
            const auto before = roads;
            const auto original_heuristic = heuristic;
            for (auto start = std::size_t{0}; start < size; ++start) {
                const auto result =
                    algos::a_star(roads, std::to_string(start),
                                  std::to_string(goal), heuristic);
                const auto expected = costs.at(start).at(goal);
                RC_ASSERT(result.has_value() == expected.has_value());
                if (result) {
                    RC_ASSERT(result->cost == *expected);
                    RC_ASSERT(!result->path.empty());
                    RC_ASSERT(result->path.front() == std::to_string(start));
                    RC_ASSERT(result->path.back() == std::to_string(goal));
                    const auto unique =
                        std::set(result->path.begin(), result->path.end());
                    RC_ASSERT(unique.size() == result->path.size());
                    auto total = std::int64_t{0};
                    for (auto i = std::size_t{1}; i < result->path.size();
                         ++i) {
                        auto distance = std::optional<std::int64_t>{};
                        for (const auto &[neighbor, weight] :
                             roads.at(result->path.at(i - 1))) {
                            if (neighbor == result->path.at(i)) {
                                distance =
                                    std::min(distance.value_or(weight), weight);
                            }
                        }
                        RC_ASSERT(distance.has_value());
                        total += *distance;
                    }
                    RC_ASSERT(total == result->cost);
                }
            }
            RC_ASSERT(roads == before);
            RC_ASSERT(heuristic == original_heuristic);
        }));
    } | std::array{false, true};

    using Example = std::tuple<Roads, Heuristic, std::optional<Route>>;
    "boundaries, reopening, cycles, parallel edges, and wide costs"_test =
        [](const auto &example) -> void {
        const auto &[roads, heuristic, expected] = example;
        expect(algos::a_star(roads, "S", "G", heuristic) == expected);
    } | std::vector<Example>{
            {{}, {{"S", 0}, {"G", 0}}, std::nullopt},
            {{{"S", {{"G", 10}, {"A", 1}}}, {"A", {{"G", 1}}}},
             {{"S", 2}, {"A", 1}, {"G", 0}},
             Route{.path = {"S", "A", "G"}, .cost = 2}},
            {{{"S", {{"A", 3}, {"B", 1}}},
              {"A", {{"G", 3}}},
              {"B", {{"A", 1}}}},
             {{"S", 0}, {"A", 0}, {"B", 4}, {"G", 0}},
             Route{.path = {"S", "B", "A", "G"}, .cost = 5}},
            {{{"S", {{"S", 0}, {"A", 0}}}, {"A", {{"S", 0}, {"G", 2}}}},
             {{"S", 0}, {"A", 0}, {"G", 0}},
             Route{.path = {"S", "A", "G"}, .cost = 2}},
            {{{"S", {{"A", 0}}}, {"A", {{"S", 0}}}},
             {{"S", 7}, {"A", 2}, {"G", 0}},
             std::nullopt},
            {{{"S", {{"G", 9}, {"G", 2}, {"G", 5}}}},
             {{"S", 2}, {"G", 0}},
             Route{.path = {"S", "G"}, .cost = 2}},
            {{{"S", {{"A", 3'000'000'000}}}, {"A", {{"G", 3'000'000'000}}}},
             {{"S", 6'000'000'000}, {"A", 3'000'000'000}, {"G", 0}},
             Route{.path = {"S", "A", "G"}, .cost = 6'000'000'000}}};

    "start equals goal with an omitted leaf"_test = [] -> void {
        expect(algos::a_star({}, "S", "S", {{"S", 0}}) ==
               std::optional{Route{.path = {"S"}, .cost = 0}});
    };

    "equal cost routes"_test = [] -> void {
        const auto roads = Roads{
            {"S", {{"B", 1}, {"A", 1}}}, {"A", {{"G", 1}}}, {"B", {{"G", 1}}}};
        const auto result = algos::a_star(
            roads, "S", "G", {{"S", 0}, {"A", 0}, {"B", 0}, {"G", 0}});
        expect(result ==
                   std::optional{Route{.path = {"S", "A", "G"}, .cost = 2}} ||
               result ==
                   std::optional{Route{.path = {"S", "B", "G"}, .cost = 2}});
    };

    "Romania route owns its result"_test = [] -> void {
        auto roads = Roads{
            {"Arad", {{"Zerind", 75}, {"Sibiu", 140}, {"Timisoara", 118}}},
            {"Zerind", {{"Arad", 75}, {"Oradea", 71}}},
            {"Oradea", {{"Zerind", 71}, {"Sibiu", 151}}},
            {"Sibiu",
             {{"Arad", 140},
              {"Oradea", 151},
              {"Fagaras", 99},
              {"Rimnicu", 80}}},
            {"Timisoara", {{"Arad", 118}, {"Lugoj", 111}}},
            {"Lugoj", {{"Timisoara", 111}, {"Mehadia", 70}}},
            {"Mehadia", {{"Lugoj", 70}, {"Drobeta", 75}}},
            {"Drobeta", {{"Mehadia", 75}, {"Craiova", 120}}},
            {"Craiova", {{"Drobeta", 120}, {"Rimnicu", 146}, {"Pitesti", 138}}},
            {"Rimnicu", {{"Sibiu", 80}, {"Craiova", 146}, {"Pitesti", 97}}},
            {"Fagaras", {{"Sibiu", 99}, {"Bucharest", 211}}},
            {"Pitesti",
             {{"Rimnicu", 97}, {"Craiova", 138}, {"Bucharest", 101}}},
            {"Bucharest",
             {{"Fagaras", 211},
              {"Pitesti", 101},
              {"Giurgiu", 90},
              {"Urziceni", 85}}},
            {"Giurgiu", {{"Bucharest", 90}}},
            {"Urziceni", {{"Bucharest", 85}, {"Hirsova", 98}, {"Vaslui", 142}}},
            {"Hirsova", {{"Urziceni", 98}, {"Eforie", 86}}},
            {"Eforie", {{"Hirsova", 86}}},
            {"Vaslui", {{"Urziceni", 142}, {"Iasi", 92}}},
            {"Iasi", {{"Vaslui", 92}, {"Neamt", 87}}},
            {"Neamt", {{"Iasi", 87}}}};
        auto heuristic =
            Heuristic{{"Arad", 366},    {"Zerind", 374},    {"Oradea", 380},
                      {"Sibiu", 253},   {"Timisoara", 329}, {"Lugoj", 244},
                      {"Mehadia", 241}, {"Drobeta", 242},   {"Craiova", 160},
                      {"Rimnicu", 193}, {"Fagaras", 176},   {"Pitesti", 100},
                      {"Bucharest", 0}, {"Giurgiu", 77},    {"Urziceni", 80},
                      {"Hirsova", 151}, {"Eforie", 161},    {"Vaslui", 199},
                      {"Iasi", 226},    {"Neamt", 234}};
        const auto expected = std::optional{
            Route{.path = {"Arad", "Sibiu", "Rimnicu", "Pitesti", "Bucharest"},
                  .cost = 418}};
        const auto before = roads;
        const auto original_heuristic = heuristic;
        auto result = algos::a_star(roads, "Arad", "Bucharest", heuristic);
        expect(result == expected);
        expect(roads == before);
        expect(heuristic == original_heuristic);
        if (result) {
            result->path.clear();
        }
        result = algos::a_star(roads, "Arad", "Bucharest", heuristic);
        roads.clear();
        heuristic.clear();
        expect(result == expected);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
