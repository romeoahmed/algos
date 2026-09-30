#include "solution.hpp"

#include <charconv>
#include <cstdint>
#include <functional>
#include <memory>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace algos {
namespace {
auto operation(char op) noexcept -> auto {
    return [op](std::int64_t a, std::int64_t b) noexcept -> std::int64_t {
        if (op == '+') {
            return a + b;
        }
        if (op == '-') {
            return a - b;
        }
        return a * b;
    };
}
} // namespace

auto expressions(std::string_view text) -> std::vector<std::int64_t> {
    auto memo =
        std::unordered_map<std::string_view, std::vector<std::int64_t>>{};
    const auto solve =
        [&memo](this const auto &self,
                std::string_view part) -> const std::vector<std::int64_t> & {
        if (const auto found = memo.find(part); found != memo.end()) {
            return found->second;
        }
        auto result = std::vector<std::int64_t>{};
        for (auto i = part.find_first_of("+-*"); i != part.npos;
             i = part.find_first_of("+-*", i + 1)) {
            const auto combine = operation(part.at(i));
            const auto &left = self(part.substr(0, i));
            const auto &right = self(part.substr(i + 1));
            for (const auto value : left) {
                const auto with_left = std::bind_front(combine, value);
                result.append_range(right | std::views::transform(with_left));
            }
        }
        if (result.empty()) {
            auto value = std::int64_t{0};
            std::from_chars(std::to_address(part.begin()),
                            std::to_address(part.end()), value);
            result.push_back(value);
        }
        // Keys borrow text; rehashing keeps references to cached results valid.
        return memo.emplace(part, std::move(result)).first->second;
    };
    return solve(text);
}
} // namespace algos
