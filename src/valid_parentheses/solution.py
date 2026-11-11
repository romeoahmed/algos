CLOSER_OF = {"(": ")", "[": "]", "{": "}"}


def valid_parentheses(text: str) -> bool:
    """Return whether every bracket closes in properly nested order.

    Characters other than the three opening brackets count as closers.
    """
    owed: list[str] = []  # Closers still expected, innermost on top.
    for char in text:
        if char in CLOSER_OF:
            owed.append(CLOSER_OF[char])
        elif not owed or owed.pop() != char:
            return False
    return not owed
