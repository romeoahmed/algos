import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.rain_water.solution import rain_water


def fill_by_levels(heights: list[int]) -> int:
    """Count each unit cell below a level that has a wall on both sides."""
    return sum(
        1
        for level in range(1, max(heights, default=0) + 1)
        for i, height in enumerate(heights)
        if height < level
        and any(wall >= level for wall in heights[:i])
        and any(wall >= level for wall in heights[i + 1 :])
    )


@given(st.lists(st.integers(0, 8), max_size=30))
def test_matches_level_filling(heights: list[int]) -> None:
    before = heights.copy()
    expected = fill_by_levels(heights)
    assert rain_water(heights) == expected
    assert rain_water(tuple(heights)) == expected
    assert heights == before


@pytest.mark.parametrize(
    ("heights", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([3], 0, id="singleton"),
        pytest.param([1, 2, 3], 0, id="rising"),
        pytest.param([3, 2, 1], 0, id="falling"),
        pytest.param([3, 0, 3], 3, id="basin"),
        pytest.param([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1], 6, id="example"),
        pytest.param([4, 2, 0, 3, 2, 5], 9, id="lower-left-wall"),
        pytest.param([5, 0, 0, 1], 2, id="lower-right-wall"),
    ],
)
def test_boundaries(heights: list[int], expected: int) -> None:
    assert rain_water(heights) == expected
