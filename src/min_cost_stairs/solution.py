from collections.abc import Iterable
from functools import reduce


def min_cost_stairs(costs: Iterable[int]) -> int:
    """Return the minimum total cost to climb past the last step.

    Start on step 0 or 1; after paying costs[i], move up one or two steps.
    Requires at least two nonnegative costs. Consume iterator inputs completely.
    """

    def climb(window: tuple[int, int], cost: int) -> tuple[int, int]:
        before, current = window
        return current, min(before, current) + cost

    steps = iter(costs)
    start = next(steps), next(steps)
    return min(reduce(climb, steps, initial=start))
