from hypothesis import example, given
from hypothesis import strategies as st

from src.quickselect.solution import quickselect


@given(st.lists(st.integers(), min_size=1))
@example([7])
@example([3, 3, 3])
@example([3, 1, 3, 2])
def test_each_rank_matches_descending_order(values: list[int]) -> None:
    before = values.copy()
    for rank, expected in enumerate(sorted(values, reverse=True), start=1):
        assert quickselect(values, rank) == expected
        assert quickselect(tuple(values), rank) == expected
    assert values == before
