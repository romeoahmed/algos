import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.partition_labels.solution import partition_labels


def exhaustive_partition(text: str) -> tuple[int, ...]:
    if not text:
        return ()
    best: tuple[int, ...] = ()
    for mask in range(1 << (len(text) - 1)):
        ends = [i + 1 for i in range(len(text) - 1) if mask & (1 << i)] + [len(text)]
        start = 0
        used: set[str] = set()
        lengths = []
        for end in ends:
            letters = set(text[start:end])
            if used & letters:
                break
            used |= letters
            lengths.append(end - start)
            start = end
        else:
            if len(lengths) > len(best):
                best = tuple(lengths)
    return best


@given(st.text(alphabet="abcd", max_size=10))
def test_matches_all_cut_patterns(text: str) -> None:
    assert partition_labels(text) == exhaustive_partition(text)


@pytest.mark.parametrize(
    ("text", "expected"),
    [
        pytest.param("", (), id="empty"),
        pytest.param("a", (1,), id="singleton"),
        pytest.param("ababcbacadefegdehijhklij", (9, 7, 8), id="example"),
        pytest.param("eccbbbbdec", (10,), id="chained-boundaries"),
        pytest.param("abac", (3, 1), id="late-repeat"),
        pytest.param("abcdefghijklmnopqrstuvwxyz", (1,) * 26, id="distinct-letters"),
        pytest.param("a" * 2000, (2000,), id="long-single-part"),
    ],
)
def test_boundaries(text: str, expected: tuple[int, ...]) -> None:
    assert partition_labels(text) == expected


def test_independent_results() -> None:
    first = partition_labels("abac")
    assert partition_labels("abc") == (1, 1, 1)
    assert first == (3, 1)
