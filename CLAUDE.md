@AGENTS.md

## Claude Code 补充

以上为项目约定，以下是在本仓库中工作时容易踩到的细节。

### 新增或修改题目

- 先读一道结构相近的现有题目，对齐讲解的语气、伪代码风格以及两种语言的惯用写法，再动笔。
- 先写完一题，确认 Python 测试与 C++ 单个目标通过后再写下一题，及早暴露风格和工具链问题：`cmake --build --preset <预设> --target <题目>`，`ctest --preset <预设> -R '^<题目>$'`。
- README 索引表按东亚字符宽度对齐，中文字符计两列；新增行后检查列宽与已有行一致。

### C++ 与 clang-tidy

clang-tidy 的警告视为错误，以下写法可避开常见问题：

- 访问 `span` 元素用 `front()`、`back()` 或 `subspan(i).front()`，测试中的容器用 `.at()`。`cppcoreguidelines-pro-bounds-avoid-unchecked-container-access` 会拒绝 `operator[]`。
- 测试复制 `before` 以检查输入保留时，被复制的生成值不要声明为 `const`，否则会触发 `performance-unnecessary-copy-initialization`。
- 多个 `int` 相加可能溢出时，用 `std::int64_t` 累加并在接口中说明；计数结果用 `std::uint64_t`，并在头文件注明可表示的范围。

### 检查与提交

- 本机工具链通常定义在已忽略的 `CMakeUserPresets.json` 中，先查看它，沿用已有的预设与构建目录。
- 本机通常只有一种编译器；未在 GCC 或 MSVC 上验证的改动，交付时要说明，由 CI 补充验证。
- 提交信息沿用 Conventional Commits，与 `git log` 中的风格一致，例如 `feat: …`、`refactor: …`、`chore(deps): …`。
