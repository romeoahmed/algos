from hypothesis import example, given
from hypothesis import strategies as st

from src.rotated_search.solution import rotated_search


@given(st.lists(st.integers(), unique=True), st.integers(), st.integers())
@example([], 0, 0)
@example([1], 0, 2)
@example([0, 1, 2, 4, 5, 6, 7], 3, 3)
def test_matches_linear_search(values: list[int], offset: int, target: int) -> None:
    ordered = sorted(values)
    pivot = offset % len(ordered) if ordered else 0
    rotated = ordered[pivot:] + ordered[:pivot]
    before = rotated.copy()
    for index, value in enumerate(rotated):
        assert rotated_search(rotated, value) == index
    expected = next((i for i, value in enumerate(rotated) if value == target), -1)
    assert rotated_search(rotated, target) == expected
    assert rotated_search(tuple(rotated), target) == expected
    assert rotated == before
