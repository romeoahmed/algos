import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.house_robber.solution import house_robber


def exhaustive(values: list[int]) -> int:
    """Try every subset whose bit mask has no two adjacent bits."""
    return max(
        sum(value for i, value in enumerate(values) if mask >> i & 1)
        for mask in range(1 << len(values))
        if (mask & mask >> 1) == 0
    )


@given(st.lists(st.integers(), max_size=12))
def test_matches_exhaustive_search(values: list[int]) -> None:
    before = values.copy()
    expected = exhaustive(values)
    assert house_robber(values) == expected
    assert house_robber(iter(values)) == expected
    assert values == before


@pytest.mark.parametrize(
    ("values", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([5], 5, id="singleton"),
        pytest.param([-5], 0, id="skip-negative"),
        pytest.param([1, 2, 3, 1], 4, id="alternate"),
        pytest.param([2, 7, 9, 3, 1], 12, id="example"),
        pytest.param([3, 4, 3], 6, id="larger-neighbor-loses"),
        pytest.param([2, 1, 1, 2], 4, id="skip-two"),
        pytest.param([1] * 1000, 500, id="long"),
    ],
)
def test_boundaries(values: list[int], expected: int) -> None:
    assert house_robber(values) == expected
