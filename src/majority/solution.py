from collections.abc import Iterable
from functools import reduce


def majority(values: Iterable[int]) -> int:
    """Return the value occurring more than half the time; it must exist.

    Consume iterator inputs completely."""

    def vote(state: tuple[int, int], value: int) -> tuple[int, int]:
        candidate, balance = state
        candidate = value if balance == 0 else candidate
        return candidate, balance + (1 if value == candidate else -1)

    return reduce(vote, values, (0, 0))[0]
