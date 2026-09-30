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

// Return a minimum-cost route, or nullopt if the goal is unreachable.
// Requires a finite graph with nonnegative weights and 0 <= h(v) <= h*(v)
// for every vertex, including start and goal. Missing road keys have no
// outgoing edges. Every evaluated g + distance and g + h must fit in int64_t.
[[nodiscard]] auto a_star(const Roads &roads, std::string_view start,
                          std::string_view goal, const Heuristic &heuristic)
    -> std::optional<Route>;
} // namespace algos
