#pragma once

#include <cstddef>
#include <stack>

namespace algos {
// FIFO queue on two LIFO stacks.
class StackQueue {
  public:
    // Append value at the back.
    auto push(int value) -> void;

    // Remove and return the front; amortized O(1). Requires a nonempty queue.
    auto pop() -> int;

    // Return the front without changing either stack. Requires a nonempty
    // queue.
    [[nodiscard]] auto peek() const -> int;

    [[nodiscard]] auto empty() const noexcept -> bool;
    [[nodiscard]] auto size() const noexcept -> std::size_t;

  private:
    std::stack<int> inbox_;  // Newest on top.
    std::stack<int> outbox_; // Oldest on top; empty only when the queue is.
};
} // namespace algos
