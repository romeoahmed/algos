from collections.abc import Sequence


def rotated_search(values: Sequence[int], target: int) -> int:
    """Return the target's index, or -1 if absent.

    Requires a rotated strictly increasing sequence; empty is allowed."""

    def search(left: int, right: int) -> int:
        if left == right:
            return -1
        middle = left + (right - left) // 2
        if values[middle] == target:
            return middle
        keep_left = (
            values[left] <= target < values[middle]
            if values[left] <= values[middle]
            else not values[middle] < target <= values[right - 1]
        )
        return search(left, middle) if keep_left else search(middle + 1, right)

    return search(0, len(values))
