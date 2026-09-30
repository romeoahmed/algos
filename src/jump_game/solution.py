from collections.abc import Sequence


def jump_game(values: Sequence[int]) -> int:
    """Return the minimum jumps from the first index to the last.

    Requires a nonempty sequence of nonnegative maximum steps with a reachable end.
    """

    def expand(first: int, stop: int, jumps: int) -> int:
        if stop == len(values):
            return jumps
        following = 1 + max(i + values[i] for i in range(first, stop))
        return expand(stop, min(len(values), following), jumps + 1)

    return expand(0, 1, 0)
