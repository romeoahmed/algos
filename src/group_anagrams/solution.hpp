#pragma once

#include <span>
#include <string>
#include <vector>

namespace algos {
using Groups = std::vector<std::vector<std::string>>;

// Both functions copy the words into groups ordered by first appearance, each
// keeping input order.

// Group lowercase words by an array of 26 letter counts. Requires a-z only.
[[nodiscard]] auto group_anagrams(std::span<const std::string> words) -> Groups;

// Group words over any bytes by a hash map of byte counts.
[[nodiscard]] auto group_anagrams_hashed(std::span<const std::string> words)
    -> Groups;
} // namespace algos
