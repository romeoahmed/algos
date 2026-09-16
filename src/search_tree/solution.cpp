#include "solution.hpp"

#include <ranges>
#include <string>
#include <string_view>

namespace algos {
namespace {
auto concat(Frontier first, const Frontier &second) -> Frontier {
    first.append_range(second);
    return first;
}

auto search(const Tree &tree, const Frontier &frontier, std::string_view goal,
            const auto &merge) -> Trace {
    if (frontier.empty()) {
        return {};
    }
    const auto &node = frontier.front();
    const auto rest =
        frontier | std::views::drop(1) | std::ranges::to<Frontier>();
    auto trace = Trace{{.node = node, .frontier = rest, .found = node == goal}};
    if (node != goal) {
        const auto entry = tree.find(node);
        const auto children = entry == tree.end() ? Frontier{} : entry->second;
        trace.append_range(search(tree, merge(rest, children), goal, merge) |
                           std::views::as_rvalue);
    }
    return trace;
}
} // namespace

auto bfs(const Tree &tree, std::string_view start, std::string_view goal)
    -> Trace {
    return search(tree, Frontier{std::string{start}}, goal, concat);
}

auto dfs(const Tree &tree, std::string_view start, std::string_view goal)
    -> Trace {
    return search(
        tree, Frontier{std::string{start}}, goal,
        [](const Frontier &rest, const Frontier &children) -> Frontier {
            return concat(children, rest);
        });
}
} // namespace algos
