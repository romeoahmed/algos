from collections import Counter


def longest_substring(text: str, k: int) -> int:
    """Return the longest substring length whose letters each occur at least k times.

    Requires lowercase ASCII letters; empty is allowed. For k <= 1, return len(text).
    """
    if k <= 1:
        return len(text)
    if len(text) < k:
        return 0
    separator = next((char for char, count in Counter(text).items() if count < k), None)
    if separator is None:
        return len(text)
    return max(longest_substring(part, k) for part in text.split(separator))
