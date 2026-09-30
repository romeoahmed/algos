from itertools import accumulate, chain, pairwise


def partition_labels(text: str) -> tuple[int, ...]:
    """Return lengths of the most parts that never split occurrences of a letter.

    Requires lowercase ASCII letters; empty is allowed.
    """
    last = {char: i for i, char in enumerate(text)}
    stops = accumulate((last[char] + 1 for char in text), max)
    cuts = (i for i, stop in enumerate(stops, 1) if i == stop)
    return tuple(stop - start for start, stop in pairwise(chain((0,), cuts)))
