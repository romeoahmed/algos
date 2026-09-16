from collections import Counter
from math import comb, prod
from operator import add, mul, sub

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.expressions.solution import expressions


@given(
    st.lists(st.integers(0, 99), min_size=4, max_size=4),
    st.text(alphabet="+-*", min_size=3, max_size=3),
)
def test_matches_all_five_parenthesizations(
    operands: list[int], operators: str
) -> None:
    a, b, c, d = operands
    x, y, z = operators
    operations = {"+": add, "-": sub, "*": mul}
    f, g, h = (operations[op] for op in operators)
    expected = [
        h(g(f(a, b), c), d),
        h(f(a, g(b, c)), d),
        g(f(a, b), h(c, d)),
        f(a, h(g(b, c), d)),
        f(a, g(b, h(c, d))),
    ]
    assert Counter(expressions(f"{a}{x}{b}{y}{c}{z}{d}")) == Counter(expected)


@pytest.mark.parametrize("operator", ["+", "*"])
@given(st.lists(st.integers(0, 9), min_size=1, max_size=10))
def test_preserves_catalan_multiplicity(operator: str, operands: list[int]) -> None:
    n = len(operands) - 1
    count = comb(2 * n, n) // (n + 1)
    value = sum(operands) if operator == "+" else prod(operands)
    assert expressions(operator.join(map(str, operands))) == [value] * count


@pytest.mark.parametrize(
    ("text", "expected"),
    [
        pytest.param("0", [0], id="zero"),
        pytest.param("99", [99], id="two-digit-operand"),
        pytest.param("2-1-1", [0, 2], id="subtraction-order"),
        pytest.param("2*3-4*5", [-34, -14, -10, -10, 10], id="duplicates"),
        pytest.param(
            "99*99*99*99*99*99*99", [93_206_534_790_699] * 132, id="wide-result"
        ),
        pytest.param("99*99*99*99*99*0", [0] * 42, id="wide-intermediate-product"),
    ],
)
def test_returns_independent_results(text: str, expected: list[int]) -> None:
    result = expressions(text)
    assert sorted(result) == expected
    result.clear()
    assert sorted(expressions(text)) == expected
