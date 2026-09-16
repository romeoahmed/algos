from itertools import pairwise

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.a_star.solution import Heuristic, Roads, Route, a_star


def shortest_costs(roads: Roads) -> dict[tuple[str, str], int]:
    costs = {(node, node): 0 for node in roads}
    for node, edges in roads.items():
        for neighbor, distance in edges:
            costs[node, neighbor] = min(costs.get((node, neighbor), distance), distance)
    for middle in roads:
        for start in roads:
            for goal in roads:
                if (start, middle) in costs and (middle, goal) in costs:
                    cost = costs[start, middle] + costs[middle, goal]
                    costs[start, goal] = min(costs.get((start, goal), cost), cost)
    return costs


@st.composite
def weighted_graphs(draw: st.DrawFn) -> tuple[Roads, str, Heuristic]:
    size = draw(st.integers(1, 7))
    vertices = tuple(map(str, range(size)))
    edges = draw(
        st.lists(
            st.tuples(
                st.sampled_from(vertices),
                st.sampled_from(vertices),
                st.integers(0, 30),
            ),
            max_size=35,
        )
    )
    roads = {
        node: [(neighbor, cost) for source, neighbor, cost in edges if source == node]
        for node in vertices
    }
    goal = draw(st.sampled_from(vertices))
    costs = shortest_costs(roads)
    heuristic = {
        node: draw(st.integers(0, costs.get((node, goal), 30))) for node in vertices
    }
    return roads, goal, heuristic


@pytest.mark.parametrize("zero_heuristic", [False, True])
@given(case=weighted_graphs())
def test_matches_floyd_warshall(
    case: tuple[Roads, str, Heuristic], zero_heuristic: bool
) -> None:
    roads, goal, estimates = case
    heuristic = dict.fromkeys(roads, 0) if zero_heuristic else dict(estimates)
    before = {node: tuple(edges) for node, edges in roads.items()}
    original_heuristic = heuristic.copy()
    costs = shortest_costs(roads)
    for start in roads:
        result = a_star(roads, start, goal, heuristic)
        if (start, goal) not in costs:
            assert result is None
        else:
            assert result is not None
            assert result.cost == costs[start, goal]
            assert result.path[0] == start
            assert result.path[-1] == goal
            assert len(set(result.path)) == len(result.path)
            assert result.cost == sum(
                min(cost for neighbor, cost in roads[node] if neighbor == following)
                for node, following in pairwise(result.path)
            )
    assert {node: tuple(edges) for node, edges in roads.items()} == before
    assert heuristic == original_heuristic


@pytest.mark.parametrize(
    ("roads", "heuristic", "start", "goal", "expected"),
    [
        pytest.param({}, {"S": 0}, "S", "S", Route(("S",), 0), id="same-vertex"),
        pytest.param(
            {}, {"S": 0, "G": 0}, "S", "G", None, id="unreachable-missing-keys"
        ),
        pytest.param(
            {"S": [("G", 10), ("A", 1)], "A": [("G", 1)]},
            {"S": 2, "A": 1, "G": 0},
            "S",
            "G",
            Route(("S", "A", "G"), 2),
            id="goal-must-be-selected",
        ),
        pytest.param(
            {"S": [("A", 3), ("B", 1)], "A": [("G", 3)], "B": [("A", 1)]},
            {"S": 0, "A": 0, "B": 4, "G": 0},
            "S",
            "G",
            Route(("S", "B", "A", "G"), 5),
            id="reopen-with-inconsistent-heuristic",
        ),
        pytest.param(
            {"S": [("S", 0), ("A", 0)], "A": [("S", 0), ("G", 2)]},
            {"S": 0, "A": 0, "G": 0},
            "S",
            "G",
            Route(("S", "A", "G"), 2),
            id="zero-weight-cycle",
        ),
        pytest.param(
            {"S": [("A", 0)], "A": [("S", 0)]},
            {"S": 7, "A": 2, "G": 0},
            "S",
            "G",
            None,
            id="unreachable-through-cycle",
        ),
        pytest.param(
            {"S": [("G", 9), ("G", 2), ("G", 5)]},
            {"S": 2, "G": 0},
            "S",
            "G",
            Route(("S", "G"), 2),
            id="parallel-edges",
        ),
        pytest.param(
            {"S": [("A", 3_000_000_000)], "A": [("G", 3_000_000_000)]},
            {"S": 6_000_000_000, "A": 3_000_000_000, "G": 0},
            "S",
            "G",
            Route(("S", "A", "G"), 6_000_000_000),
            id="wide-cost-and-priority",
        ),
    ],
)
def test_boundaries(
    roads: Roads, heuristic: Heuristic, start: str, goal: str, expected: Route | None
) -> None:
    assert a_star(roads, start, goal, heuristic) == expected


