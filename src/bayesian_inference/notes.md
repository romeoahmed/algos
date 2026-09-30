# 贝叶斯网络推断

## 问题

已知草地湿了，下雨的概率是多少？草地也可能被洒水器打湿，因此需要把各种可能原因一起考虑。

用布尔贝叶斯网络表示这些依赖：每个节点只有 False、True 两种取值，条件概率表描述它在父节点取值已知时为 True 的概率。给定查询节点 q 和证据 E（部分节点的已知取值），返回 P(X_q = True | E)。允许空证据，也允许查询节点已在证据中。

网络按拓扑顺序存储，即父节点总在子节点之前。每个 `BooleanNode` 包含：

- `parents`：互不重复的父节点下标，均小于当前下标。
- `probabilities`：当前节点为 True 的条件概率。有 d 个父节点就有 2ᵈ 行；没有父节点时，唯一一行是先验概率。

父节点取值按**声明顺序**编码为二进制下标：False 为 0、True 为 1，第一个父节点为最高位。两个父节点的四行依次对应 `(False, False)`、`(False, True)`、`(True, False)`、`(True, True)`。例如 `parents=(1, 0)`，当节点 1 为 False、节点 0 为 True 时，查第 1 行。父节点下标不必递增。

调用者保证网络非空、下标有效、表完整且概率有限并在 [0, 1] 内。证据概率为正，浮点计算的归一化分母非零；C++ 表下标须能用 `size_t` 表示。函数保留输入。

## 思路

算法分为三步：**相乘得到联合概率，求和消去隐藏变量，归一化得到后验概率。**

对一次完整赋值 x，贝叶斯网络给出分解：

P(X = x) = ∏ᵢ P(Xᵢ = xᵢ | 父节点的取值)。

分别固定查询为 False、True，对所有符合证据的赋值求和，得到 f = P(X_q=False, E) 和 t = P(X_q=True, E)。二者之和就是 P(E)，所以答案为 t / (f + t)。若查询已在证据中，答案直接为相应的 0 或 1。

枚举时只维护已经确定的赋值前缀。访问节点 i 时，其父节点都已赋值，可以查出概率 p。若当前值为 True，本层贡献 p；否则贡献 1 − p。

证据节点和查询节点只走指定分支，其他节点分别走 False、True 两个分支并求和。每个分支把本层贡献乘以后续枚举结果；贡献为零时直接返回零，省去不可能事件的展开。所有节点处理完毕时返回 1，表示空积。这样无需先构造完整的事件表。

## 伪代码

遵循[记号约定](../notation.md)。network、q、E 在调用中只读；`BRANCH` 使用当前层的 values、p、b。

```text
BAYESIAN-INFERENCE(network, q, E)
    if q ∈ E
        return 1 if E[q] else 0
    f ← ENUMERATE([], False)
    t ← ENUMERATE([], True)
    return t / (f + t)

ENUMERATE(values, b)
    i ← |values|
    if i = |network|
        return 1
    node ← network[i]
    row ← 0
    for parent ∈ node.parents
        row ← 2 · row + (1 if values[parent] else 0)
    p ← node.probabilities[row]

    BRANCH(v)
        probability ← p if v else 1 − p
        if probability = 0
            return 0
        return probability · ENUMERATE(values ⧺ [v], b)

    if i ∈ E
        return BRANCH(E[i])
    if i = q
        return BRANCH(b)
    return BRANCH(False) + BRANCH(True)
```

## 示例

四个节点依次为阴天 C、洒水 S、下雨 R、草地湿 W；依赖关系为 C → S、C → R、S → W、R → W。

```python
from src.bayesian_inference.solution import BooleanNode, bayesian_inference

network = (
    BooleanNode((), (0.5,)),
    BooleanNode((0,), (0.5, 0.1)),
    BooleanNode((0,), (0.2, 0.8)),
    BooleanNode((1, 2), (0.0, 0.9, 0.9, 0.99)),
)
posterior = bayesian_inference(network, query=2, evidence={3: True})
# Approximately 0.707928.
```

