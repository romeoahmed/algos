from itertools import product
from math import comb

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.climbing_stairs.solution import climbing_stairs


def count_by_two_steps(n: int) -> int:
    """Choose which of the n - k steps are the k two-stair steps."""
    return sum(comb(n - twos, twos) for twos in range(n // 2 + 1))


def enumerate_walks(n: int) -> int:
    """Count step sequences of every length whose total is exactly n."""
    return sum(
        sum(steps) == n
        for length in range(n + 1)
        for steps in product((1, 2), repeat=length)
    )


@given(st.integers(0, 400))
def test_matches_combinatorial_count(n: int) -> None:
    assert climbing_stairs(n) == count_by_two_steps(n)


@pytest.mark.parametrize("n", range(13))
def test_matches_enumeration(n: int) -> None:
    assert climbing_stairs(n) == enumerate_walks(n)


@pytest.mark.parametrize(
    ("n", "expected"),
    [
        pytest.param(0, 1, id="no-stairs"),
        pytest.param(1, 1, id="one-stair"),
        pytest.param(2, 2, id="two-stairs"),
        pytest.param(4, 5, id="example"),
        pytest.param(5, 8, id="recursion-tree"),
        pytest.param(92, 12200160415121876738, id="uint64-limit"),
    ],
)
def test_boundaries(n: int, expected: int) -> None:
    assert climbing_stairs(n) == expected
