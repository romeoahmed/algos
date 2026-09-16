#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace algos {
using Roads =
    std::map<std::string, std::vector<std::pair<std::string, std::int64_t>>,
             std::less<>>;
using Heuristic = std::map<std::string, std::int64_t, std::less<>>;

struct Route {
    std::vector<std::string> path;
    std::int64_t cost = 0;

    auto operator==(const Route &) const -> bool = default;
};

// Returns a minimum-cost route, or nullopt when unreachable.
// Requires a finite graph, nonnegative weights, and 0 <= h(v) <= h*(v)
// for every vertex, including start and goal. Missing keys have no outgoing
// edges. All evaluated g + distance and g + h sums must fit in int64_t.
[[nodiscard]] auto a_star(const Roads &roads, std::string_view start,
                          std::string_view goal, const Heuristic &heuristic)
    -> std::optional<Route>;
} // namespace algos
