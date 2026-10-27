from itertools import combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.longest_common.solution import longest_common


def exhaustive(first: str, second: str) -> int:
    """Try every subsequence of first and keep the longest one found in second."""

    def within(chosen: tuple[str, ...]) -> bool:
        remaining = iter(second)
        return all(char in remaining for char in chosen)

    return max(
        size
        for size in range(len(first) + 1)
        for chosen in combinations(first, size)
        if within(chosen)
    )


texts = st.text(alphabet="abc", max_size=9)


@given(texts, texts)
def test_matches_exhaustive_search(first: str, second: str) -> None:
    expected = exhaustive(first, second)
    assert longest_common(first, second) == expected
    assert longest_common(second, first) == expected


@pytest.mark.parametrize(
    ("first", "second", "expected"),
    [
        pytest.param("", "", 0, id="both-empty"),
        pytest.param("", "abc", 0, id="one-empty"),
        pytest.param("abc", "abc", 3, id="identical"),
        pytest.param("abc", "def", 0, id="disjoint"),
        pytest.param("abcde", "ace", 3, id="example"),
        pytest.param("ace", "abcde", 3, id="swapped"),
        pytest.param("abcba", "abcbcba", 5, id="repeated"),
        pytest.param("ab" * 150, "ba" * 100, 200, id="long"),
    ],
)
def test_boundaries(first: str, second: str, expected: int) -> None:
    assert longest_common(first, second) == expected
