#pragma once

#include <functional>
#include <set>
#include <span>
#include <string>
#include <vector>

namespace algos {
using Facts = std::set<std::string, std::less<>>;

struct Rule {
    Facts conditions;
    std::string conclusion;
};

struct Inference {
    // Initial facts and all derived conclusions.
    Facts facts;
    // New conclusions in discovery order, each appearing once.
    std::vector<std::string> derived;

    auto operator==(const Inference &) const -> bool = default;
};

// Return the least rule-closed fact set and the discovery trace of new facts.
// Requires finite positive rules, applied in order until no new facts appear.
// All conditions must hold; empty conditions always hold.
// New facts can trigger later rules in the same round.
[[nodiscard]] auto infer(const Facts &facts, std::span<const Rule> rules)
    -> Inference;
} // namespace algos
