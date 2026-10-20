from functools import reduce
from itertools import accumulate


def unique_paths(rows: int, columns: int) -> int:
    """Return the number of right/down paths across a rows-by-columns grid.

    Requires positive dimensions.
    """

    def advance(row: list[int], _: int) -> list[int]:
        return list(accumulate(row))

    shorter, longer = min(rows, columns), max(rows, columns)
    return reduce(advance, range(longer - 1), [1] * shorter)[-1]
