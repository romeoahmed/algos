from itertools import combinations, pairwise

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.longest_increasing.solution import longest_increasing


def exhaustive(values: list[int]) -> int:
    """Check every subsequence, including the empty one."""
    return max(
        size
        for size in range(len(values) + 1)
        for chosen in combinations(values, size)
        if all(left < right for left, right in pairwise(chosen))
    )


@given(st.lists(st.integers(-6, 6), max_size=11))
def test_matches_exhaustive_search(values: list[int]) -> None:
    before = values.copy()
    expected = exhaustive(values)
    assert longest_increasing(values) == expected
    assert longest_increasing(tuple(values)) == expected
    assert values == before


@pytest.mark.parametrize(
    ("values", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([4], 1, id="singleton"),
        pytest.param([7, 7, 7], 1, id="equal-values"),
        pytest.param([10, 9, 2, 5, 3, 7, 101, 18], 4, id="example"),
        pytest.param([1, 100, 2, 3], 3, id="skip-neighbor"),
        pytest.param([0, 1, 0, 3, 2, 3], 4, id="repeated-values"),
        pytest.param(list(range(200, 0, -1)), 1, id="decreasing"),
        pytest.param(list(range(200)), 200, id="increasing"),
    ],
)
def test_boundaries(values: list[int], expected: int) -> None:
    assert longest_increasing(values) == expected
