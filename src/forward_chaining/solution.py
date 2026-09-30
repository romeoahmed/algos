from collections.abc import Sequence, Set
from functools import reduce
from typing import NamedTuple

type Rule = tuple[Set[str], str]


class Inference(NamedTuple):
    facts: frozenset[str]
    derived: tuple[str, ...]


def infer(facts: Set[str], rules: Sequence[Rule]) -> Inference:
    """Return the least rule-closed facts and the ordered trace of new facts.

    Rules are finite positive implications, scanned in order each round.
    New facts can trigger later rules in the same round. Inputs are preserved.
    """

    def apply(state: Inference, rule: Rule) -> Inference:
        conditions, conclusion = rule
        if conclusion in state.facts or not conditions <= state.facts:
            return state
        return Inference(state.facts | {conclusion}, (*state.derived, conclusion))

    def close(state: Inference) -> Inference:
        following = reduce(apply, rules, initial=state)
        if len(following.facts) == len(state.facts):
            return following
        return close(following)

    return close(Inference(frozenset(facts), ()))
