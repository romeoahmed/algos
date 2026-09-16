from collections import Counter

from hypothesis import example, given
from hypothesis import strategies as st

from src.majority.solution import majority


@st.composite
def majority_inputs(draw: st.DrawFn) -> list[int]:
    candidate = draw(st.integers())
    noise = draw(st.lists(st.integers()))
    copies = draw(st.integers(len(noise) + 1, len(noise) + 2))
    return list(draw(st.permutations(noise + [candidate] * copies)))


@given(majority_inputs())
@example([7])
@example([1, 2, 2, 2])
@example([2, 2, 1, 1, 1, 2, 2])
def test_matches_frequency_count(values: list[int]) -> None:
    before = values.copy()
    expected, _ = Counter(values).most_common(1)[0]
    assert majority(values) == expected
    assert majority(iter(values)) == expected
    assert values == before
