# algos

用伪代码、Python 3.14+ 和 C++23 学习算法。每题包含中文讲解、正确性证明和属性测试。

侧重简洁的递归与函数式表达：保留输入，按值返回结果，将缓冲和缓存留在调用内部。允许为清晰的表达付出复制成本，复杂度与递归限制见各题说明。

## 算法

可从[归并排序](src/merge_sort/notes.md)开始，对照讲解、实现与测试。伪代码采用 CLRS 第 4 版风格，统一使用 0 起始下标和半开区间，详见[记号约定](src/notation.md)。

| 算法                                                              | 核心思路                       |
| ----------------------------------------------------------------- | ------------------------------ |
| [归并排序](src/merge_sort/notes.md)                               | 递归分治，稳定归并             |
| [第 k 大元素](src/quickselect/notes.md)                           | 按枢轴分组，递归选择           |
| [搜索旋转数组](src/rotated_search/notes.md)                       | 识别有序半区，递归二分         |
| [多数元素](src/majority/notes.md)                                 | 折叠投票状态                   |
| [表达式的所有括号组合](src/expressions/notes.md)                  | 递归枚举，缓存子式             |
| [每种字符至少出现 k 次的最长子串](src/longest_substring/notes.md) | 按低频字符切分，递归取最大值   |
| [树的广度优先与深度优先搜索](src/search_tree/notes.md)            | 用待访问序列的合并策略统一搜索 |
| [A* 最短路径](src/a_star/notes.md)                              | 启发式选择，递归松弛与重新展开 |

## 运行

以下命令在仓库根目录执行。两种语言可独立使用，首次安装依赖需要联网。

### Python

安装 [uv](https://docs.astral.sh/uv/getting-started/installation/)，按 `.python-version` 和 `uv.lock` 准备环境：

```sh
uv sync --locked
uv run --locked pytest
uv run --locked python -c "from src.merge_sort.solution import merge_sort; print(merge_sort([3, 1, 2, 1]))"
# [1, 1, 2, 3]
```

只测一题：`uv run --locked pytest src/merge_sort/test.py`。

### C++

需要 CMake 4.2+、Git，以及支持本项目 C++23 特性的编译器和标准库。[CI](.github/workflows/ci.yml) 配置了 Clang 22 + libc++、GCC 15 + libstdc++、Visual Studio 2026 的 Debug 与 Release 检查。

默认预设需要 Ninja，产物位于 `build/default/`：

```sh
cmake --preset default
cmake --build --preset debug
ctest --preset debug
```

依赖由 FetchContent 获取；将构建和测试预设换为 `release` 可运行 Release。只测一题：

```sh
cmake --build --preset debug --target merge_sort
ctest --preset debug -R '^merge_sort$'
```

首次配置可加 `-DCMAKE_CXX_COMPILER=clang++` 或 `-DCMAKE_CXX_COMPILER=g++`。Clang 使用 libc++ 时需准备对应标准库，必要时加 `-DCMAKE_CXX_FLAGS=-stdlib=libc++`。切换工具链应使用独立构建目录；可在 `CMakeUserPresets.json` 中继承 `default`，目录按预设名区分。

Windows 安装 Visual Studio 2026 的 C++ 桌面开发工作负载、Windows SDK 和 ASan 组件，在 PowerShell 中执行：

```sh
cmake --preset vs2026
cmake --build --preset vs2026-debug
ctest --preset vs2026-debug
```

该预设生成 x64 工程，产物位于 `build/vs2026/`；Release 使用 `vs2026-release`。需要编译数据库 `compile_commands.json` 时使用 Ninja，Visual Studio 生成器不支持导出。

## 开发

每题位于 `src/<algorithm>/`：

```text
notes.md       题意、思路、伪代码、证明与复杂度
solution.py    Python 实现
solution.hpp   C++ 接口
solution.cpp   C++ 实现
test.py        pytest + Hypothesis
test.cpp       Boost.UT + RapidCheck
```

算法按文档前提接收合法输入；Python 迭代器按需消耗。标准库用于遍历、筛选、映射和折叠，算法核心显式实现。测试以排序、计数、穷举等独立方法核对结果，并检查边界和输入保留。

```sh
uv run --locked ruff check .
uv run --locked ruff format --check .
uv run --locked ty check
clang-format --dry-run --Werror src/*/*.cpp src/*/*.hpp
```

C++ 将警告视为错误；Ninja 配置时加 `-DCMAKE_CXX_CLANG_TIDY=clang-tidy` 可随构建检查。Debug 默认启用 ASan，GCC/Clang 另启用 UBSan；用 `-DALGOS_SANITIZE=OFF` 关闭。

新增算法需补齐六个文件，更新本页索引和 [CMake 目标](CMakeLists.txt)。完整维护约定与检查命令见 [AGENTS.md](AGENTS.md)。

## 许可证

[MIT](LICENSE) · Copyright © 2026 Romeo Ahmed
