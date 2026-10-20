from collections.abc import Sequence
from itertools import accumulate


def rain_water(heights: Sequence[int]) -> int:
    """Return the units of water trapped between unit-width bars.

    Requires nonnegative heights; empty input returns 0.
    """
    left_peaks = accumulate(heights, max)
    right_peaks = list(accumulate(reversed(heights), max))
    return sum(
        min(left, right) - height
        for height, left, right in zip(
            heights, left_peaks, reversed(right_peaks), strict=True
        )
    )
