from collections.abc import Sequence


def longest_increasing(values: Sequence[int]) -> int:
    """Return the length of a longest strictly increasing subsequence.

    Empty input returns 0.
    """
    endings: list[int] = []
    for value in values:
        # endings is shorter than values, so zip pairs only earlier positions.
        predecessors = (
            length
            for previous, length in zip(values, endings, strict=False)
            if previous < value
        )
        endings.append(1 + max(predecessors, default=0))
    return max(endings, default=0)
