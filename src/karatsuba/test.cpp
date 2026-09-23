#include "solution.hpp"

#include <boost/ut.hpp>
#include <exception>
#include <iostream>
#include <ranges>
#include <rapidcheck.h>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace {
// Independent schoolbook multiplication, with a carry after each digit pair.
auto schoolbook(std::string_view x, std::string_view y) -> std::string {
    auto digits = std::vector<int>(x.size() + y.size());
    for (auto i = x.size(); i > 0; --i) {
        for (auto j = y.size(); j > 0; --j) {
            const auto k = i + j - 1;
            const auto value =
                digits.at(k) + (x.at(i - 1) - '0') * (y.at(j - 1) - '0');
            digits.at(k) = value % 10;
            digits.at(k - 1) += value / 10;
        }
    }
    auto result = std::string{};
    for (const auto digit : digits) {
        if (digit != 0 || !result.empty()) {
            result.push_back(static_cast<char>('0' + digit));
        }
    }
    return result.empty() ? "0" : result;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "matches schoolbook multiplication"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto decimal =
                rc::gen::nonEmpty(rc::gen::container<std::string>(
                    rc::gen::inRange('0', static_cast<char>('9' + 1))));
            const auto x = *decimal;
            const auto y = *decimal;
            const auto before = std::pair{x, y};
            RC_ASSERT(algos::karatsuba(x, y) == schoolbook(x, y));
            RC_ASSERT((std::pair{x, y} == before));
        }));
    };
    "zeros, carries, odd lengths, and unequal lengths"_test =
        [](const auto &example) -> void {
        const auto &[x, y, expected] = example;
        expect(algos::karatsuba(x, y) == expected);
        expect(algos::karatsuba(y, x) == expected);
    } | std::vector<std::tuple<std::string_view, std::string_view,
                               std::string_view>>{
            {"0", "0", "0"},
            {"000", "12345", "0"},
            {"0001", "0009", "9"},
            {"12", "34", "408"},
            {"1234", "5678", "7006652"},
            {"99", "99", "9801"},
            {"999", "999", "998001"},
            {"100", "101", "10100"},
            {"1", "123456789", "123456789"},
            {"10001", "10001", "100020001"}};
    "reads only the supplied views"_test = [] -> void {
        const auto x = std::string{"x001234y"};
        const auto y = std::string{"a005678b"};
        const auto before = std::pair{x, y};
        expect(algos::karatsuba(std::string_view{x}.substr(1, 6),
                                std::string_view{y}.substr(1, 6)) == "7006652");
        expect((std::pair{x, y} == before));
    };
    "long mixed digits with leading zeros"_test = [] -> void {
        const auto x = std::views::iota(0, 1001) |
                       std::views::transform([](int i) -> char {
                           return static_cast<char>('0' + (7 * i + 3) % 10);
                       }) |
                       std::ranges::to<std::string>();
        const auto y = x | std::views::reverse | std::ranges::to<std::string>();
        const auto expected = schoolbook(x, y);
        expect(algos::karatsuba(x, y) == expected);
        expect(algos::karatsuba("000" + x, "0000" + y) == expected);
    };
    "thousand digit operands and independent results"_test = [] -> void {
        auto x = std::string(1000, '9');
        auto y = "1" + std::string(999, '0') + "1";
        const auto expected = schoolbook(x, y);
        const auto result = algos::karatsuba(x, y);
        expect(result == expected);
        const auto square =
            std::string(999, '9') + "8" + std::string(999, '0') + "1";
        expect(algos::karatsuba(x, x) == square);
        expect(algos::karatsuba("99", y) == schoolbook("99", y));
        x.clear();
        y.clear();
        expect(result == expected);
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
