from collections import Counter
from itertools import combinations
from string import ascii_lowercase

import pytest
from hypothesis import example, given
from hypothesis import strategies as st

from src.longest_substring.solution import longest_substring


@given(st.text(alphabet="abcde", max_size=20), st.integers(-1, 22))
def test_matches_exhaustive_search(text: str, k: int) -> None:
    expected = max(
        (
            end - start
            for start, end in combinations(range(len(text) + 1), 2)
            if min(Counter(text[start:end]).values()) >= k
        ),
        default=0,
    )
    assert longest_substring(text, k) == expected


@pytest.mark.parametrize(
    ("text", "k", "expected"),
    [
        pytest.param("", 0, 0, id="empty"),
        pytest.param("abc", -1, 3, id="nonpositive-threshold"),
        pytest.param("aaabb", 3, 3, id="split"),
        pytest.param("ababbc", 2, 5, id="valid-prefix"),
        pytest.param("aabbcc", 3, 0, id="no-valid-substring"),
        pytest.param("aabaa", 3, 0, id="segments-must-stay-contiguous"),
    ],
)
def test_boundaries(text: str, k: int, expected: int) -> None:
    assert longest_substring(text, k) == expected


@given(st.sampled_from(ascii_lowercase), st.integers(0, 10_000))
@example("z", 10_000)
def test_single_letter_threshold(char: str, size: int) -> None:
    text = char * size
    assert longest_substring(text, size) == size
    assert longest_substring(text, size + 1) == 0
