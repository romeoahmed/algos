#pragma once

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace algos {
using Frontier = std::vector<std::string>;
using Tree = std::map<std::string, Frontier, std::less<>>;

struct Step {
    std::string node;
    Frontier frontier;
    bool found = false;

    auto operator==(const Step &) const -> bool = default;
};
using Trace = std::vector<Step>;

// Returns a trace through the goal, or exhausts the subtree rooted at start.
// Requires a finite ordered tree with unique labels; omitted keys are leaves.
// Steps record the frontier after removal but before expansion.
[[nodiscard]] auto bfs(const Tree &tree, std::string_view start,
                       std::string_view goal) -> Trace;
[[nodiscard]] auto dfs(const Tree &tree, std::string_view start,
                       std::string_view goal) -> Trace;
} // namespace algos
