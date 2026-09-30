# 维护 algos

Python 3.14+ 与 C++23 算法学习项目。目录结构和运行方式见 [README](README.md)，工具链与检查流程见 [CI](.github/workflows/ci.yml)。

## 内容与实现

- 每题位于 `src/<algorithm>/`，包含 `notes.md`、`solution.py`、`solution.hpp`、`solution.cpp`、`test.py`、`test.cpp`。新增题目时同步更新 README 索引和根 CMake 目标列表。
- 讲解使用简体中文，覆盖题意、思路、伪代码、示例、正确性与复杂度。遵循[记号约定](src/notation.md)，保持讲解、两种语言的实现和测试语义一致。
- 标识符、文档字符串和代码注释使用英文。函数、变量和命名空间采用 `snake_case`，类型及类型别名采用 `PascalCase`。
- 以算法展示为先，优先纯函数、递归、函数组合和值语义。保留输入，缓冲和缓存限于调用内部；局部循环或修改可用于清楚地表达单个步骤。避免无助于表达的抽象与分配。
- 标准库用于遍历、视图、筛选、映射、折叠和缓存；C++ 的遍历与数据变换优先使用 `std::ranges`。不以内建排序或 `nth_element` 替代算法核心。
- 输入由调用者保证满足题目前提，不增加非法输入校验或抛异常分支。说明复制成本、递归限制和迭代器消耗；不引入可变全局状态，不调整递归上限。
- Python 函数须有类型标注。C++ 实现放在 `.cpp`，使用尾置返回类型，遵循 [Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)，使用 RAII，明确所有权，保证引用和视图的生命周期安全。格式遵循仓库配置，缩进为四空格。

## 测试

使用 pytest/Hypothesis 与 Boost.UT/RapidCheck。生成满足题目前提的输入，以独立方法验证结果、边界和输入保留；测试中可使用排序、穷举等参考方法。

断言针对可观察行为，不依赖内部实现。修复失败时不得削弱断言，算法正确性与代码风格分别审查。

## 构建与依赖

- CMake 以 4.4 为基准，逻辑集中在根文件；优先目标属性和 `target_*` 命令，依赖通过 FetchContent 固定到提交。必要的第三方适配限于对应依赖目标。
- Python 依赖通过 uv 管理，依赖声明与 `uv.lock` 保持一致。
- 不同工具链使用独立构建目录。机器配置放在环境变量、命令行或已忽略的 `CMakeUserPresets.json` 中。

## 检查

在仓库根目录执行，所需工具应位于 `PATH`。按修改的语言运行检查；共享配置变更须运行受影响语言的完整测试。仅改文档时核对链接、命令和技术表述。

Python：

```sh
uv sync --locked
uv run --locked ruff check .
uv run --locked ruff format --check .
uv run --locked ty check
uv run --locked pytest
```

C++ 检查以 Clang/Ninja 为例；已有构建目录沿用其工具链和对应预设：

```sh
clang-format --dry-run --Werror src/*/*.cpp src/*/*.hpp
cmake --preset default -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_CLANG_TIDY=clang-tidy
cmake --build --preset debug
ctest --preset debug
cmake --build --preset release
ctest --preset release
```

交付时说明修改内容、完成的检查和尚未验证的部分。
