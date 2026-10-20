import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.min_cost_stairs.solution import min_cost_stairs


def search_routes(costs: list[int]) -> int:
    """Explore every route forward from each start without sharing results."""

    def leave(i: int) -> int:
        if i >= len(costs):
            return 0
        return costs[i] + min(leave(i + 1), leave(i + 2))

    return min(leave(0), leave(1))


@given(st.lists(st.integers(0, 1000), min_size=2, max_size=16))
def test_matches_route_search(costs: list[int]) -> None:
    before = costs.copy()
    expected = search_routes(costs)
    assert min_cost_stairs(costs) == expected
    assert min_cost_stairs(iter(costs)) == expected
    assert costs == before


@pytest.mark.parametrize(
    ("costs", "expected"),
    [
        pytest.param([0, 0], 0, id="free"),
        pytest.param([5, 3], 3, id="start-on-second"),
        pytest.param([10, 15, 20], 15, id="example"),
        pytest.param([1, 100, 1, 1, 1, 100, 1, 1, 100, 1], 6, id="avoid-peaks"),
        pytest.param([1] * 1000, 500, id="long"),
    ],
)
def test_boundaries(costs: list[int], expected: int) -> None:
    assert min_cost_stairs(costs) == expected
