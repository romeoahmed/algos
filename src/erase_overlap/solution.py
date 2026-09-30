from collections.abc import Sequence


def erase_overlap(intervals: Sequence[tuple[int, int]]) -> int:
    """Return the minimum number of intervals to remove.

    Requires start < end for every interval; touching endpoints are compatible.
    """

    def choose(boundary: int | None) -> int:
        end = min(
            (end for start, end in intervals if boundary is None or start >= boundary),
            default=None,
        )
        return 0 if end is None else 1 + choose(end)

    return len(intervals) - choose(None)
