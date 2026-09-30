#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <span>
#include <vector>

namespace algos {
auto bayesian_inference(std::span<const BooleanNode> network, std::size_t query,
                        const std::map<std::size_t, bool> &evidence) -> double {
    if (const auto observed = evidence.find(query);
        observed != evidence.end()) {
        return static_cast<double>(observed->second);
    }
    const auto enumerate_all =
        [network, query, &evidence](this const auto &self,
                                    const std::vector<std::uint8_t> &values,
                                    bool query_value) -> double {
        const auto index = values.size();
        if (index == network.size()) {
            return 1.0;
        }
        const auto &node = network.subspan(index).front();
        const auto row = std::ranges::fold_left(
            node.parents, std::size_t{0},
            [&values](std::size_t prefix, std::size_t parent) -> std::size_t {
                return 2 * prefix + values.at(parent);
            });
        const auto p_true = node.probabilities.at(row);
        const auto branch = [&](bool value) -> double {
            const auto probability = value ? p_true : 1 - p_true;
            if (probability == 0) {
                return 0.0;
            }
            auto following = std::vector<std::uint8_t>{};
            following.reserve(values.size() + 1);
            following.append_range(values);
            following.push_back(value);
            return probability * self(following, query_value);
        };
        if (const auto found = evidence.find(index); found != evidence.end()) {
            return branch(found->second);
        }
        if (index == query) {
            return branch(query_value);
        }
        return branch(false) + branch(true);
    };
    const auto false_mass = enumerate_all({}, false);
    const auto true_mass = enumerate_all({}, true);
    return true_mass / (false_mass + true_mass);
}
} // namespace algos
