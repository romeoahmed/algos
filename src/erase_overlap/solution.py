from collections.abc import Sequence


def erase_overlap(intervals: Sequence[tuple[int, int]]) -> int:
    """Return the minimum number of intervals to remove.

    Requires start < end for every interval; touching endpoints are compatible.
    """

    def choose(boundary: int | None, kept: int) -> int:
        end = min(
            (end for start, end in intervals if boundary is None or start >= boundary),
            default=None,
        )
        return kept if end is None else choose(end, kept + 1)

    return len(intervals) - choose(None, 0)
