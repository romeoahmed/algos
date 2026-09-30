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
    // Pending nodes after removing node, before adding its children.
    Frontier frontier;
    bool found = false;

    auto operator==(const Step &) const -> bool = default;
};
using Trace = std::vector<Step>;

// Both searches stop at the first match or exhaust the subtree rooted at start.
// Requires a finite ordered tree with unique labels; omitted keys are leaves.

// Return a breadth-first trace, preserving sibling order.
[[nodiscard]] auto bfs(const Tree &tree, std::string_view start,
                       std::string_view goal) -> Trace;

// Return a preorder trace, preserving sibling order.
[[nodiscard]] auto dfs(const Tree &tree, std::string_view start,
                       std::string_view goal) -> Trace;
} // namespace algos
