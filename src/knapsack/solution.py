from collections.abc import Iterable
from functools import reduce
from typing import NamedTuple


class Item(NamedTuple):
    weight: int
    value: int


def knapsack(items: Iterable[Item], capacity: int) -> int:
    """Return the largest total value of items whose total weight fits.

    Each item is used at most once. Requires nonnegative weights, values,
    and capacity; choosing nothing is allowed. Consume iterator inputs completely.
    """

    def add(best: list[int], item: Item) -> list[int]:
        # Pair each cell best[j] with rest = best[j - weight]. Both come from
        # the previous row, so the new item is taken at most once.
        return best[: item.weight] + [
            max(cell, rest + item.value)
            for cell, rest in zip(best[item.weight :], best, strict=False)
        ]

    return reduce(add, items, initial=[0] * (capacity + 1))[capacity]
