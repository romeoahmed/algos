import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.stock_profit.solution import stock_profit


def brute_force(prices: list[int]) -> int:
    """Try every buy day with every later sell day, or no trade."""
    trades = (
        prices[sell] - prices[buy]
        for buy in range(len(prices))
        for sell in range(buy + 1, len(prices))
    )
    return max((0, *trades))


@given(st.lists(st.integers(), max_size=30))
def test_matches_brute_force(prices: list[int]) -> None:
    before = prices.copy()
    expected = brute_force(prices)
    assert stock_profit(prices) == expected
    assert stock_profit(tuple(prices)) == expected
    assert prices == before


@pytest.mark.parametrize(
    ("prices", "expected"),
    [
        pytest.param([], 0, id="empty"),
        pytest.param([5], 0, id="singleton"),
        pytest.param([7, 1, 5, 3, 6, 4], 5, id="example"),
        pytest.param([7, 6, 4, 3, 1], 0, id="falling"),
        pytest.param([2, 4, 1], 2, id="low-after-peak"),
        pytest.param([3, 8, 1, 6], 5, id="earlier-low-wins"),
    ],
)
def test_boundaries(prices: list[int], expected: int) -> None:
    assert stock_profit(prices) == expected
