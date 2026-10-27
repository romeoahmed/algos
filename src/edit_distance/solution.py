from functools import reduce
from itertools import accumulate, pairwise


def edit_distance(source: str, target: str) -> int:
    """Return the fewest single-character edits turning source into target.

    Insertions, deletions, and substitutions each cost 1, so the arguments
    are interchangeable.
    """
    shorter, longer = sorted((source, target), key=len)

    def advance(above: list[int], char: str) -> list[int]:
        # Each cell sees its column character and the (diagonal, up) pair.
        def step(left: int, cell: tuple[str, tuple[int, int]]) -> int:
            other, (diagonal, up) = cell
            return diagonal if char == other else 1 + min(diagonal, up, left)

        # Against the empty prefix, each row deletes one more character.
        cells = zip(shorter, pairwise(above), strict=True)
        return list(accumulate(cells, step, initial=above[0] + 1))

    return reduce(advance, longer, initial=list(range(len(shorter) + 1)))[-1]
