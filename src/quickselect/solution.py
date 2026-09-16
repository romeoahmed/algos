from collections.abc import Sequence


def _partition(values: Sequence[int], pivot: int) -> tuple[list[int], list[int]]:
    return (
        [value for value in values if value > pivot],
        [value for value in values if value < pivot],
    )


def quickselect(values: Sequence[int], k: int) -> int:
    """Return the k-th largest value, counting duplicates.

    Requires 1 <= k <= len(values)."""
    pivot = values[len(values) // 2]
    greater, lower = _partition(values, pivot)
    upper_count = len(values) - len(lower)
    if k <= len(greater):
        return quickselect(greater, k)
    if k <= upper_count:
        return pivot
    return quickselect(lower, k - upper_count)
