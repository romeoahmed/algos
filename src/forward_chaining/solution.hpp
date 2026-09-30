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
    // Includes the initial facts and every derived conclusion.
    Facts facts;
    // New facts in discovery order, without duplicates or initial facts.
    std::vector<std::string> derived;

    auto operator==(const Inference &) const -> bool = default;
};

// Applies rules in order until a full round adds no new facts.
// All conditions must hold; empty conditions always hold.
// New facts can trigger later rules in the same round.
// Requires finite positive rules. Preserves inputs and performs no output.
[[nodiscard]] auto infer(const Facts &facts, std::span<const Rule> rules)
    -> Inference;
} // namespace algos
