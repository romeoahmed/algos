#pragma once

#include <memory>
#include <span>
#include <vector>

namespace algos {
struct ListNode;

// Frees a chain front to back, so destroying a long list cannot overflow the
// stack the way nested unique_ptr destructors would.
struct ListDeleter {
    auto operator()(ListNode *node) const noexcept -> void;
};

// Each node owns its successor; a List owns the whole chain from its head.
using List = std::unique_ptr<ListNode, ListDeleter>;

struct ListNode {
    int value = 0;
    List next;
};

// Return a fresh list holding values in order.
[[nodiscard]] auto make_list(std::span<const int> values) -> List;

// Return the values from head to the end.
[[nodiscard]] auto list_values(const ListNode *head) -> std::vector<int>;

// Take over the chain, reverse it in place by relinking its nodes, and return
// the new head.
[[nodiscard]] auto reverse_list(List head) noexcept -> List;
} // namespace algos
