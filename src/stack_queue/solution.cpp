#include "solution.hpp"

#include <cstddef>

namespace algos {
auto StackQueue::push(int value) -> void {
    // An empty queue's first element is also its front.
    (outbox_.empty() ? outbox_ : inbox_).push(value);
}

auto StackQueue::pop() -> int {
    const auto front = outbox_.top();
    outbox_.pop();
    if (outbox_.empty()) {
        // Reverse the inbox so its oldest element becomes the new top.
        while (!inbox_.empty()) {
            outbox_.push(inbox_.top());
            inbox_.pop();
        }
    }
    return front;
}

auto StackQueue::peek() const -> int { return outbox_.top(); }

auto StackQueue::empty() const noexcept -> bool { return outbox_.empty(); }

auto StackQueue::size() const noexcept -> std::size_t {
    return inbox_.size() + outbox_.size();
}
} // namespace algos
