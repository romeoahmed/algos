# algos

用伪代码、Python 3.14+ 和 C++23 学习算法。每题包含中文讲解、两种语言的实现，以及示例测试和属性测试。

实现侧重递归、函数组合和值语义：保留输入，将缓冲和缓存留在调用内部，用清晰的数据流表达算法。各题说明同时分析复制成本、递归深度和空间开销。

## 从一题开始

可以从[归并排序](src/merge_sort/notes.md)入手，先理解思路和正确性，再对照实现与测试。每题位于 `src/<algorithm>/`：

```text
notes.md       题意、思路、伪代码、示例、正确性与复杂度
solution.py    Python 实现
solution.hpp   C++ 接口
solution.cpp   C++ 实现
test.py        pytest + Hypothesis 测试
test.cpp       Boost.UT + RapidCheck 测试
```

伪代码采用 CLRS 第 4 版风格，统一使用 0 起始下标和半开区间，详见[记号约定](src/notation.md)。

## 算法索引

| 算法                                                              | 核心思路                         |
| ----------------------------------------------------------------- | -------------------------------- |
| [归并排序](src/merge_sort/notes.md)                               | 递归分治，稳定归并               |
| [第 k 大元素](src/quickselect/notes.md)                           | 按枢轴分组，递归选择             |
| [搜索旋转数组](src/rotated_search/notes.md)                       | 识别有序半区，递归二分           |
| [多数元素](src/majority/notes.md)                                 | 折叠投票状态                     |
| [表达式的所有括号组合](src/expressions/notes.md)                  | 递归枚举，缓存子式               |
| [每种字符至少出现 k 次的最长子串](src/longest_substring/notes.md) | 按低频字符切分，递归取最大值     |
| [树的广度优先与深度优先搜索](src/search_tree/notes.md)            | 用待访问序列的合并策略统一搜索   |
| [A* 最短路径](src/a_star/notes.md)                                | 启发式选择，松弛与重新展开       |
| [缺一格棋盘覆盖](src/tromino_tiling/notes.md)                     | 中心骨牌构造四个同类子问题       |
| [Karatsuba 大整数乘法](src/karatsuba/notes.md)                    | 三次递归乘法与移位组合           |
| [跳跃游戏 II](src/jump_game/notes.md)                             | 逐层扩展可达范围，求最少跳数     |
| [无重叠区间](src/erase_overlap/notes.md)                          | 选择最早结束的区间，筛选兼容区间 |
| [用最少箭引爆气球](src/burst_balloons/notes.md)                   | 在最早右端点射箭，筛选剩余区间   |
| [划分字母区间](src/partition_labels/notes.md)                     | 根据字符的最后位置确定分段边界   |
| [规则正向推理](src/forward_chaining/notes.md)                     | 按序应用规则，求事实的最小闭包   |
| [贝叶斯网络推断](src/bayesian_inference/notes.md)                 | 递归枚举，边缘化与归一化         |

## 运行

在仓库根目录执行以下命令。两种语言可独立使用，首次准备环境需要联网。

### Python

安装 [uv](https://docs.astral.sh/uv/getting-started/installation/)，按 `.python-version` 和 `uv.lock` 准备环境并运行测试：

```sh
uv sync --locked
uv run --locked pytest
```

调用算法：

```sh
uv run --locked python -c "from src.merge_sort.solution import merge_sort; print(merge_sort([3, 1, 2, 1]))"
# [1, 1, 2, 3]
```

只测一题：`uv run --locked pytest src/merge_sort/test.py`。

### C++

需要 CMake 4.4+、Git，以及支持本项目 C++23 特性的编译器和标准库。具体工具链见 [CI](.github/workflows/ci.yml)。依赖由 FetchContent 按固定提交获取。

默认预设使用 Ninja Multi-Config，需要安装 Ninja：

```sh
cmake --preset default
cmake --build --preset debug
ctest --preset debug
```

产物位于 `build/default/`。将构建和测试预设换为 `release` 可运行 Release，两种配置共用该目录。只测一题：

```sh
cmake --build --preset debug --target merge_sort
ctest --preset debug -R '^merge_sort$'
```

首次配置时，可加 `-DCMAKE_CXX_COMPILER=clang++` 或 `-DCMAKE_CXX_COMPILER=g++` 选择编译器。Clang 使用 libc++ 时，需安装对应标准库，必要时加 `-DCMAKE_CXX_FLAGS=-stdlib=libc++`。不同工具链使用独立构建目录；本机设置可写入已忽略的 `CMakeUserPresets.json`，继承 `default` 后目录按预设名区分。

Windows 可使用 Visual Studio 2026 预设。安装 C++ 桌面开发工作负载、Windows SDK 和 ASan 组件后，在 PowerShell 中执行：

```sh
cmake --preset vs2026
cmake --build --preset vs2026-debug
ctest --preset vs2026-debug
```

该预设生成 x64 工程，产物位于 `build/vs2026/`；Release 使用 `vs2026-release`。

Debug 默认启用 ASan，GCC/Clang 另启用 UBSan；配置时加 `-DALGOS_SANITIZE=OFF` 可关闭。Ninja 构建目录下会生成算法目标的 `compile_commands.json`，供编辑器和代码分析工具使用；Visual Studio 生成器不支持导出。

## 维护

新增算法时，补齐上述六个文件，并更新本页索引和根 [CMakeLists.txt](CMakeLists.txt)。实现约定、测试要求和检查命令见 [AGENTS.md](AGENTS.md)。

## 许可证

[MIT](LICENSE)
