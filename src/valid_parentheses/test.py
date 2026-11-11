import re

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.valid_parentheses.solution import valid_parentheses

PAIRS = ("()", "[]", "{}")


def cancel_pairs(text: str) -> bool:
    """Delete adjacent matching pairs until none remain; valid iff empty."""
    while (shorter := re.sub(r"\(\)|\[\]|\{\}", "", text)) != text:
        text = shorter
    return not text


# Grammar: empty, or an open bracket, a balanced body, its closer, the rest.
balanced = st.recursive(
    st.just(""),
    lambda inner: st.builds(
        lambda pair, body, rest: pair[0] + body + pair[1] + rest,
        st.sampled_from(PAIRS),
        inner,
        inner,
    ),
)


@given(balanced)
def test_accepts_balanced_strings(text: str) -> None:
    assert valid_parentheses(text)


@given(st.text("()[]{}", max_size=16) | balanced)
def test_matches_cancelling_pairs(text: str) -> None:
    assert valid_parentheses(text) == cancel_pairs(text)


@pytest.mark.parametrize(
    ("text", "expected"),
    [
        pytest.param("", True, id="empty"),
        pytest.param("()[]{}", True, id="sequence"),
        pytest.param("{[()]}", True, id="nested"),
        pytest.param("(]", False, id="wrong-kind"),
        pytest.param("([)]", False, id="crossed"),
        pytest.param("((", False, id="unclosed"),
        pytest.param(")", False, id="unopened"),
        pytest.param("())", False, id="extra-closer"),
        pytest.param("(" * 100_000 + ")" * 100_000, True, id="deep"),
    ],
)
def test_boundaries(text: str, expected: bool) -> None:
    assert valid_parentheses(text) == expected
