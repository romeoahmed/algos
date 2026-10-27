from functools import reduce


def climbing_stairs(n: int) -> int:
    """Return the number of 1- and 2-step sequences that climb exactly n stairs.

    Requires n >= 0; zero stairs have one empty sequence.
    """

    def climb(window: tuple[int, int], _: int) -> tuple[int, int]:
        before, current = window
        return current, before + current

    return reduce(climb, range(n), initial=(0, 1))[1]
