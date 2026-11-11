import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.longest_consecutive.solution import longest_consecutive


def scan_sorted(values: list[int]) -> int:
    """Sort the distinct values and measure the longest run of step 1."""
    best = run = 0
    previous: int | None = None
    for value in sorted(set(values)):
        run = run + 1 if previous is not None and value == previous + 1 else 1
        best = max(best, run)
        previous = value
    return best


@given(st.lists(st.integers(-20, 20)) | st.lists(st.integers()))
def test_matches_sorted_scan(values: list[int]) -> None:
    before = values.copy()
    expected = scan_sorted(values)
    assert longest_consecutive(values) == expected
    assert longest_consecutive(iter(values)) == expected
    assert values == before


@pytest.mark.parametrize(
    ("values", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([100, 4, 200, 1, 3, 2], 4, id="example"),
        pytest.param([0, 3, 7, 2, 5, 8, 4, 6, 0, 1], 9, id="duplicates"),
        pytest.param([5, 5, 5], 1, id="all-equal"),
        pytest.param([-2, -1, 0, 1], 4, id="through-zero"),
        pytest.param(list(range(100_000, 0, -1)), 100_000, id="linear-time"),
    ],
)
def test_boundaries(values: list[int], expected: int) -> None:
    assert longest_consecutive(values) == expected
