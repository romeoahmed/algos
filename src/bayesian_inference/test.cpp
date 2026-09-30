#include "solution.hpp"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <exception>
#include <iostream>
#include <map>
#include <ranges>
#include <rapidcheck.h>
#include <tuple>
#include <vector>

namespace {
// Sum full assignments independently of recursive marginalization.
auto event_masses(const std::vector<algos::BooleanNode> &network,
                  std::size_t query,
                  const std::map<std::size_t, bool> &evidence)
    -> std::array<double, 2> {
    auto masses = std::array{0.0, 0.0};
    for (const auto mask :
         std::views::iota(std::size_t{0}, std::size_t{1} << network.size())) {
        const auto value = [mask](std::size_t index) -> bool {
            return ((mask >> index) & 1U) != 0;
        };
        if (!std::ranges::all_of(evidence, [&value](const auto &entry) -> bool {
                return value(entry.first) == entry.second;
            })) {
            continue;
        }
        auto probability = 1.0;
        for (const auto index :
             std::views::iota(std::size_t{0}, network.size())) {
            const auto &node = network.at(index);
            auto row = std::size_t{0};
            for (const auto bit :
                 std::views::iota(std::size_t{0}, node.parents.size())) {
                const auto parent =
                    node.parents.at(node.parents.size() - 1 - bit);
                row += static_cast<std::size_t>(value(parent)) << bit;
            }
            const auto p_true = node.probabilities.at(row);
            probability *= value(index) ? p_true : 1 - p_true;
        }
        masses.at(static_cast<std::size_t>(value(query))) += probability;
    }
    return masses;
}

auto close(double actual, double expected) -> bool {
    return std::abs(actual - expected) < 1e-12;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    using algos::bayesian_inference;
    using algos::BooleanNode;
    "matches complete event tables and preserves inputs"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = *rc::gen::inRange<std::size_t>(1, 7);
            auto network = std::vector<BooleanNode>{};
            auto evidence = std::map<std::size_t, bool>{};
            for (const auto index : std::views::iota(std::size_t{0}, size)) {
                const auto mask =
                    *rc::gen::inRange(std::size_t{0}, std::size_t{1} << index);
                auto parents =
                    std::views::iota(std::size_t{0}, index) |
                    std::views::filter([mask](std::size_t parent) -> bool {
                        return ((mask >> parent) & 1U) != 0;
                    }) |
                    std::ranges::to<std::vector>();
                if (*rc::gen::arbitrary<bool>()) {
                    std::ranges::reverse(parents);
                }
                const auto numerators = *rc::gen::container<std::vector<int>>(
                    std::size_t{1} << parents.size(), rc::gen::inRange(1, 10));
                network.push_back(BooleanNode{
                    .parents = parents,
                    .probabilities =
                        numerators |
                        std::views::transform([](int numerator) -> double {
                            return static_cast<double>(numerator) / 10;
                        }) |
                        std::ranges::to<std::vector>()});
                if (*rc::gen::arbitrary<bool>()) {
                    evidence.emplace(index, *rc::gen::arbitrary<bool>());
                }
            }
            const auto original_network = network;
            const auto original_evidence = evidence;
            for (const auto query : std::views::iota(std::size_t{0}, size)) {
                const auto masses = event_masses(network, query, evidence);
                const auto result =
                    bayesian_inference(network, query, evidence);
                RC_ASSERT(close(result,
                                masses.at(1) / (masses.at(0) + masses.at(1))));
                RC_ASSERT(result >= 0 && result <= 1);
            }
            RC_ASSERT(network == original_network);
            RC_ASSERT(evidence == original_evidence);
        }));
    };
    "singleton priors and observed queries"_test = [](double prior) -> void {
        const auto network =
            std::vector<BooleanNode>{{.parents = {}, .probabilities = {prior}}};
        expect(close(bayesian_inference(network, 0, {}), prior));
        if (prior > 0) {
            expect(bayesian_inference(network, 0, {{0, true}}) == 1.0);
        }
        if (prior < 1) {
            expect(bayesian_inference(network, 0, {{0, false}}) == 0.0);
        }
    } | std::array{0.0, 0.1, 0.5, 1.0};
    "deterministic child and disconnected evidence"_test =
        [](bool value) -> void {
        const auto network = std::vector<BooleanNode>{
            {.parents = {}, .probabilities = {0.3}},
            {.parents = {0}, .probabilities = {0.0, 1.0}},
            {.parents = {}, .probabilities = {0.7}}};
        expect(bayesian_inference(network, 0, {{1, value}}) ==
               static_cast<double>(value));
        expect(close(bayesian_inference(network, 0, {{2, value}}), 0.3));
    } | std::array{false, true};
    "deterministic chains preserve priors and reveal ancestors"_test =
        [] -> void {
        auto network =
            std::vector<BooleanNode>{{.parents = {}, .probabilities = {0.3}}};
        network.append_range(
            std::views::iota(std::size_t{1}, std::size_t{80}) |
            std::views::transform([](std::size_t index) -> BooleanNode {
                return {.parents = {index - 1}, .probabilities = {0.0, 1.0}};
            }));
        const auto last = network.size() - 1;
        expect(close(bayesian_inference(network, last, {}), 0.3));
        expect(bayesian_inference(network, 0, {{last, true}}) == 1.0);
        expect(bayesian_inference(network, 0, {{last, false}}) == 0.0);
    };
    "table rows follow the declared parent order"_test =
        [](const auto &example) -> void {
        const auto &[first, second, expected] = example;
        const auto network = std::vector<BooleanNode>{
            {.parents = {}, .probabilities = {0.3}},
            {.parents = {}, .probabilities = {0.6}},
            {.parents = {1, 0}, .probabilities = {0.1, 0.2, 0.7, 0.9}}};
        expect(close(bayesian_inference(network, 2, {{0, first}, {1, second}}),
                     expected));
    } | std::array{std::tuple{false, false, 0.1}, std::tuple{true, false, 0.2},
                   std::tuple{false, true, 0.7}, std::tuple{true, true, 0.9}};
    "sprinkler example and repeated calls"_test = [] -> void {
        const auto network = std::vector<BooleanNode>{
            {.parents = {}, .probabilities = {0.5}},
            {.parents = {0}, .probabilities = {0.5, 0.1}},
            {.parents = {0}, .probabilities = {0.2, 0.8}},
            {.parents = {1, 2}, .probabilities = {0.0, 0.9, 0.9, 0.99}}};
        const auto masses = event_masses(network, 2, {{3, true}});
        expect(close(masses.at(0), 0.189));
        expect(close(masses.at(1), 0.4581));
        expect(close(bayesian_inference(network, 3, {}), 0.6471));
        const auto posterior = bayesian_inference(network, 2, {{3, true}});
        expect(close(posterior, 0.4581 / 0.6471));
        expect(close(bayesian_inference(network, 2, {}), 0.5));
        expect(bayesian_inference(network, 2, {{3, true}}) == posterior);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
