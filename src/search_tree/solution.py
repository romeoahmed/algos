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
    are leaves. Each step records the node, frontier after removal and before
    expansion, and whether the goal was found.
    """
    trace: list[Step] = []

    def visit(pending: Frontier) -> None:
        if not pending:
            return
        node, rest = pending[0], pending[1:]
        trace.append((node, rest, node == goal))
        if node != goal:
            visit(merge(rest, tree.get(node, ())))

    visit(frontier)
    return tuple(trace)


def bfs(tree: Tree, start: str, goal: str) -> Trace:
    """Return a breadth-first trace through the goal, or exhaust the subtree."""
    return search(tree, (start,), goal, add)


def dfs(tree: Tree, start: str, goal: str) -> Trace:
    """Return a preorder trace through the goal, or exhaust the subtree."""
    return search(tree, (start,), goal, lambda rest, children: children + rest)
