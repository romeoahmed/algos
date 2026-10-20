from collections.abc import Iterable
from functools import reduce


def house_robber(values: Iterable[int]) -> int:
    """Return the maximum sum of pairwise nonadjacent values.

    Choosing nothing is allowed, so empty input returns 0.
    Consume iterator inputs completely.
    """

    def visit(window: tuple[int, int], value: int) -> tuple[int, int]:
        before, best = window
        return best, max(best, before + value)

    return reduce(visit, values, (0, 0))[1]
