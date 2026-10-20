import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.max_subarray.solution import max_subarray


def brute_force(values: list[int]) -> int:
    """Sum every nonempty contiguous slice."""
    return max(
        sum(values[start:stop])
        for start in range(len(values))
        for stop in range(start + 1, len(values) + 1)
    )


@given(st.lists(st.integers(), min_size=1, max_size=30))
def test_matches_brute_force(values: list[int]) -> None:
    before = values.copy()
    expected = brute_force(values)
    assert max_subarray(values) == expected
    assert max_subarray(iter(values)) == expected
    assert values == before


@pytest.mark.parametrize(
    ("values", "expected"),
    [
        pytest.param([-3], -3, id="negative-singleton"),
        pytest.param([-3, -1, -2], -1, id="all-negative"),
        pytest.param([-2, 1, -3, 4, -1, 2, 1, -5, 4], 6, id="example"),
        pytest.param([5, 4, -1, 7, 8], 23, id="whole-array"),
        pytest.param([3, -4, 3], 3, id="restart-ties"),
        pytest.param([0, 0], 0, id="zeros"),
    ],
)
def test_boundaries(values: list[int], expected: int) -> None:
    assert max_subarray(values) == expected
