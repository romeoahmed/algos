#include "solution.hpp"

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

namespace algos {
namespace {
// Map key to a bucket of table by Fibonacci hashing: multiply by 2^64 / golden
// ratio and keep the top log2(size) bits. Requires a power-of-two size.
auto home(int key, const auto &table) noexcept -> std::size_t {
    constexpr auto golden = std::uint64_t{0x9E37'79B9'7F4A'7C15};
    const auto mixed =
        static_cast<std::uint64_t>(std::hash<int>{}(key)) * golden;
    const auto bits = static_cast<unsigned>(std::countr_zero(table.size()));
    return static_cast<std::size_t>(mixed >> (64U - bits));
}
} // namespace

auto ChainedHashSet::add(int key) -> bool {
    auto &chain = buckets_.at(home(key, buckets_));
    if (std::ranges::contains(chain, key)) {
        return false;
    }
    chain.push_front(key);
    ++size_;
    if (size_ > buckets_.size()) {
        rehash(2 * buckets_.size());
    }
    return true;
}

auto ChainedHashSet::remove(int key) -> bool {
    if (buckets_.at(home(key, buckets_)).remove(key) == 0) {
        return false;
    }
    --size_;
    return true;
}

auto ChainedHashSet::contains(int key) const -> bool {
    return std::ranges::contains(buckets_.at(home(key, buckets_)), key);
}

auto ChainedHashSet::size() const noexcept -> std::size_t { return size_; }

auto ChainedHashSet::rehash(std::size_t capacity) -> void {
    auto next = std::vector<std::forward_list<int>>(capacity);
    for (auto &chain : buckets_) {
        while (!chain.empty()) {
            // Move the first node into its new bucket without reallocating.
            auto &target = next.at(home(chain.front(), next));
            target.splice_after(target.before_begin(), chain,
                                chain.before_begin());
        }
    }
    buckets_ = std::move(next);
}

auto ProbingHashSet::locate(int key) const -> std::pair<std::size_t, bool> {
    const auto capacity = slots_.size();
    auto free = std::optional<std::size_t>{}; // First tombstone on the way.
    // Terminates: the load bound always leaves an empty slot.
    for (auto index = home(key, slots_);; index = (index + 1) % capacity) {
        const auto &slot = slots_.at(index);
        switch (slot.state) {
        case State::empty:
            return {free.value_or(index), false};
        case State::deleted:
            free = free.value_or(index);
            break;
        case State::occupied:
            if (slot.key == key) {
                return {index, true};
            }
        }
    }
}

auto ProbingHashSet::add(int key) -> bool {
    const auto [index, found] = locate(key);
    if (found) {
        return false;
    }
    auto &slot = slots_.at(index);
    if (slot.state == State::deleted) {
        --deleted_;
    }
    slot = {.state = State::occupied, .key = key};
    ++size_;
    const auto capacity = slots_.size();
    if (4 * (size_ + deleted_) > 3 * capacity) {
        // Grow only when live keys need it; otherwise just drop tombstones.
        rebuild(2 * size_ > capacity ? 2 * capacity : capacity);
    }
    return true;
}

auto ProbingHashSet::remove(int key) -> bool {
    const auto [index, found] = locate(key);
    if (!found) {
        return false;
    }
    slots_.at(index).state = State::deleted;
    --size_;
    ++deleted_;
    return true;
}

auto ProbingHashSet::contains(int key) const -> bool {
    return locate(key).second;
}

auto ProbingHashSet::size() const noexcept -> std::size_t { return size_; }

auto ProbingHashSet::rebuild(std::size_t capacity) -> void {
    const auto old = std::exchange(slots_, std::vector<Slot>(capacity));
    deleted_ = 0;
    for (const auto &slot : old) {
        if (slot.state == State::occupied) {
            slots_.at(locate(slot.key).first) = slot;
        }
    }
}
} // namespace algos
