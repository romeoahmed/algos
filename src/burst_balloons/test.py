from itertools import combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.burst_balloons.solution import burst_balloons


def minimum_cover(intervals: list[tuple[int, int]]) -> int:
    # Any arrow can move to the smallest right endpoint among the intervals it hits.
    endpoints = {end for _, end in intervals}
    return min(
        size
        for size in range(len(endpoints) + 1)
        for points in combinations(endpoints, size)
        if all(
            any(start <= point <= end for point in points) for start, end in intervals
        )
    )


@given(st.lists(st.tuples(st.integers(-12, 12), st.integers(0, 8)), max_size=7))
def test_matches_all_endpoint_covers(data: list[tuple[int, int]]) -> None:
    intervals = [(start, start + width) for start, width in data]
    before = intervals.copy()
    expected = minimum_cover(intervals)
    assert burst_balloons(intervals) == expected
    assert burst_balloons(tuple(reversed(intervals))) == expected
    assert intervals == before


@pytest.mark.parametrize(
    ("intervals", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([(1, 1)], 1, id="point"),
        pytest.param([(10, 16), (2, 8), (1, 6), (7, 12)], 2, id="example"),
        pytest.param([(1, 2), (2, 3)], 1, id="touching"),
        pytest.param([(1, 2), (2, 3), (3, 4)], 2, id="nontransitive-overlap"),
        pytest.param([(1, 2)] * 3, 1, id="duplicates"),
        pytest.param([(0, 10), (1, 2), (8, 9)], 2, id="nested"),
        pytest.param([(1, 1), (2, 2)], 2, id="separate-points"),
        pytest.param([(-(2**63), 0), (0, 2**63 - 1)], 1, id="wide-endpoints"),
    ],
)
def test_boundaries(intervals: list[tuple[int, int]], expected: int) -> None:
    assert burst_balloons(intervals) == expected
