from collections.abc import Sequence
from functools import reduce
from itertools import accumulate


def obstacle_paths(grid: Sequence[str]) -> int:
    """Return the number of right/down paths across a grid avoiding obstacles.

    Rows are equal-length strings in which "#" marks an obstacle.
    Requires at least one row and one column.
    """

    def advance(above: list[int], row: str) -> list[int]:
        # An obstacle cuts every path through it; other cells add up and left.
        def step(left: int, cell: tuple[str, int]) -> int:
            kind, up = cell
            return 0 if kind == "#" else left + up

        cells = zip(row, above, strict=True)
        return list(accumulate(cells, step, initial=0))[1:]

    # A virtual row above the grid sends one path into the start cell.
    start = [1] + [0] * (len(grid[0]) - 1)
    return reduce(advance, grid, initial=start)[-1]
