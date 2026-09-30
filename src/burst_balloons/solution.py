from collections.abc import Sequence


def burst_balloons(intervals: Sequence[tuple[int, int]]) -> int:
    """Return the minimum number of points piercing all closed intervals.

    Requires start <= end for every closed interval; empty is allowed.
    """

    def choose(boundary: int | None, arrows: int) -> int:
        end = min(
            (end for start, end in intervals if boundary is None or start > boundary),
            default=None,
        )
        return arrows if end is None else choose(end, arrows + 1)

    return choose(None, 0)
