#pragma once

#include <cstddef>
#include <map>
#include <span>
#include <vector>

namespace algos {
// Stores P(node=true) by parent bits, first parent most significant.
struct BooleanNode {
    std::vector<std::size_t> parents;
    std::vector<double> probabilities;

    auto operator==(const BooleanNode &) const -> bool = default;
};

// Returns P(query=true | evidence) by exact enumeration. Preserves inputs.
// Requires a nonempty topologically ordered Boolean network and valid indices.
// Parents are distinct earlier indices; tables contain 2^parents.size() finite
// probabilities in [0, 1]. Parent bits use false=0 and true=1.
// Table indices must fit in size_t.
// Evidence has positive probability and a nonzero computed mass.
[[nodiscard]] auto
bayesian_inference(std::span<const BooleanNode> network, std::size_t query,
                   const std::map<std::size_t, bool> &evidence) -> double;
} // namespace algos
