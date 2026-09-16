# A* 最短路径

## 问题

在城市道路图中，从 `start` 出发到达 `goal`，求总路程最短的一条路径。输入为邻接表 `roads` 和各城市到终点的距离估计 `heuristic`；返回值 `Route` 包含城市序列 `path` 与总路程 `cost`。

起终点相同时返回单节点路径，代价为 0；存在多条最短路径时返回任意一条。不可达时，Python 返回 `None`，C++ 返回 `std::nullopt`。

图有限，顶点标签为字符串，邻接表每项为 `(neighbor, distance)`。边权为非负整数，允许自环、零权边和重边；缺失的键表示没有出边。有向边按给定方向使用，无向道路需列出两个方向。

## 思路

A* 同时考虑已经走了多远，以及估计还要走多远：

| 记号 | 含义 |
| --- | --- |
| g(v) | 当前路径从起点到 v 的实际路程 |
| h(v) | 从 v 到终点的估计路程 |
| f(v) = g(v) + h(v) | 沿当前路径继续前往终点的估计总路程 |

每次优先展开 f 最小的城市。h 全为 0 时，就只比较已经付出的代价，退化为一致代价搜索。

为保证最优，估计须满足**可采纳性**：`0 ≤ h(v) ≤ h*(v)`，即非负且不高估真实剩余距离 h*(v)，因此 h(goal) = 0。启发式须覆盖所有顶点及起终点；无法到达终点的顶点可使用任意有限非负估计。调用者保证这些前提。

搜索状态由两个映射组成：`frontier` 保存每个待展开城市的候选路径，`best` 保存每个已发现城市的最小 g。

一次递归完成以下步骤：

1. frontier 为空则不可达；否则选出 f 最小的城市，平局时按城市名选择。
2. 若选中终点，返回路径；否则取出该城市，检查它的每条出边。
3. 若 `g + distance` 比邻居的已知代价更小，就更新 best 与候选路径。这个步骤称为**松弛**。
4. 用更新后的状态递归搜索。

同一城市在 frontier 中至多保留一条路径，改进时直接替换；若它已展开，更小的 g 会将它重新加入。相等代价不更新，避免零权环反复入队。因此，本实现允许启发式不满足更强的**一致性**条件 `h(u) ≤ distance(u,v) + h(v)`。

终点必须在**被选中时**返回。刚发现一条通往终点的路时，其他候选仍可能带来更短的路线。

## 伪代码

[记号约定](../notation.md)。映射的 `COPY` 产生独立映射；路径按值拼接。`ARGMIN` 返回使给定键最小的城市，`REMOVE` 删除映射项，`NONE` 表示不可达。

```text
A-STAR(roads, start, goal, h)
    SEARCH(frontier, best)
        if frontier is empty
            return NONE
        u ← ARGMIN(frontier.keys, v ↦ (frontier[v].cost + h[v], v))
        (path, g) ← frontier[u]
        if u = goal
            return (path, g)
        pending ← COPY(frontier); REMOVE(pending, u)
        costs ← COPY(best)
        for (v, distance) in roads.get(u, [])
            candidate ← g + distance
            if v ∉ costs or candidate < costs[v]
                costs[v] ← candidate
                pending[v] ← (path ⧺ [v], candidate)
        return SEARCH(pending, costs)

    return SEARCH({start: ([start], 0)}, {start: 0})
```

## 示例

题目给出的 Romania 道路从 Arad 到 Bucharest 的最短路径为：

```text
Arad → Sibiu → Rimnicu → Pitesti → Bucharest
       140       80        97        101
总代价：140 + 80 + 97 + 101 = 418
```

依次被选中的城市如下，最后一步直接返回：

| 城市 | g | h | f |
| --- | ---: | ---: | ---: |
| Arad | 0 | 366 | 366 |
| Sibiu | 140 | 253 | 393 |
| Rimnicu | 220 | 193 | 413 |
| Fagaras | 239 | 176 | 415 |
| Pitesti | 317 | 100 | 417 |
| Bucharest | 418 | 0 | 418 |

展开 Fagaras 时，先找到一条经过它到 Bucharest 的路径，总代价为 239 + 211 = 450。但 Pitesti 的 f 只有 417，会先被选中，并将终点的代价改进为 317 + 101 = 418。这也解释了为什么不能在第一次发现终点时返回。

<details>
<summary>完整 Romania 示例：20 个城市，可在仓库根目录运行</summary>

两种语言的测试均使用这份完整地图。

