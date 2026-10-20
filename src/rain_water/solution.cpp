#include "solution.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <ranges>
#include <span>
#include <vector>

namespace algos {
auto rain_water(std::span<const int> heights) -> std::int64_t {
    const auto taller = [](int first, int second) -> int {
        return std::max(first, second);
    };
    // Prefix and suffix maxima, each counting the bar itself.
    auto left_peaks = std::vector<int>(heights.size());
    std::inclusive_scan(heights.begin(), heights.end(), left_peaks.begin(),
                        taller);
    auto right_peaks = std::vector<int>(heights.size());
    std::inclusive_scan(heights.rbegin(), heights.rend(), right_peaks.begin(),
                        taller);
    const auto water = std::views::zip_transform(
        [](int height, int left, int right) -> std::int64_t {
            return std::min(left, right) - height;
        },
        heights, left_peaks, right_peaks | std::views::reverse);
    return std::ranges::fold_left(water, std::int64_t{0}, std::plus{});
}
} // namespace algos
