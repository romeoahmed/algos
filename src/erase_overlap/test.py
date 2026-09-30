from itertools import combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.erase_overlap.solution import erase_overlap


def minimum_removals(intervals: list[tuple[int, int]]) -> int:
    kept = max(
        size
        for size in range(len(intervals) + 1)
        for subset in combinations(intervals, size)
        if all(a[1] <= b[0] or b[1] <= a[0] for a, b in combinations(subset, 2))
    )
    return len(intervals) - kept


@given(st.lists(st.tuples(st.integers(-12, 12), st.integers(1, 8)), max_size=8))
def test_matches_all_subsets(data: list[tuple[int, int]]) -> None:
    intervals = [(start, start + width) for start, width in data]
    before = intervals.copy()
    expected = minimum_removals(intervals)
    assert erase_overlap(intervals) == expected
    assert erase_overlap(tuple(reversed(intervals))) == expected
    assert intervals == before


@pytest.mark.parametrize(
    ("intervals", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([(1, 2)], 0, id="singleton"),
        pytest.param([(1, 2), (2, 3), (3, 4), (1, 3)], 1, id="example"),
        pytest.param([(1, 2)] * 3, 2, id="duplicates"),
        pytest.param([(0, 10), (1, 2), (2, 3)], 1, id="nested"),
        pytest.param([(0, 3), (2, 4), (3, 6)], 1, id="shortest-is-not-best"),
        pytest.param([(-3, -2), (-2, -1)], 0, id="negative-touching"),
        pytest.param([(-(2**63), 0), (0, 2**63 - 1)], 0, id="wide-endpoints"),
    ],
)
def test_boundaries(intervals: list[tuple[int, int]], expected: int) -> None:
    assert erase_overlap(intervals) == expected
