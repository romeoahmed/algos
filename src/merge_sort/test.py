from hypothesis import example, given
from hypothesis import strategies as st

from src.merge_sort.solution import merge_sort


@given(st.lists(st.integers()))
@example([])
@example([7])
@example([2, 1])
@example([3, 1, 2, 1])
def test_returns_sorted_copy(values: list[int]) -> None:
    before = values.copy()
    expected = sorted(values)
    result = merge_sort(values)
    assert result == expected
    assert merge_sort(tuple(values)) == expected
    result.append(0)
    assert values == before
