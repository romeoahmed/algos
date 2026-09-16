from functools import cache
from itertools import product
from operator import add, mul, sub


def expressions(text: str) -> list[int]:
    """Return all parenthesization results, retaining duplicates in any order.

    Requires 1-20 ASCII characters: integers 0-99 joined by +, -, or *,
    without whitespace, parentheses, or unary operators.
    """
    operations = {"+": add, "-": sub, "*": mul}

    @cache
    def solve(part: str) -> tuple[int, ...]:
        if part.isdecimal():
            return (int(part),)
        return tuple(
            operations[op](left, right)
            for i, op in enumerate(part)
            if op in operations
            for left, right in product(solve(part[:i]), solve(part[i + 1 :]))
        )

    return list(solve(text))
