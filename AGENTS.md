# 维护 algos

Python 3.14+ 与 C++23 算法学习项目。环境配置见 [README](README.md)，工具链见 [CI](.github/workflows/ci.yml)。

## 修改约定

- 每题位于 `src/<algorithm>/`，包含 `notes.md`、`solution.py`、`solution.hpp`、`solution.cpp`、`test.py`、`test.cpp`。新增算法须更新 README 和根 CMake。
- 说明使用简体中文；标识符、文档字符串和代码注释使用英文，命名采用 snake_case。遵循[记号约定](src/notation.md)，保持题意、伪代码、实现和测试一致；讲解须包含思路、示例、正确性与复杂度。
- 以算法展示为先，优先简洁的递归、函数组合和值语义。保留输入，缓冲和缓存限于调用内部；局部循环或修改可用于清楚地表达单个步骤。说明复制成本、递归限制和迭代器消耗，不引入全局状态或调整递归上限。
- 假设输入满足文档前提，不添加非法输入校验或抛异常分支。标准库用于遍历、视图、筛选、映射、折叠和缓存，不以内建排序或 `nth_element` 替代算法核心；避免无助于表达的抽象和分配。
- Python 函数须有类型标注。C++ 遵循 [Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)，使用 RAII、明确所有权和安全的生命周期；实现放在 `.cpp`，使用尾置返回类型及仓库格式配置，缩进为四空格。
- 测试使用 pytest/Hypothesis 与 Boost.UT/RapidCheck。生成合法输入，以独立方法检查结果、边界和输入保留；验证可观察行为，不依赖内部实现。修复失败时不得削弱断言，算法正确性与风格分别评估。
- CMake 逻辑集中在根文件，使用目标级设置和 FetchContent；Python 依赖由 uv 管理。各工具链使用独立构建目录，机器配置放在环境变量、命令行或已忽略的 `CMakeUserPresets.json` 中。

## 检查

在仓库根目录执行，所需工具应位于 `PATH`：

```sh
uv sync --locked
uv run --locked ruff check .
uv run --locked ruff format --check .
uv run --locked ty check
uv run --locked pytest

clang-format --dry-run --Werror src/*/*.cpp src/*/*.hpp
cmake --preset default -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_CLANG_TIDY=clang-tidy
cmake --build --preset debug
ctest --preset debug
```

按修改的语言执行检查；共享配置变更须运行受影响语言的完整测试。仅改文档时核对链接、命令和技术表述。交付时说明已完成的检查与尚未验证的部分。
