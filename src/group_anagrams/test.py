import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.group_anagrams.solution import (
    array_signature,
    group_anagrams,
    hash_signature,
)


def group_by_sorting(words: list[str]) -> list[list[str]]:
    """Put each word in the first group whose first word sorts the same."""
    groups: list[list[str]] = []
    for word in words:
        group = next((g for g in groups if sorted(g[0]) == sorted(word)), None)
        if group is None:
            groups.append([word])
        else:
            group.append(word)
    return groups


@given(st.lists(st.text("abc", max_size=4)))
def test_both_signatures_match_sorting(words: list[str]) -> None:
    before = words.copy()
    expected = group_by_sorting(words)
    assert group_anagrams(words) == expected
    assert group_anagrams(iter(words), hash_signature) == expected
    assert words == before


@given(st.lists(st.text(max_size=4) | st.text("aé😀", max_size=4)))
def test_hash_signature_handles_any_characters(words: list[str]) -> None:
    assert group_anagrams(words, hash_signature) == group_by_sorting(words)


@given(st.data())
def test_signatures_ignore_order(data: st.DataObject) -> None:
    word = data.draw(st.text("abcdefghijklmnopqrstuvwxyz"))
    shuffled = "".join(data.draw(st.permutations(word)))
    assert array_signature(shuffled) == array_signature(word)
    assert hash_signature(shuffled) == hash_signature(word)


@pytest.mark.parametrize(
    ("words", "expected"),
    [
        pytest.param(
            ["eat", "tea", "tan", "ate", "nat", "bat"],
            [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]],
            id="example",
        ),
        pytest.param([""], [[""]], id="empty-word"),
        pytest.param([], [], id="no-words"),
        pytest.param(["ab", "ba", "ab"], [["ab", "ba", "ab"]], id="repeats"),
        pytest.param(["aab", "abb"], [["aab"], ["abb"]], id="same-letters"),
    ],
)
def test_boundaries(words: list[str], expected: list[list[str]]) -> None:
    assert group_anagrams(words) == expected
    assert group_anagrams(words, hash_signature) == expected