```python
from src.a_star.solution import a_star

roads = {
    "Arad": [("Zerind", 75), ("Sibiu", 140), ("Timisoara", 118)],
    "Zerind": [("Arad", 75), ("Oradea", 71)],
    "Oradea": [("Zerind", 71), ("Sibiu", 151)],
    "Sibiu": [("Arad", 140), ("Oradea", 151), ("Fagaras", 99), ("Rimnicu", 80)],
    "Timisoara": [("Arad", 118), ("Lugoj", 111)],
    "Lugoj": [("Timisoara", 111), ("Mehadia", 70)],
    "Mehadia": [("Lugoj", 70), ("Drobeta", 75)],
    "Drobeta": [("Mehadia", 75), ("Craiova", 120)],
    "Craiova": [("Drobeta", 120), ("Rimnicu", 146), ("Pitesti", 138)],
    "Rimnicu": [("Sibiu", 80), ("Craiova", 146), ("Pitesti", 97)],
    "Fagaras": [("Sibiu", 99), ("Bucharest", 211)],
    "Pitesti": [("Rimnicu", 97), ("Craiova", 138), ("Bucharest", 101)],
    "Bucharest": [
        ("Fagaras", 211),
        ("Pitesti", 101),
        ("Giurgiu", 90),
        ("Urziceni", 85),
    ],
    "Giurgiu": [("Bucharest", 90)],
    "Urziceni": [("Bucharest", 85), ("Hirsova", 98), ("Vaslui", 142)],
    "Hirsova": [("Urziceni", 98), ("Eforie", 86)],
    "Eforie": [("Hirsova", 86)],
    "Vaslui": [("Urziceni", 142), ("Iasi", 92)],
    "Iasi": [("Vaslui", 92), ("Neamt", 87)],
    "Neamt": [("Iasi", 87)],
}
h = {
    "Arad": 366,
    "Zerind": 374,
    "Oradea": 380,
    "Sibiu": 253,
    "Timisoara": 329,
    "Lugoj": 244,
    "Mehadia": 241,
    "Drobeta": 242,
    "Craiova": 160,
    "Rimnicu": 193,
    "Fagaras": 176,
    "Pitesti": 100,
    "Bucharest": 0,
    "Giurgiu": 77,
    "Urziceni": 80,
    "Hirsova": 151,
    "Eforie": 161,
    "Vaslui": 199,
    "Iasi": 226,
    "Neamt": 234,
}

route = a_star(roads, "Arad", "Bucharest", h)
if route is not None:
    print(" → ".join(route.path))
    print(route.cost)
# Arad → Sibiu → Rimnicu → Pitesti → Bucharest
# 418
```

</details>

## 正确性

**路径与代价不变量。** frontier 中的每条路径都由实际道路连接，其 cost 等于道路代价之和，并等于 best 中对应城市的值。初始路径成立；松弛只在原路径末尾追加一条实际道路，并同步更新代价，故保持不变量。更优路径替换旧候选，不会留下过期项。

**终止。** best 只会严格下降。若候选路径试图回到自身已包含的城市，非负边权使新代价不小于先前到达该城市的前缀代价，也就不能改进 best。因此入队路径均无重复城市。有限图的简单路径只有有限条，严格改进和展开也只有有限次。

**最优性。** 设最短路程为 C*。只要目标尚未被选中，frontier 中就存在某个最短路径前缀：沿最短路径找第一个尚未以最优 g 展开的顶点 v，它要么是初始化时加入的起点，要么已由前驱的松弛加入。即使 v 曾以更大代价展开，也会重新加入。可采纳性保证这个候选的 f ≤ C*。

因此，代价大于 C* 的目标不可能先被选中；目标被选中时 h = 0，f = g，返回的可行路径必有 g = C*。如果 frontier 耗尽仍未命中，则不存在可达目标，否则最短路上的待处理前缀不可能全部消失。

## 复杂度与实现

本实现用线性选择和完整路径展示 A*。主要成本来自扫描候选、复制路径以及保留递归状态。

设 V、E 为顶点数和边数，K 为实际选择次数（含重新展开），M 为累计检查的出边数，L 为保存的最长路径的顶点数，L ≤ V。按单位成本计算整数和标签操作，令 B = log(V + 1)，得到以下上界：

| 实现 | 时间 | 额外空间 |
| --- | --- | --- |
| Python | O(KV + ML) | O(KVL)，保守上界 |
| C++ | O(KVB + M(B + L)) | O((V + K)L) |

Python 每步复制两个状态映射，成功松弛时复制路径元组；旧映射随递归层保留，元组可共享。字典查找按平均常数成本计。

C++ 用 `min_element` 配合优先级投影选择，用 `map::extract` 取得选中路径的所有权，避免取出时复制。状态映射按值接收、更新后移动给下一层；成功松弛仍需复制路径，各层保留选中的路径。`std::map` 的查找和更新带来对数成本。

启发式一致时，每个城市至多展开一次，K ≤ V、M ≤ E；仅可采纳时可能重新展开，不能直接套用基于堆和一致启发式的 O((V + E) log V) 时间界。

两种实现都保留道路和启发式输入，返回独立结果，没有跨调用缓存或算法内打印。Python 只修改新复制的状态，返回不可变元组；C++ 修改独占状态，命中时移动结果。

递归深度为 O(K)，取决于选择次数，而非最终路径长度。Python 不消除尾调用，C++ 也不保证尾调用优化，分别受递归上限和可用栈空间限制，因此这种写法适合小规模展示。Python 邻接表必须是可重复遍历的序列，重新展开时需要再次读取，不能使用一次性迭代器。

C++ 使用 `std::int64_t`，要求所有计算到的 g + distance 和 g + h 均可表示。Python 使用任意精度整数，大整数和较长标签的实际运算、复制成本需另计。

测试以 Floyd–Warshall 独立求出随机图的最短距离，再据此生成可采纳启发式；核对最优代价、路径边、输入保留，以及零启发式、不一致启发式、不可达、零权环、重边、平局和大整数代价。

参考：[UC Berkeley CS 188：Informed Search](https://inst.eecs.berkeley.edu/~cs188/textbook/search/informed.html)。
