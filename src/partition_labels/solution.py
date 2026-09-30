from itertools import accumulate, pairwise


def partition_labels(text: str) -> tuple[int, ...]:
    """Return partition lengths in text order, maximizing the number of parts.

    Each letter stays in one part. Requires lowercase ASCII letters; empty is allowed.
    """
    last = {char: i for i, char in enumerate(text)}
    stops = accumulate((last[char] + 1 for char in text), max, initial=0)
    cuts = (i for i, stop in enumerate(stops) if i == stop)
    return tuple(stop - start for start, stop in pairwise(cuts))
