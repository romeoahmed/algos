#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace algos {
namespace {
using Frontier = std::map<std::string, Route, std::less<>>;
using Costs = std::map<std::string, std::int64_t, std::less<>>;

auto search(const Roads &roads, std::string_view goal,
            const Heuristic &heuristic, Frontier frontier, Costs best)
    -> std::optional<Route> {
    if (frontier.empty()) {
        return std::nullopt;
    }
    const auto priority = [&heuristic](const auto &entry) -> auto {
        return std::pair{entry.second.cost + heuristic.at(entry.first),
                         std::string_view{entry.first}};
    };
    auto current =
        frontier.extract(std::ranges::min_element(frontier, {}, priority));
    auto &route = current.mapped();
    if (current.key() == goal) {
        return std::move(route);
    }
    if (const auto outgoing = roads.find(current.key());
        outgoing != roads.end()) {
        for (const auto &[neighbor, distance] : outgoing->second) {
            const auto cost = route.cost + distance;
            const auto known = best.find(neighbor);
            if (known == best.end() || cost < known->second) {
                best.insert_or_assign(neighbor, cost);
                auto path = route.path;
                path.push_back(neighbor);
                frontier.insert_or_assign(
                    neighbor, Route{.path = std::move(path), .cost = cost});
            }
        }
    }
    return search(roads, goal, heuristic, std::move(frontier), std::move(best));
}
} // namespace

auto a_star(const Roads &roads, std::string_view start, std::string_view goal,
            const Heuristic &heuristic) -> std::optional<Route> {
    return search(roads, goal, heuristic,
                  Frontier{{std::string{start},
                            Route{.path = {std::string{start}}, .cost = 0}}},
                  Costs{{std::string{start}, 0}});
}
} // namespace algos
