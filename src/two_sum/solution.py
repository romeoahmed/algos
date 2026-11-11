from collections.abc import Iterable


def two_sum(values: Iterable[int], target: int) -> tuple[int, int] | None:
    """Return indices i < j with values summing to target and the least such j.

    Stop at that j, consuming an iterator only that far; None if no pair exists.
    """
    seen: dict[int, int] = {}  # Value scanned so far -> its latest index.
    for j, value in enumerate(values):
        if (i := seen.get(target - value)) is not None:
            return i, j
        seen[value] = j
    return None
