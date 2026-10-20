from collections.abc import Iterable
from itertools import accumulate


def max_subarray(values: Iterable[int]) -> int:
    """Return the maximum sum of a nonempty contiguous subarray.

    Requires nonempty input. Consume iterator inputs completely.
    """

    def extend(ending: int, value: int) -> int:
        return max(value, ending + value)

    return max(accumulate(values, extend))
