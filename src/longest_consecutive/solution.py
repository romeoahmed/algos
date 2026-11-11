from collections.abc import Iterable
from itertools import count, takewhile


def longest_consecutive(values: Iterable[int]) -> int:
    """Return the length of the longest run of consecutive distinct values.

    Count upward only from run starts, so each value is walked over once.
    Consume iterator inputs completely.
    """
    present = set(values)

    def run_from(start: int) -> int:
        return sum(1 for _ in takewhile(present.__contains__, count(start)))

    return max(
        (run_from(value) for value in present if value - 1 not in present),
        default=0,
    )
