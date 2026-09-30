#include "solution.hpp"

#include <ranges>
#include <span>
#include <string>
#include <string_view>

namespace algos {
namespace {
auto concat(std::span<const std::string> first,
            std::span<const std::string> second) -> Frontier {
    auto result = Frontier{};
    result.reserve(first.size() + second.size());
    result.append_range(first);
    result.append_range(second);
    return result;
}

auto search(const Tree &tree, std::span<const std::string> frontier,
            std::string_view goal, const auto &merge) -> Trace {
    auto trace = Trace{};
    const auto visit = [&](this const auto &self,
                           std::span<const std::string> pending) -> void {
        if (pending.empty()) {
            return;
        }
        const auto &node = pending.front();
        const auto rest = pending.subspan(1);
        trace.push_back({.node = node,
                         .frontier = rest | std::ranges::to<Frontier>(),
                         .found = node == goal});
        if (node == goal) {
            return;
        }
        const auto entry = tree.find(node);
        const auto children = entry == tree.end()
                                  ? std::span<const std::string>{}
                                  : std::span<const std::string>{entry->second};
        self(merge(rest, children));
    };
    visit(frontier);
    return trace;
}
} // namespace

auto bfs(const Tree &tree, std::string_view start, std::string_view goal)
    -> Trace {
    return search(tree, Frontier{std::string{start}}, goal, concat);
}

auto dfs(const Tree &tree, std::string_view start, std::string_view goal)
    -> Trace {
    return search(tree, Frontier{std::string{start}}, goal,
                  [](std::span<const std::string> rest,
                     std::span<const std::string> children) -> Frontier {
                      return concat(children, rest);
                  });
}
} // namespace algos
