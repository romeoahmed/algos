# 每种字符至少出现 k 次的最长子串

## 问题

输入只含 ASCII 小写字母。求最长连续子串的长度，使其中每种字符至少出现 k 次。空串返回 0；k ≤ 1 时返回输入长度。

## 思路

统计当前片段的字符频次。若 c 出现不足 k 次，合法子串就不能包含 c。按 c 切分，递归求各段答案并取最大值；若所有字符均达标，返回整段长度。

切分后的片段不能拼接：`"aabaa"`、k = 3 时，两侧的 `"aa"` 均不合法，答案为 0；删去 b 拼成 `"aaaa"` 会破坏连续性。

## 伪代码

[记号约定](../notation.md)。

```text
LONGEST-SUBSTRING(s, k)
    if k ≤ 1
        return |s|
    if |s| < k
        return 0
    counts ← map with default value 0
    for c in s
        counts[c] ← counts[c] + 1
    for c in s
        if counts[c] < k
            return MAX([LONGEST-SUBSTRING(t, k) : t ∈ SPLIT(s, c)])
    return |s|
```

## 示例

`"ababbc"`、k = 2：a、b、c 分别出现 2、3、1 次。按 c 切分得到 `"ababb"` 和空串，前者合法，答案为 5。

## 正确性

所有字符均达到 k 次时，整个区间合法。否则，合法子串必然完整落在某个切分片段内；片段中的合法子串也属于原串，故答案等于各段答案的最大值。

每次切分至少排除一种字符，子段严格变短，递归终止。空段返回 0。

## 复杂度与实现

每条递归路径最多排除 26 种字符，同层片段互不重叠、总长至多 n，故时间 O(n)。

- Python 用 `Counter` 计数、`next` 选择低频字符，再把递归结果交给 `max`。`split` 复制子串，额外空间 O(n)。
- C++ 用 `fold_left` 构造固定的 26 项频次数组，以 `find_if`、`split`、`transform` 和 `max` 组合子问题。子视图不复制字符，额外空间 O(1)。

只有找到分隔符时才切分，交给 `max` 的范围始终非空。
