from collections import deque

from hypothesis import given
from hypothesis import strategies as st

from src.stack_queue.solution import StackQueue


@given(st.lists(st.integers() | st.none()))
def test_matches_deque(operations: list[int | None]) -> None:
    """Integers push and None pops; collections.deque is the reference."""
    queue = StackQueue[int]()
    model: deque[int] = deque()
    for operation in operations:
        if operation is not None:
            queue.push(operation)
            model.append(operation)
        elif model:
            assert queue.pop() == model.popleft()
        assert len(queue) == len(model)
        if model:
            assert queue.peek() == model[0]


def test_example_sequence() -> None:
    queue = StackQueue[int]()
    for value in (1, 2, 3):
        queue.push(value)
    popped = [queue.pop()]
    queue.push(4)
    popped += [queue.pop() for _ in range(3)]
    assert popped == [1, 2, 3, 4]
    assert len(queue) == 0


def test_generic_values_in_bulk() -> None:
    queue = StackQueue[str]()
    words = [str(index) for index in range(100_000)]
    for word in words:
        queue.push(word)
    assert queue.peek() == "0"
    assert [queue.pop() for _ in words] == words
