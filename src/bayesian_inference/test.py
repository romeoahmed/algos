from itertools import product
from math import prod

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.bayesian_inference.solution import BooleanNode, bayesian_inference


def event_masses(
    network: list[BooleanNode], query: int, evidence: dict[int, bool]
) -> tuple[float, float]:
    masses = [0.0, 0.0]
    for event in product((False, True), repeat=len(network)):
        if any(event[index] != value for index, value in evidence.items()):
            continue
        probabilities = (
            node.probabilities[
                sum(
                    int(event[parent]) << bit
                    for bit, parent in enumerate(reversed(node.parents))
                )
            ]
            for node in network
        )
        masses[event[query]] += prod(
            probability if value else 1 - probability
            for value, probability in zip(event, probabilities, strict=True)
        )
    return masses[0], masses[1]


@st.composite
def networks(draw: st.DrawFn) -> tuple[list[BooleanNode], dict[int, bool]]:
    size = draw(st.integers(1, 6))
    network = []
    for index in range(size):
        parents = (
            tuple(draw(st.lists(st.integers(0, index - 1), unique=True, max_size=3)))
            if index
            else ()
        )
        numerators = draw(
            st.lists(
                st.integers(1, 9),
                min_size=2 ** len(parents),
                max_size=2 ** len(parents),
            )
        )
        network.append(BooleanNode(parents, tuple(value / 10 for value in numerators)))
    evidence = draw(st.dictionaries(st.integers(0, size - 1), st.booleans()))
    return network, evidence


@given(networks())
def test_matches_complete_event_table(
    case: tuple[list[BooleanNode], dict[int, bool]],
) -> None:
    network, evidence = case
    original_network = network.copy()
    original_evidence = evidence.copy()
    for query in range(len(network)):
        false_mass, true_mass = event_masses(network, query, evidence)
        result = bayesian_inference(network, query, evidence)
        assert result == pytest.approx(
            true_mass / (false_mass + true_mass), rel=1e-12, abs=1e-12
        )
        assert 0 <= result <= 1
        assert bayesian_inference(tuple(network), query, evidence) == result
    assert network == original_network
    assert evidence == original_evidence


@pytest.mark.parametrize("prior", [0.0, 0.1, 0.5, 1.0])
def test_single_node_and_observed_query(prior: float) -> None:
    network = [BooleanNode((), (prior,))]
    assert bayesian_inference(network, 0, {}) == pytest.approx(prior)
    if prior > 0:
        assert bayesian_inference(network, 0, {0: True}) == 1
    if prior < 1:
        assert bayesian_inference(network, 0, {0: False}) == 0


@pytest.mark.parametrize("value", [False, True])
def test_deterministic_child_and_disconnected_evidence(value: bool) -> None:
    network = [
        BooleanNode((), (0.3,)),
        BooleanNode((0,), (0.0, 1.0)),
        BooleanNode((), (0.7,)),
    ]
    assert bayesian_inference(network, 0, {1: value}) == float(value)
    assert bayesian_inference(network, 0, {2: value}) == pytest.approx(0.3)


def test_deterministic_chain() -> None:
    network = [BooleanNode((), (0.3,))] + [
        BooleanNode((index - 1,), (0.0, 1.0)) for index in range(1, 80)
    ]
    last = len(network) - 1
    assert bayesian_inference(network, last, {}) == pytest.approx(0.3)
    assert bayesian_inference(network, 0, {last: True}) == 1
    assert bayesian_inference(network, 0, {last: False}) == 0


@pytest.mark.parametrize(
    ("first", "second", "expected"),
    [(False, False, 0.1), (True, False, 0.2), (False, True, 0.7), (True, True, 0.9)],
)
def test_parent_order(first: bool, second: bool, expected: float) -> None:
    network = [
        BooleanNode((), (0.3,)),
        BooleanNode((), (0.6,)),
        BooleanNode((1, 0), (0.1, 0.2, 0.7, 0.9)),
    ]
    assert bayesian_inference(network, 2, {0: first, 1: second}) == pytest.approx(
        expected
    )


def test_sprinkler_example(capsys: pytest.CaptureFixture[str]) -> None:
    network = [
        BooleanNode((), (0.5,)),
        BooleanNode((0,), (0.5, 0.1)),
        BooleanNode((0,), (0.2, 0.8)),
        BooleanNode((1, 2), (0.0, 0.9, 0.9, 0.99)),
    ]
    assert event_masses(network, 2, {3: True}) == pytest.approx((0.189, 0.4581))
    assert bayesian_inference(network, 3, {}) == pytest.approx(0.6471)
    posterior = bayesian_inference(network, 2, {3: True})
    assert posterior == pytest.approx(0.4581 / 0.6471)
    assert bayesian_inference(network, 2, {}) == pytest.approx(0.5)
    assert bayesian_inference(network, 2, {3: True}) == posterior
    assert capsys.readouterr().out == ""
