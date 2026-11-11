import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.two_sum.solution import two_sum


def all_pairs(values: list[int], target: int) -> list[tuple[int, int]]:
    """Try every pair of indices and keep those summing to target."""
    return [
        (i, j)
        for j in range(len(values))
        for i in range(j)
        if values[i] + values[j] == target
    ]


@given(
    st.lists(st.integers(-8, 8)) | st.lists(st.integers()),
    st.integers(-16, 16) | st.integers(),
)
def test_finds_the_earliest_completed_pair(values: list[int], target: int) -> None:
    before = values.copy()
    pairs = all_pairs(values, target)
    result = two_sum(values, target)
    if not pairs:
        assert result is None
    else:
        assert result is not None
        i, j = result
        assert i < j
        assert values[i] + values[j] == target
        assert j == min(second for _, second in pairs)
    assert two_sum(iter(values), target) == result
    assert values == before


@pytest.mark.parametrize(
    ("values", "target", "expected"),
    [
        pytest.param([2, 7, 11, 15], 9, (0, 1), id="example"),
        pytest.param([3, 2, 4], 6, (1, 2), id="skip-self"),
        pytest.param([3, 3], 6, (0, 1), id="equal-halves"),
        pytest.param([3], 6, None, id="single"),
        pytest.param([], 0, None, id="empty"),
        pytest.param([1, 5, 1, 5], 6, (0, 1), id="first-completion"),
    ],
)
def test_boundaries(
    values: list[int], target: int, expected: tuple[int, int] | None
) -> None:
    assert two_sum(values, target) == expected
