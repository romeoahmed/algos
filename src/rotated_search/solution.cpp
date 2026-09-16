#include "solution.hpp"

#include <cstddef>
#include <iterator>
#include <span>

namespace algos {
auto rotated_search(std::span<const int> values, int target) noexcept
    -> std::ptrdiff_t {
    const auto search =
        [target](this const auto &self, std::span<const int> part,
                 std::ptrdiff_t offset) noexcept -> std::ptrdiff_t {
        if (part.empty()) {
            return -1;
        }
        const auto middle = part.size() / 2;
        const auto value = *std::next(part.begin(), std::ssize(part) / 2);
        const auto index = offset + std::ssize(part) / 2;
        if (value == target) {
            return index;
        }
        const auto keep_left = part.front() <= value
                                   ? part.front() <= target && target < value
                                   : !(value < target && target <= part.back());
        return keep_left ? self(part.first(middle), offset)
                         : self(part.subspan(middle + 1), index + 1);
    };
    return search(values, 0);
}
} // namespace algos
