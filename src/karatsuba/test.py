import pytest
from hypothesis import example, given
from hypothesis import strategies as st

from src.karatsuba.solution import karatsuba


@given(st.integers(0, (1 << 4096) - 1), st.integers(0, (1 << 4096) - 1))
@example(0, 1 << 4096)
@example((1 << 4095) - 1, (1 << 4095) - 1)
def test_matches_integer_multiplication(x: int, y: int) -> None:
    assert karatsuba(x, y) == x * y


@pytest.mark.parametrize(
    ("x", "y"),
    [
        pytest.param(0, 0, id="both-zero"),
        pytest.param(1, 1, id="both-one"),
        pytest.param(12, 34, id="two-digit-example"),
        pytest.param(1234, 5678, id="four-digit-example"),
        pytest.param(255, 255, id="small-limit"),
        pytest.param(256, 257, id="first-split"),
        pytest.param(999, 999, id="carry"),
        pytest.param(0, 10**1000, id="zero-times-large"),
        pytest.param(1, 10**1000, id="one-times-large"),
        pytest.param(99, 10**1000 - 1, id="unequal-lengths"),
        pytest.param(10**1000 - 1, 10**1000 - 1, id="long-carries"),
        pytest.param(10**1000 + 1, 10**999 + 1, id="sparse-digits"),
    ],
)
def test_boundaries_and_thousand_digit_operands(x: int, y: int) -> None:
    assert karatsuba(x, y) == x * y
    assert karatsuba(y, x) == x * y
