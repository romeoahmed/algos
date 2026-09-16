from collections.abc import Callable

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.search_tree.solution import Trace, Tree, bfs, dfs


@st.composite
def ordered_trees(draw: st.DrawFn) -> tuple[Tree, dict[str, tuple[int, ...]]]:
    size = draw(st.integers(1, 30))
    parents = [draw(st.integers(0, node - 1)) for node in range(1, size)]
    children = {
        str(node): tuple(
            str(child) for child, parent in enumerate(parents, 1) if parent == node
        )
        for node in range(size)
    }
    tree = {
        node: tuple(draw(st.permutations(group))) for node, group in children.items()
    }
    paths: dict[str, tuple[int, ...]] = {"0": ()}
    for node, parent in enumerate(parents, 1):
        paths[str(node)] = (*paths[str(parent)], tree[str(parent)].index(str(node)))
    return tree, paths


@pytest.mark.parametrize("search", [bfs, dfs], ids=["bfs", "dfs"])
@given(ordered_trees(), st.integers(0, 30))
def test_matches_path_order(
    search: Callable[[Tree, str, str], Trace],
    case: tuple[Tree, dict[str, tuple[int, ...]]],
    target: int,
) -> None:
    tree, paths = case
    before = dict(tree)
    order = sorted(
        paths,
        key=lambda node: (
            (len(paths[node]), paths[node]) if search is bfs else paths[node]
        ),
    )
    goal = str(target)
    visited: set[str] = set()
    expected = []
    for node in order:
        available = (
            {child for parent in visited for child in tree[parent]} - visited - {node}
        )
        rest = tuple(item for item in order if item in available)
        expected.append((node, rest, node == goal))
        if node == goal:
            break
        visited.add(node)
    assert search(tree, "0", goal) == tuple(expected)
    assert tree == before


@pytest.mark.parametrize("search", [bfs, dfs], ids=["bfs", "dfs"])
@pytest.mark.parametrize("goal", ["leaf", "missing"])
def test_omitted_and_explicit_leaves(
    search: Callable[[Tree, str, str], Trace], goal: str
) -> None:
    expected = (("leaf", (), goal == "leaf"),)
    assert search({}, "leaf", goal) == expected
    assert search({"leaf": ()}, "leaf", goal) == expected


@pytest.mark.parametrize(
    ("search", "expected"),
    [
        (bfs, (("A", (), False), ("B", ("C",), False), ("C", ("D", "E"), True))),
        (
            dfs,
            (
                ("A", (), False),
                ("B", ("C",), False),
                ("D", ("E", "C"), False),
                ("E", ("C",), False),
                ("C", (), True),
            ),
        ),
    ],
    ids=["bfs", "dfs"],
)
def test_stops_at_goal_with_an_unexpanded_frontier(
    search: Callable[[Tree, str, str], Trace], expected: Trace
) -> None:
    tree = {"A": ("B", "C"), "B": ("D", "E"), "C": ("F",)}
    assert search(tree, "A", "C") == expected
    assert search(tree, "B", "E") == (
        ("B", (), False),
        ("D", ("E",), False),
        ("E", (), True),
    )
