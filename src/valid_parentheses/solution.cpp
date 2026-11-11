#include "solution.hpp"

#include <stack>
#include <string_view>

namespace algos {
auto valid_parentheses(std::string_view text) -> bool {
    auto owed = std::stack<char>{}; // Closers still expected, innermost on top.
    for (const auto bracket : text) {
        switch (bracket) {
        case '(':
            owed.push(')');
            break;
        case '[':
            owed.push(']');
            break;
        case '{':
            owed.push('}');
            break;
        default:
            if (owed.empty() || owed.top() != bracket) {
                return false;
            }
            owed.pop();
        }
    }
    return owed.empty();
}
} // namespace algos
