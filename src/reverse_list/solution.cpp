#include "solution.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

namespace algos {
auto ListDeleter::operator()(ListNode *node) const noexcept -> void {
    while (node != nullptr) {
        // Adopt one node and detach its successor, so freeing it recurses no
        // further.
        const auto owned = std::unique_ptr<ListNode>{node};
        node = owned->next.release();
    }
}

auto make_list(std::span<const int> values) -> List {
    return std::ranges::fold_left(
        values | std::views::reverse, List{}, [](List rest, int value) -> List {
            // List has its own deleter, so adopt the node make_unique built.
            return List{
                std::make_unique<ListNode>(value, std::move(rest)).release()};
        });
}

auto list_values(const ListNode *head) -> std::vector<int> {
    auto values = std::vector<int>{};
    for (auto node = head; node != nullptr; node = node->next.get()) {
        values.push_back(node->value);
    }
    return values;
}

auto reverse_list(List head) noexcept -> List {
    auto reversed = List{};
    while (head) {
        auto rest = std::move(head->next); // Save the successor first.
        head->next = std::move(reversed);
        reversed = std::move(head);
        head = std::move(rest);
    }
    return reversed;
}
} // namespace algos
