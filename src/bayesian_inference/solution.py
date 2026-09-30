from collections.abc import Mapping, Sequence
from functools import reduce
from typing import NamedTuple


class BooleanNode(NamedTuple):
    """Store P(node=True) by parent bits, first parent most significant."""

    parents: tuple[int, ...]
    probabilities: tuple[float, ...]


def bayesian_inference(
    network: Sequence[BooleanNode], query: int, evidence: Mapping[int, bool]
) -> float:
    """Return P(query=True | evidence) by exact enumeration.

    Requires a nonempty Boolean network in topological order, valid indices,
    and complete conditional tables with finite probabilities in [0, 1].
    Parent values index each table as binary digits, False=0 and True=1,
    with the first parent most significant. Parents are distinct earlier indices.
    Evidence must have positive probability and a nonzero computed mass.
    Inputs are preserved; network and parent sequences must be reusable.
    """
    if query in evidence:
        return float(evidence[query])

    def enumerate_all(values: tuple[bool, ...], query_value: bool) -> float:
        index = len(values)
        if index == len(network):
            return 1.0
        node = network[index]

        def append_bit(row: int, parent: int) -> int:
            return 2 * row + values[parent]

        row = reduce(append_bit, node.parents, 0)
        p_true = node.probabilities[row]

        def branch(value: bool) -> float:
            probability = p_true if value else 1 - p_true
            if probability == 0:
                return 0.0
            return probability * enumerate_all((*values, value), query_value)

        if index in evidence:
            return branch(evidence[index])
        if index == query:
            return branch(query_value)
        return sum(map(branch, (False, True)))

    false_mass = enumerate_all((), False)
    true_mass = enumerate_all((), True)
    return true_mass / (false_mass + true_mass)
