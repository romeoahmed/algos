#pragma once

#include <cstddef>
#include <map>
#include <span>
#include <vector>

namespace algos {
// P(node=true) indexed by parent bits (false=0, true=1).
// The first declared parent contributes the most significant bit.
struct BooleanNode {
    std::vector<std::size_t> parents;
    std::vector<double> probabilities;

    auto operator==(const BooleanNode &) const -> bool = default;
};

// Return P(query=true | evidence) by exact enumeration.
// Requires a nonempty topologically ordered Boolean network and valid indices.
// Parents are distinct earlier indices; tables contain 2^parents.size() finite
// probabilities in [0, 1], with row indices representable as size_t.
// Evidence must have positive probability and a nonzero computed mass.
[[nodiscard]] auto
bayesian_inference(std::span<const BooleanNode> network, std::size_t query,
                   const std::map<std::size_t, bool> &evidence) -> double;
} // namespace algos
