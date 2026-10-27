from itertools import combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.knapsack.solution import Item, knapsack


def exhaustive(items: list[Item], capacity: int) -> int:
    """Try every subset of items, including the empty one, and keep the best fit."""
    return max(
        sum(item.value for item in chosen)
        for size in range(len(items) + 1)
        for chosen in combinations(items, size)
        if sum(item.weight for item in chosen) <= capacity
    )


items = st.builds(Item, st.integers(0, 12), st.integers(0, 50))


@given(st.lists(items, max_size=10), st.integers(0, 30))
def test_matches_exhaustive_search(items: list[Item], capacity: int) -> None:
    before = items.copy()
    expected = exhaustive(items, capacity)
    assert knapsack(items, capacity) == expected
    assert knapsack(iter(items), capacity) == expected
    assert items == before


@pytest.mark.parametrize(
    ("items", "capacity", "expected"),
    [
        pytest.param([], 5, 0, id="no-items"),
        pytest.param([Item(0, 4), Item(1, 9)], 0, 4, id="zero-capacity"),
        pytest.param([Item(6, 10)], 5, 0, id="too-heavy"),
        pytest.param([Item(1, 1)], 4, 1, id="once-only"),
        pytest.param([Item(2, 3), Item(3, 4), Item(4, 5)], 5, 7, id="example"),
        pytest.param(
            [Item(10, 60), Item(20, 100), Item(30, 120)], 50, 220, id="not-by-ratio"
        ),
        pytest.param(
            [Item(5, 10), Item(4, 40), Item(6, 30), Item(3, 50)], 10, 90, id="mixed"
        ),
        pytest.param([Item(1, 1)] * 200, 100, 100, id="many-items"),
    ],
)
def test_boundaries(items: list[Item], capacity: int, expected: int) -> None:
    assert knapsack(items, capacity) == expected
