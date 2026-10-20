from collections.abc import Sequence
from itertools import accumulate


def stock_profit(prices: Sequence[int]) -> int:
    """Return the best profit from buying once and selling on a later day.

    Not trading is allowed, so empty input or falling prices return 0.
    """
    lows = accumulate(prices, min)
    return max(
        (price - low for price, low in zip(prices, lows, strict=True)), default=0
    )
