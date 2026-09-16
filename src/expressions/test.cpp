#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

auto main() -> int try {
    using namespace boost::ut;
    "matches all five parenthesizations"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto operand = rc::gen::inRange<std::int64_t>(0, 100);
            const auto [a, b, c, d] =
                *rc::gen::tuple(operand, operand, operand, operand);
            const auto symbol = rc::gen::element('+', '-', '*');
            const auto [x, y, z] = *rc::gen::tuple(symbol, symbol, symbol);
            const auto operation = [](char op) -> auto {
                return [op](std::int64_t left,
                            std::int64_t right) -> std::int64_t {
                    if (op == '+') {
                        return left + right;
                    }
                    if (op == '-') {
                        return left - right;
                    }
                    return left * right;
                };
            };
            const auto f = operation(x);
            const auto g = operation(y);
            const auto h = operation(z);
            auto expected = std::vector{
                h(g(f(a, b), c), d), h(f(a, g(b, c)), d), g(f(a, b), h(c, d)),
                f(a, h(g(b, c), d)), f(a, g(b, h(c, d)))};
            auto actual = algos::expressions(
                std::to_string(a) + x + std::to_string(b) + y +
                std::to_string(c) + z + std::to_string(d));
            std::ranges::sort(expected);
            std::ranges::sort(actual);
            RC_ASSERT(actual == expected);
        }));
    };
    "preserves Catalan multiplicity"_test = [](char op) -> void {
        expect(rc::check([op] -> void {
            const auto size = *rc::gen::inRange(1, 11);
            const auto operands =
                *rc::gen::container<std::vector<std::int64_t>>(
                    static_cast<std::size_t>(size),
                    rc::gen::inRange<std::int64_t>(0, 10));
            auto text = std::string{};
            auto value = std::int64_t{op == '+' ? 0 : 1};
            for (const auto operand : operands) {
                if (!text.empty()) {
                    text += op;
                }
                text += std::to_string(operand);
                value = op == '+' ? value + operand : value * operand;
            }
            auto catalan = std::size_t{1};
            for (auto n = std::size_t{1}; n < operands.size(); ++n) {
                catalan = catalan * 2 * (2 * n - 1) / (n + 1);
            }
            RC_ASSERT(algos::expressions(text) ==
                      std::vector<std::int64_t>(catalan, value));
        }));
    } | std::string_view{"+*"};
    "returns independent results"_test = [](const auto &example) -> void {
        const auto &[text, expected] = example;
        auto result = algos::expressions(text);
        std::ranges::sort(result);
        expect(that % result == expected);
        result.clear();
        auto again = algos::expressions(text);
        std::ranges::sort(again);
        expect(that % again == expected);
    } | std::vector<std::pair<std::string_view, std::vector<std::int64_t>>>{
            {"0", {0}},
            {"99", {99}},
            {"2-1-1", {0, 2}},
            {"2*3-4*5", {-34, -14, -10, -10, 10}},
            {"99*99*99*99*99*99*99",
             std::vector<std::int64_t>(132, 93'206'534'790'699)},
            {"99*99*99*99*99*0", std::vector<std::int64_t>(42, 0)}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
