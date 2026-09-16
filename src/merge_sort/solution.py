from collections.abc import Sequence


def _merge(left: Sequence[int], right: Sequence[int]) -> list[int]:
    result: list[int] = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    return [*result, *left[i:], *right[j:]]


def merge_sort(values: Sequence[int]) -> list[int]:
    """Return a stably sorted copy in nondecreasing order."""
    if len(values) < 2:
        return list(values)
    middle = len(values) // 2
    return _merge(merge_sort(values[:middle]), merge_sort(values[middle:]))
