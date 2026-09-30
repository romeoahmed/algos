# 树的广度优先与深度优先搜索

## 问题

输入为有限有序树，节点标签是唯一字符串，孩子按给定顺序排列。映射中缺失的键和空孩子序列都表示叶子。`start` 是搜索起点，`goal` 可以不存在。

返回搜索轨迹，包含首次命中目标的步骤；未命中时遍历起点的整棵子树。每步为 `(node, frontier, found)`，其中 frontier 是**取出当前节点后、加入孩子前**的待访问序列。

## 思路

每步取出首节点并记录轨迹；若未命中，将其孩子与剩余节点合并，再递归搜索。BFS 与 DFS 仅合并顺序不同：

- BFS：孩子放在尾部，逐层访问。
- DFS：孩子按原顺序放在头部，先序遍历。

`merge` 接收两个序列并返回新序列。每步直接追加到本次调用的轨迹缓冲区，最后返回完整结果；输入树保持不变。

## 伪代码

[记号约定](../notation.md)。`CHILDREN(T, v)` 在键缺失时返回空序列。

```text
SEARCH(T, F, goal, merge)
    trace ← []

    VISIT(pending)
        if pending is empty
            return
        node ← pending[0]; rest ← pending[1:|pending|]
        APPEND(trace, (node, COPY(rest), node = goal))
        if node ≠ goal
            VISIT(merge(rest, CHILDREN(T, node)))

    VISIT(F)
    return trace

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

设输入树有 n 个节点，实际访问 k 个节点，S 为各步取出节点前的待访问序列长度之和。字符串操作按单位成本计，构造下一步序列和保存快照共需 O(S) 时间，追加 k 个步骤共需 O(k) 时间。Python 按字典平均常数查找计，总时间 O(S)；C++ 的映射查找另需 O(k log(n + 1)) 时间。

两种实现的额外空间均为 O(S)，包含输出、各层待访问序列和 O(k) 递归栈。宽树上 S 最坏为 O(n²)，完整轨迹本身就可能达到这个规模；单链上 S = k，Python 为线性时间和空间，C++ 另计映射查找。实际字符串长度还会影响复制和比较成本。

Python 在局部列表中追加步骤，最后转成不可变元组。C++ 用只读 `span` 借用剩余序列和孩子，仅为快照及新序列复制字符串；合并产生的临时向量存活至递归返回，视图不会悬空。返回结果独立拥有数据，调用之间不共享缓冲区，算法内不打印。

递归调用位于访问步骤的尾部，深度随访问步数而非树高增长。Python 不消除尾调用，C++ 也不保证尾调用优化；这种完整轨迹适合小规模展示，分别受递归上限和可用栈空间限制。
