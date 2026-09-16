from collections.abc import Mapping, Sequence
from typing import NamedTuple

type Roads = Mapping[str, Sequence[tuple[str, int]]]
type Heuristic = Mapping[str, int]


class Route(NamedTuple):
    path: tuple[str, ...]
    cost: int


def a_star(roads: Roads, start: str, goal: str, heuristic: Heuristic) -> Route | None:
    """Return a minimum-cost route, or None if the goal is unreachable.

    Requires a finite graph with nonnegative integer weights and a nonnegative
    admissible heuristic for every vertex, including start and goal. Missing road
    keys have no outgoing edges. Adjacency sequences must be reusable.
    """

    def search(frontier: dict[str, Route], best: dict[str, int]) -> Route | None:
        if not frontier:
            return None
        node = min(
            frontier, key=lambda node: (frontier[node].cost + heuristic[node], node)
        )
        route = frontier[node]
        if node == goal:
            return route
        pending = {
            city: candidate for city, candidate in frontier.items() if city != node
        }
        costs = best.copy()
        for neighbor, distance in roads.get(node, ()):
            cost = route.cost + distance
            if neighbor not in costs or cost < costs[neighbor]:
                costs[neighbor] = cost
                pending[neighbor] = Route((*route.path, neighbor), cost)
        return search(pending, costs)

    return search({start: Route((start,), 0)}, {start: 0})