例如 C、S、R 全为 True 且 W=True 的联合概率为 0.5 × 0.1 × 0.8 × 0.99 = 0.0396。固定 W=True 后，八种事件如下：

| C | S | R | 联合概率 |
| --- | --- | --- | --- |
| True | True | True | 0.039600 |
| True | False | True | 0.324000 |
| False | True | True | 0.049500 |
| False | False | True | 0.045000 |
| True | True | False | 0.009000 |
| True | False | False | 0.000000 |
| False | True | False | 0.180000 |
| False | False | False | 0.000000 |

前四行相加得到 t = 0.4581，后四行相加得到 f = 0.1890。因此 P(W=True) = 0.6471，P(R=True | W=True) = 0.4581 / 0.6471 ≈ 0.707928。

没有证据时，查询 R 得到先验 0.5。观察到草地湿后，下雨的概率上升，但洒水也能解释这一现象，所以后验仍小于 1。

## 正确性

若查询已被观测，由正概率证据下的条件概率定义，答案就是对应的 0 或 1。

否则固定查询值 b，对剩余节点数归纳。完整前缀下，剩余因子的空积为 1，基例成立。假设递归正确返回每个扩展前缀下的后续乘积之和：当前节点的父节点均已确定，查表和取补数给出正确的本层因子；乘以后续结果，就得到当前取值下所有合法扩展的贡献。

固定节点只有一个允许的取值；隐藏节点的两个取值互斥且穷尽，分支相加不会遗漏或重复事件。零概率分支的所有扩展贡献均为零，跳过不改变总和。因此从空前缀开始，分别得到 f、t。两者之和为 P(E)，归一化即得所求后验。

## 复杂度与实现

设 n 为节点数，h 为既未观测、又不是查询节点的数量。固定查询值后，最多枚举 2ʰ 种隐藏变量赋值；每条路径最多处理 n 个节点。每层编码父节点、复制赋值前缀均需 O(n) 时间，因此 **O(n²2ʰ)** 是两种实现的统一保守时间上界。C++ 的证据查找为 O(log(|E| + 1))，包含在此界内；Python 按字典平均常数查找计。

深度优先枚举只保留当前路径上的前缀，长度从 0 增至 n，总计 **O(n²) 额外空间、O(n) 递归深度**。输入的条件概率表可能指数大，不计入额外空间。若查询已在证据中，Python 直接返回的平均时间为 O(1)，C++ 为 O(log(|E| + 1))，额外空间均为 O(1)。

Python 用元组保存前缀，以 `reduce` 编码父节点，以 `sum(map(...))` 汇总分支。C++ 用 `std::vector<std::uint8_t>` 保存 0/1，以 `std::ranges::fold_left` 编码；扩展前先预留所需空间，再用 `append_range` 复制前缀、追加当前值。每个分支仅修改自己的副本，两种实现都不修改输入，也没有跨调用缓存。跳过零概率分支可减少确定性网络中的计算，但不改变最坏复杂度。

C++ 的输入视图、引用和闭包只在调用期间使用，前缀副本在递归返回前保持存活。Python 接口需要可重复访问的网络和父节点序列，不接受一次性迭代器。

分支乘法与求和发生在递归返回后，所以不是尾递归。Python 受递归上限限制，C++ 受可用栈空间限制；本实现适合小规模教学网络。“精确枚举”表示穷尽事件而非采样，浮点运算仍有舍入和下溢风险，不定义零概率证据下的后验。

测试以完整事件表的直接枚举作为独立参照，同时检查父节点顺序、空证据、已观测查询、确定性依赖、无关证据、重复调用及输入保留。确定性链还验证了：后代与祖先取值相同时，它们具有相同先验，而观测末端可以确定祖先。
