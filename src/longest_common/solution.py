from functools import reduce
from itertools import accumulate, pairwise


def longest_common(first: str, second: str) -> int:
    """Return the length of a longest common subsequence of two strings.

    The arguments are interchangeable; an empty string gives 0.
    """
    shorter, longer = sorted((first, second), key=len)

    def advance(above: list[int], char: str) -> list[int]:
        # Each cell sees its column character and the (diagonal, up) pair.
        def step(left: int, cell: tuple[str, tuple[int, int]]) -> int:
            other, (diagonal, up) = cell
            return diagonal + 1 if char == other else max(up, left)

        cells = zip(shorter, pairwise(above), strict=True)
        return list(accumulate(cells, step, initial=0))

    return reduce(advance, longer, initial=[0] * (len(shorter) + 1))[-1]
