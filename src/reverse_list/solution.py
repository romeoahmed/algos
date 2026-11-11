from collections.abc import Iterable
from dataclasses import dataclass


@dataclass(slots=True, eq=False, repr=False)
class ListNode:
    """Singly linked node; equality is identity, so long chains never recurse."""

    value: int
    next: ListNode | None = None


def make_list(values: Iterable[int]) -> ListNode | None:
    """Link fresh nodes holding values in order behind a sentinel."""
    sentinel = ListNode(0)
    tail = sentinel
    for value in values:
        node = ListNode(value)
        tail.next = node
        tail = node
    return sentinel.next


def list_values(head: ListNode | None) -> list[int]:
    """Return the values from head to the end."""
    values: list[int] = []
    while head is not None:
        values.append(head.value)
        head = head.next
    return values


def reverse_list(head: ListNode | None) -> ListNode | None:
    """Reverse the list in place by relinking its nodes; return the new head."""
    reversed_head: ListNode | None = None
    while head is not None:
        rest = head.next  # Save the successor before overwriting the link.
        head.next = reversed_head
        reversed_head = head
        head = rest
    return reversed_head