def test_equal_cost_routes() -> None:
    roads = {"S": [("B", 1), ("A", 1)], "A": [("G", 1)], "B": [("G", 1)]}
    result = a_star(roads, "S", "G", dict.fromkeys(("S", "A", "B", "G"), 0))
    assert result in (Route(("S", "A", "G"), 2), Route(("S", "B", "G"), 2))


def test_romania() -> None:
    roads = {
        "Arad": [("Zerind", 75), ("Sibiu", 140), ("Timisoara", 118)],
        "Zerind": [("Arad", 75), ("Oradea", 71)],
        "Oradea": [("Zerind", 71), ("Sibiu", 151)],
        "Sibiu": [("Arad", 140), ("Oradea", 151), ("Fagaras", 99), ("Rimnicu", 80)],
        "Timisoara": [("Arad", 118), ("Lugoj", 111)],
        "Lugoj": [("Timisoara", 111), ("Mehadia", 70)],
        "Mehadia": [("Lugoj", 70), ("Drobeta", 75)],
        "Drobeta": [("Mehadia", 75), ("Craiova", 120)],
        "Craiova": [("Drobeta", 120), ("Rimnicu", 146), ("Pitesti", 138)],
        "Rimnicu": [("Sibiu", 80), ("Craiova", 146), ("Pitesti", 97)],
        "Fagaras": [("Sibiu", 99), ("Bucharest", 211)],
        "Pitesti": [("Rimnicu", 97), ("Craiova", 138), ("Bucharest", 101)],
        "Bucharest": [
            ("Fagaras", 211),
            ("Pitesti", 101),
            ("Giurgiu", 90),
            ("Urziceni", 85),
        ],
        "Giurgiu": [("Bucharest", 90)],
        "Urziceni": [("Bucharest", 85), ("Hirsova", 98), ("Vaslui", 142)],
        "Hirsova": [("Urziceni", 98), ("Eforie", 86)],
        "Eforie": [("Hirsova", 86)],
        "Vaslui": [("Urziceni", 142), ("Iasi", 92)],
        "Iasi": [("Vaslui", 92), ("Neamt", 87)],
        "Neamt": [("Iasi", 87)],
    }
    heuristic = {
        "Arad": 366,
        "Zerind": 374,
        "Oradea": 380,
        "Sibiu": 253,
        "Timisoara": 329,
        "Lugoj": 244,
        "Mehadia": 241,
        "Drobeta": 242,
        "Craiova": 160,
        "Rimnicu": 193,
        "Fagaras": 176,
        "Pitesti": 100,
        "Bucharest": 0,
        "Giurgiu": 77,
        "Urziceni": 80,
        "Hirsova": 151,
        "Eforie": 161,
        "Vaslui": 199,
        "Iasi": 226,
        "Neamt": 234,
    }
    before = {node: list(edges) for node, edges in roads.items()}
    original_heuristic = heuristic.copy()
    result = a_star(roads, "Arad", "Bucharest", heuristic)
    assert result == Route(("Arad", "Sibiu", "Rimnicu", "Pitesti", "Bucharest"), 418)
    assert roads == before
    assert heuristic == original_heuristic
    roads.clear()
    heuristic.clear()
    assert result == Route(("Arad", "Sibiu", "Rimnicu", "Pitesti", "Bucharest"), 418)
