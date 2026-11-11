#pragma once

#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <utility>
#include <vector>

namespace algos {
// Both sets start with this many buckets or slots, keep the count a power of
// two, and map keys to positions by Fibonacci hashing; see notes.md.
inline constexpr auto hash_set_initial_capacity = std::size_t{8};

// Integer set using separate chaining. Doubles the bucket count whenever the
// size would exceed it, so chains average at most one key.
class ChainedHashSet {
  public:
    // Insert key; return whether the set changed.
    auto add(int key) -> bool;
    // Remove key; return whether the set changed.
    auto remove(int key) -> bool;
    [[nodiscard]] auto contains(int key) const -> bool;
    [[nodiscard]] auto size() const noexcept -> std::size_t;

  private:
    std::vector<std::forward_list<int>> buckets_ =
        std::vector<std::forward_list<int>>(hash_set_initial_capacity);
    std::size_t size_ = 0;

    auto rehash(std::size_t capacity) -> void;
};

// Integer set using linear probing with tombstones. Rebuilds once keys plus
// tombstones would fill more than 3/4 of the slots, so every probe sequence
// reaches an empty slot.
class ProbingHashSet {
  public:
    // Insert key, reusing the first tombstone; return whether the set changed.
    auto add(int key) -> bool;
    // Leave a tombstone at key; return whether the set changed.
    auto remove(int key) -> bool;
    [[nodiscard]] auto contains(int key) const -> bool;
    [[nodiscard]] auto size() const noexcept -> std::size_t;

  private:
    enum class State : std::uint8_t {
        empty,    // Never used: probe sequences stop here.
        occupied, // Holds key.
        deleted,  // Tombstone: probe sequences pass through.
    };
    struct Slot {
        State state = State::empty;
        int key = 0;
    };

    std::vector<Slot> slots_ = std::vector<Slot>(hash_set_initial_capacity);
    std::size_t size_ = 0;
    std::size_t deleted_ = 0;

    // Return {slot holding key, true} or {slot where key belongs, false}.
    [[nodiscard]] auto locate(int key) const -> std::pair<std::size_t, bool>;
    auto rebuild(std::size_t capacity) -> void;
};
} // namespace algos
