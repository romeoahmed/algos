#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

namespace algos {
namespace {
auto trim(std::string_view value) -> std::string_view {
    const auto first = value.find_first_not_of('0');
    return first == value.npos ? std::string_view{"0"} : value.substr(first);
}

auto add(std::string_view x, std::string_view y) -> std::string {
    auto result = std::string(std::max(x.size(), y.size()) + 1, '0');
    auto i = x.size();
    auto j = y.size();
    auto carry = 0;
    for (auto &digit : result | std::views::reverse) {
        const auto sum = carry + (i > 0 ? x.at(--i) - '0' : 0) +
                         (j > 0 ? y.at(--j) - '0' : 0);
        digit = static_cast<char>('0' + sum % 10);
        carry = sum / 10;
    }
    result.erase(0, result.size() - trim(result).size());
    return result;
}

// Requires result >= y for canonical nonnegative decimal strings.
auto subtract(std::string result, std::string_view y) -> std::string {
    auto j = y.size();
    auto borrow = 0;
    for (auto &digit : result | std::views::reverse) {
        const auto difference =
            digit - '0' - borrow - (j > 0 ? y.at(--j) - '0' : 0);
        borrow = difference < 0 ? 1 : 0;
        digit = static_cast<char>('0' + difference + 10 * borrow);
    }
    result.erase(0, result.size() - trim(result).size());
    return result;
}

auto split(std::string_view value, std::size_t low_digits)
    -> std::pair<std::string_view, std::string_view> {
    const auto middle = value.size() - std::min(value.size(), low_digits);
    return {trim(value.substr(0, middle)), trim(value.substr(middle))};
}

auto shift(std::string value, std::size_t digits) -> std::string {
    if (value != "0") {
        value.append(digits, '0');
    }
    return value;
}

auto parse_small(std::string_view value) -> int {
    return std::ranges::fold_left(value, 0, [](int number, char digit) -> int {
        return 10 * number + digit - '0';
    });
}
} // namespace

auto karatsuba(std::string_view x, std::string_view y) -> std::string {
    x = trim(x);
    y = trim(y);
    if (x == "0" || y == "0") {
        return "0";
    }
    const auto digits = std::max(x.size(), y.size());
    if (digits <= 2) {
        return std::to_string(parse_small(x) * parse_small(y));
    }
    const auto half = digits / 2;
    const auto [a, b] = split(x, half);
    const auto [c, d] = split(y, half);
    const auto high = karatsuba(a, c);
    const auto low = karatsuba(b, d);
    const auto cross =
        subtract(karatsuba(add(a, b), add(c, d)), add(high, low));
    return add(add(shift(high, 2 * half), shift(cross, half)), low);
}
} // namespace algos
