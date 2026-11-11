from hypothesis import given
from hypothesis import strategies as st

from src.reverse_list.solution import ListNode, list_values, make_list, reverse_list


def nodes(head: ListNode | None) -> list[ListNode]:
    """Collect the node objects from head to the end."""
    collected: list[ListNode] = []
    while head is not None:
        collected.append(head)
        head = head.next
    return collected


@given(st.lists(st.integers()))
def test_relinks_the_same_nodes_backwards(values: list[int]) -> None:
    head = make_list(values)
    assert list_values(head) == values
    before = nodes(head)
    after = nodes(reverse_list(head))
    assert [node.value for node in after] == values[::-1]
    assert all(new is old for new, old in zip(after, reversed(before), strict=True))


def test_empty_list() -> None:
    assert reverse_list(None) is None


def test_single_node() -> None:
    node = ListNode(7)
    assert reverse_list(node) is node
    assert node.next is None


def test_iterator_input_and_long_list() -> None:
    size = 200_000
    head = make_list(iter(range(size)))
    reversed_head = reverse_list(head)
    assert list_values(reversed_head) == list(range(size - 1, -1, -1))
    assert head is not None
    assert head.next is None
