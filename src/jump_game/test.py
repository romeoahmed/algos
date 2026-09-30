from itertools import pairwise

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.jump_game.solution import jump_game


def shortest_path(values: list[int]) -> int:
    """Find shortest paths in the forward graph without greedy layering."""
    distances = [len(values)] * len(values)
    distances[0] = 0
    for i, steps in enumerate(values):
        for j in range(i + 1, min(len(values), i + steps + 1)):
            distances[j] = min(distances[j], distances[i] + 1)
    return distances[-1]


@st.composite
def reachable_arrays(draw: st.DrawFn) -> list[int]:
    values = draw(st.lists(st.integers(0, 20), min_size=1, max_size=30))
    stops = sorted(
        draw(st.sets(st.integers(0, len(values) - 1))) | {0, len(values) - 1}
    )
    for start, stop in pairwise(stops):
        values[start] = max(values[start], stop - start)
    return values


@given(reachable_arrays())
def test_matches_shortest_path(values: list[int]) -> None:
    before = values.copy()
    expected = shortest_path(values)
    assert jump_game(values) == expected
    assert jump_game(tuple(values)) == expected
    assert values == before


@pytest.mark.parametrize(
    ("values", "expected"),
    [
        pytest.param([0], 0, id="singleton"),
        pytest.param([2, 3, 1, 1, 4], 2, id="example"),
        pytest.param([2, 3, 0, 1, 4], 2, id="zero-steps"),
        pytest.param([2, 3, 0, 0, 0], 2, id="avoid-dead-end"),
        pytest.param([1] * 80, 79, id="unit-steps"),
        pytest.param([10**100, 0, 0], 1, id="overshoot"),
        pytest.param([1, 10**100, 0], 2, id="overshoot-after-first-jump"),
    ],
)
def test_boundaries(values: list[int], expected: int) -> None:
    assert jump_game(values) == expected
