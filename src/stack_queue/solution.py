class StackQueue[T]:
    """FIFO queue on two LIFO stacks.

    Invariant: the outbox is empty only when the whole queue is, so its top is
    always the front. pop and peek require a nonempty queue.
    """

    def __init__(self) -> None:
        self._inbox: list[T] = []  # Newest on top.
        self._outbox: list[T] = []  # Oldest on top.

    def push(self, value: T) -> None:
        """Append value at the back."""
        (self._inbox if self._outbox else self._outbox).append(value)

    def pop(self) -> T:
        """Remove and return the front; amortized O(1)."""
        value = self._outbox.pop()
        if not self._outbox:
            while self._inbox:
                self._outbox.append(self._inbox.pop())
        return value

    def peek(self) -> T:
        """Return the front without changing either stack."""
        return self._outbox[-1]

    def __len__(self) -> int:
        return len(self._inbox) + len(self._outbox)
