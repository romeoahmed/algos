# 树的广度优先与深度优先搜索

## 问题

输入为有限有序树，节点标签是唯一字符串，孩子按给定顺序排列。映射中缺失的键和空孩子序列都表示叶子。`start` 是搜索起点，`goal` 可以不存在。

返回搜索轨迹，包含首次命中目标的步骤；未命中时遍历起点的整棵子树。每步为 `(node, frontier, found)`，其中 frontier 是**取出当前节点后、加入孩子前**的待访问序列。

## 思路

每步取出首节点并记录轨迹；若未命中，将其孩子与剩余节点合并，再递归搜索。BFS 与 DFS 仅合并顺序不同：

- BFS：孩子放在尾部，逐层访问。
- DFS：孩子按原顺序放在头部，先序遍历。

`merge` 接收两个序列并返回新序列；递归结果拼成完整轨迹，输入树保持不变。

## 伪代码

[记号约定](../notation.md)。`CHILDREN(T, v)` 在键缺失时返回空序列。

```text
SEARCH(T, F, goal, merge)
    if F is empty
        return []
    node ← F[0]; rest ← F[1:|F|]
    step ← [(node, rest, node = goal)]
    if node = goal
        return step
    next ← merge(rest, CHILDREN(T, node))
    return step ⧺ SEARCH(T, next, goal, merge)

BFS(T, start, goal)
    return SEARCH(T, [start], goal, (rest, children) ↦ rest ⧺ children)

DFS(T, start, goal)
    return SEARCH(T, [start], goal, (rest, children) ↦ children ⧺ rest)
```

## 示例

```text
A
├── B
│   ├── D
│   └── E
└── C
    └── F
```

查找 C：BFS 依次访问 A、B、C；DFS 依次访问 A、B、D、E、C。BFS 命中时的步骤是 `(C, [D, E], true)`，其中不包含 C 的孩子 F。

```python
from src.search_tree.solution import bfs, dfs

tree = {"A": ("B", "C"), "B": ("D", "E"), "C": ("F",)}
for search in (bfs, dfs):
    trace = search(tree, "A", "C")
    print(search.__name__, " → ".join(node for node, _, _ in trace))
```

## 正确性

待访问序列始终由互不相交、尚未处理的子树根组成。每步移除一个根，未命中时加入其孩子，不遗漏、不重复。有限树上每步处理一个新节点，最终命中目标或耗尽序列。

BFS 的尾部追加保证按层访问；DFS 的头部插入保证先完成前一个孩子的子树。记录发生在展开之前，命中后立即返回，符合轨迹约定。

## 复杂度与实现

设输入树有 n 个节点，字符串操作按单位成本计。待访问序列的复制、快照和轨迹拼接累计最坏需 O(n²) 时间，额外空间 O(n²)。C++ 映射查找的 O(n log n) 开销包含在此界内；实际字符串长度还会影响复制和比较成本。

Python 返回不可变元组；C++ 返回拥有字符串和快照的值，并移动递归产生的步骤，避免反复复制快照。算法内不打印。

递归深度为 O(n)，随访问步数而非树高增长。这种完整轨迹适合小规模展示；Python 受递归上限限制，C++ 受可用栈空间限制。
