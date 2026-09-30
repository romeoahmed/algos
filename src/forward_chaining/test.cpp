#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <string>
#include <tuple>
#include <vector>

namespace {
auto facts_from_mask(unsigned int mask) -> algos::Facts {
    return std::views::iota(0U, 4U) |
           std::views::filter([mask](unsigned int i) -> bool {
               return (mask & (1U << i)) != 0;
           }) |
           std::views::transform([](unsigned int i) -> std::string {
               return {static_cast<char>('a' + i)};
           }) |
           std::ranges::to<algos::Facts>();
}

// Intersect all closed models over the four generated proposition names.
auto least_model(const algos::Facts &facts,
                 const std::vector<algos::Rule> &rules) -> algos::Facts {
    auto result = facts_from_mask(15U);
    for (auto mask = 0U; mask < 16U; ++mask) {
        const auto model = facts_from_mask(mask);
        const auto closed =
            std::ranges::all_of(rules, [&model](const auto &rule) -> bool {
                return !std::ranges::includes(model, rule.conditions) ||
                       model.contains(rule.conclusion);
            });
        if (std::ranges::includes(model, facts) && closed) {
            std::erase_if(result, [&model](const auto &fact) -> bool {
                return !model.contains(fact);
            });
        }
    }
    return result;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    using algos::Facts;
    using algos::Inference;
    using algos::Rule;
    "matches the intersection of all closed models"_test = [] -> void {
        expect(rc::check([] -> void {
            auto facts = facts_from_mask(*rc::gen::inRange(0U, 16U));
            const auto size = *rc::gen::inRange<std::size_t>(0, 9);
            auto rules = std::vector<Rule>{};
            for (auto i = std::size_t{0}; i < size; ++i) {
                rules.push_back(Rule{
                    .conditions = facts_from_mask(*rc::gen::inRange(0U, 16U)),
                    .conclusion = std::string(1, *rc::gen::inRange('a', 'e'))});
            }
            const auto original_facts = facts;
            const auto original_rules = rules;
            const auto expected = least_model(facts, rules);
            const auto result = algos::infer(facts, rules);
            RC_ASSERT(result.facts == expected);
            RC_ASSERT(facts == original_facts);
            RC_ASSERT(std::ranges::equal(
                rules, original_rules,
                [](const Rule &left, const Rule &right) -> bool {
                    return left.conditions == right.conditions &&
                           left.conclusion == right.conclusion;
                }));
            const auto again = algos::infer(result.facts, rules);
            RC_ASSERT(again.facts == result.facts);
            RC_ASSERT(again.derived.empty());
            auto known = facts;
            for (const auto &conclusion : result.derived) {
                RC_ASSERT(!known.contains(conclusion));
                RC_ASSERT(
                    std::ranges::any_of(rules, [&](const auto &rule) -> bool {
                        return rule.conclusion == conclusion &&
                               std::ranges::includes(known, rule.conditions);
                    }));
                known.insert(conclusion);
            }
            RC_ASSERT(known == result.facts);
            std::ranges::reverse(rules);
            RC_ASSERT(algos::infer(facts, rules).facts == result.facts);
            facts.clear();
            RC_ASSERT(result.facts == expected);
        }));
    };
    "ordered rounds, cycles, conjunctions, and empty conditions"_test =
        [](const auto &example) -> void {
        const auto &[facts, rules, derived] = example;
        auto expected = facts;
        expected.insert(derived.begin(), derived.end());
        expect(algos::infer(facts, rules) ==
               Inference{.facts = expected, .derived = derived});
    } | std::vector<std::tuple<Facts, std::vector<Rule>,
                               std::vector<std::string>>>{
            {{}, {}, {}},
            {{"a"}, {}, {}},
            {{},
             {{.conditions = {"a"}, .conclusion = "b"},
              {.conditions = {"b"}, .conclusion = "a"}},
             {}},
            {{"a"},
             {{.conditions = {"a"}, .conclusion = "b"},
              {.conditions = {"b"}, .conclusion = "a"}},
             {"b"}},
            {{},
             {{.conditions = {}, .conclusion = "a"},
              {.conditions = {"a"}, .conclusion = "b"},
              {.conditions = {"a"}, .conclusion = "b"}},
             {"a", "b"}},
            {{"a"},
             {{.conditions = {"b"}, .conclusion = "c"},
              {.conditions = {"a"}, .conclusion = "b"},
              {.conditions = {"b"}, .conclusion = "d"}},
             {"b", "d", "c"}},
            {{"a"}, {{.conditions = {"a", "b"}, .conclusion = "c"}}, {}},
            {{"a"}, {{.conditions = {"a"}, .conclusion = "a"}}, {}},
            {{"a"},
             {{.conditions = {"a"}, .conclusion = ""},
              {.conditions = {""}, .conclusion = "b"}},
             {"", "b"}},
            {{"a"},
             {{.conditions = {"d"}, .conclusion = "e"},
              {.conditions = {"c"}, .conclusion = "d"},
              {.conditions = {"b"}, .conclusion = "c"},
              {.conditions = {"a"}, .conclusion = "b"}},
             {"b", "c", "d", "e"}}};
    "classroom example returns independent facts and trace"_test = [] -> void {
        auto facts = Facts{"发烧", "咳嗽"};
        auto rules = std::vector<Rule>{
            {.conditions = {"发烧", "咳嗽"}, .conclusion = "可能流感"},
            {.conditions = {"可能流感"}, .conclusion = "进行流感检测"},
            {.conditions = {"进行流感检测"}, .conclusion = "等待检测结果"}};
        const auto expected =
            Inference{.facts = {"发烧", "咳嗽", "可能流感", "进行流感检测",
                                "等待检测结果"},
                      .derived = {"可能流感", "进行流感检测", "等待检测结果"}};
        auto result = algos::infer(facts, rules);
        expect(result == expected);
        result.facts.clear();
        result.derived.clear();
        result = algos::infer(facts, rules);
        facts.clear();
        rules.front().conditions.clear();
        rules.clear();
        expect(result == expected);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
