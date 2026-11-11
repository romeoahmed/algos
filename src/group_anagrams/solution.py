from collections import Counter
from collections.abc import Callable, Hashable, Iterable

type Signature = Callable[[str], Hashable]


def array_signature(word: str) -> tuple[int, ...]:
    """Count the 26 lowercase letters by direct addressing; requires a-z only."""
    counts = [0] * 26
    for letter in word:
        counts[ord(letter) - ord("a")] += 1
    return tuple(counts)


def hash_signature(word: str) -> frozenset[tuple[str, int]]:
    """Count characters in a hash map; works for any characters.

    A frozenset ignores the map's iteration order, so equal counts give equal
    signatures and equal hashes.
    """
    return frozenset(Counter(word).items())


def group_anagrams(
    words: Iterable[str], signature: Signature = array_signature
) -> list[list[str]]:
    """Group words whose signatures match.

    Groups follow first appearance, and each keeps input order.
    """
    groups: dict[Hashable, list[str]] = {}
    for word in words:
        groups.setdefault(signature(word), []).append(word)
    return list(groups.values())
