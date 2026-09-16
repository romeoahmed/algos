from collections.abc import Callable, Mapping
from operator import add

type Frontier = tuple[str, ...]
type Tree = Mapping[str, Frontier]
type Step = tuple[str, Frontier, bool]
type Trace = tuple[Step, ...]
type Merge = Callable[[Frontier, Frontier], Frontier]


def search(tree: Tree, frontier: Frontier, goal: str, merge: Merge) -> Trace:
    """Return a search trace using a pure frontier merge.

    Requires disjoint finite ordered subtrees with unique labels; omitted keys
    are leaves. Steps record (node, frontier before expansion, found).
    """
    if not frontier:
        return ()
    node, rest = frontier[0], frontier[1:]
    step = ((node, rest, node == goal),)
    if node == goal:
        return step
    return step + search(tree, merge(rest, tree.get(node, ())), goal, merge)


def bfs(tree: Tree, start: str, goal: str) -> Trace:
    """Return a breadth-first trace through the goal, or exhaust the subtree."""
    return search(tree, (start,), goal, add)


def dfs(tree: Tree, start: str, goal: str) -> Trace:
    """Return a preorder trace through the goal, or exhaust the subtree."""
    return search(tree, (start,), goal, lambda rest, children: children + rest)
