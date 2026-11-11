#include "solution.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

namespace algos {
namespace {
// SplitMix64 finalizer: every input bit affects every output bit.
constexpr auto mix(std::uint64_t value) noexcept -> std::uint64_t {
    value ^= value >> 30U;
    value *= 0xBF58'476D'1CE4'E5B9U;
    value ^= value >> 27U;
    value *= 0x94D0'49BB'1331'11EBU;
    return value ^ (value >> 31U);
}

using LetterCounts = std::array<std::size_t, 26>;

// Positions identify letters, so fold the counts in order.
struct LetterCountsHash {
    auto operator()(const LetterCounts &counts) const noexcept -> std::size_t {
        return static_cast<std::size_t>(std::ranges::fold_left(
            counts, std::uint64_t{0},
            [](std::uint64_t seed, std::size_t count) -> std::uint64_t {
                return mix(seed + count);
            }));
    }
};

using ByteCounts = std::unordered_map<char, std::size_t>;

// The map's iteration order is unspecified, so combine entries by addition,
// which ignores order: equal maps then always hash equally.
struct ByteCountsHash {
    auto operator()(const ByteCounts &counts) const noexcept -> std::size_t {
        const auto entry_hash = [](const auto &entry) -> std::uint64_t {
            const auto [byte, count] = entry;
            return mix(std::uint64_t{count} << 8U |
                       std::uint64_t{static_cast<unsigned char>(byte)});
        };
        return static_cast<std::size_t>(
            std::ranges::fold_left(counts | std::views::transform(entry_hash),
                                   std::uint64_t{0}, std::plus{}));
    }
};

auto letter_counts(std::string_view word) -> LetterCounts {
    auto counts = LetterCounts{};
    for (const auto letter : word) {
        ++counts.at(static_cast<std::size_t>(letter - 'a'));
    }
    return counts;
}

auto byte_counts(std::string_view word) -> ByteCounts {
    auto counts = ByteCounts{};
    for (const auto byte : word) {
        ++counts[byte];
    }
    return counts;
}

template <typename Signature, typename Hash>
auto group_by(std::span<const std::string> words, auto signature) -> Groups {
    auto groups = Groups{};
    // Signature -> index of its group in groups.
    auto group_of = std::unordered_map<Signature, std::size_t, Hash>{};
    for (const auto &word : words) {
        const auto [entry, inserted] =
            group_of.try_emplace(signature(word), groups.size());
        if (inserted) {
            groups.emplace_back();
        }
        groups.at(entry->second).push_back(word);
    }
    return groups;
}
} // namespace

auto group_anagrams(std::span<const std::string> words) -> Groups {
    return group_by<LetterCounts, LetterCountsHash>(words, letter_counts);
}

auto group_anagrams_hashed(std::span<const std::string> words) -> Groups {
    return group_by<ByteCounts, ByteCountsHash>(words, byte_counts);
}
} // namespace algos
