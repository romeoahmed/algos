from collections.abc import Iterator

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.edit_distance.solution import edit_distance


def search_edits(source: str, target: str) -> int:
    """Search single edits breadth-first, never longer than the longer word."""
    alphabet = set(source + target)
    limit = max(len(source), len(target))

    def neighbors(word: str) -> Iterator[str]:
        for i in range(len(word) + 1):
            if len(word) < limit:
                yield from (word[:i] + char + word[i:] for char in alphabet)
            if i < len(word):
                yield word[:i] + word[i + 1 :]
                yield from (word[:i] + char + word[i + 1 :] for char in alphabet)

    distance, frontier, seen = 0, {source}, {source}
    while target not in frontier:
        frontier = {near for word in frontier for near in neighbors(word)} - seen
        seen |= frontier
        distance += 1
    return distance


words = st.text(alphabet="abc", max_size=5)


@given(words, words)
def test_matches_edit_search(source: str, target: str) -> None:
    expected = search_edits(source, target)
    assert edit_distance(source, target) == expected
    assert edit_distance(target, source) == expected


@pytest.mark.parametrize(
    ("source", "target", "expected"),
    [
        pytest.param("", "", 0, id="both-empty"),
        pytest.param("", "abc", 3, id="insert-all"),
        pytest.param("abc", "", 3, id="delete-all"),
        pytest.param("abc", "abc", 0, id="identical"),
        pytest.param("ab", "ba", 2, id="swap"),
        pytest.param("horse", "ros", 3, id="example"),
        pytest.param("intention", "execution", 5, id="mixed"),
        pytest.param("kitten", "sitting", 3, id="classic"),
        pytest.param("a" * 300, "b" * 200, 300, id="long"),
    ],
)
def test_boundaries(source: str, target: str, expected: int) -> None:
    assert edit_distance(source, target) == expected
