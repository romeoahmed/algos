#include "solution.hpp"

#include <algorithm>
#include <span>
#include <string>
#include <utility>

namespace algos {
auto infer(const Facts &facts, std::span<const Rule> rules) -> Inference {
    const auto apply = [](Inference state, const Rule &rule) -> Inference {
        if (!state.facts.contains(rule.conclusion) &&
            std::ranges::all_of(rule.conditions,
                                [&state](const std::string &fact) -> bool {
                                    return state.facts.contains(fact);
                                })) {
            state.facts.insert(rule.conclusion);
            state.derived.push_back(rule.conclusion);
        }
        return state;
    };
    const auto close = [rules, &apply](this const auto &self,
                                       Inference state) -> Inference {
        const auto count = state.facts.size();
        auto following = std::ranges::fold_left(rules, std::move(state), apply);
        if (following.facts.size() == count) {
            return following;
        }
        return self(std::move(following));
    };
    return close(Inference{.facts = facts, .derived = {}});
}
} // namespace algos
