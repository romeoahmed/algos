from itertools import combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.forward_chaining.solution import Inference, Rule, infer


def least_model(facts: set[str], rules: list[Rule]) -> frozenset[str]:
    """Intersect all closed models over the four generated proposition names."""
    universe = set("abcd")
    candidates = (
        set(subset)
        for size in range(len(universe) + 1)
        for subset in combinations(universe, size)
    )
    models = (
        model
        for model in candidates
        if facts <= model
        and all(
            not conditions <= model or result in model for conditions, result in rules
        )
    )
    return frozenset(universe).intersection(*models)


@given(
    st.sets(st.sampled_from("abcd")),
    st.lists(
        st.tuples(st.sets(st.sampled_from("abcd")), st.sampled_from("abcd")),
        max_size=8,
    ),
)
def test_matches_intersection_of_all_models(facts: set[str], rules: list[Rule]) -> None:
    before = facts.copy(), [(set(conditions), result) for conditions, result in rules]
    expected = least_model(facts, rules)
    result = infer(facts, rules)
    assert result.facts == expected
    assert infer(facts, tuple(reversed(rules))).facts == result.facts
    assert infer(result.facts, rules) == Inference(result.facts, ())
    assert len(result.derived) == len(set(result.derived))
    assert set(result.derived) == result.facts - facts
    known = set(facts)
    for conclusion in result.derived:
        assert any(
            outcome == conclusion and conditions <= known
            for conditions, outcome in rules
        )
        known.add(conclusion)
    assert (facts, rules) == before


@pytest.mark.parametrize(
    ("facts", "rules", "derived"),
    [
        pytest.param(set(), [], (), id="empty"),
        pytest.param({"a"}, [], (), id="facts-without-rules"),
        pytest.param(set(), [({"a"}, "b"), ({"b"}, "a")], (), id="unseeded-cycle"),
        pytest.param({"a"}, [({"a"}, "b"), ({"b"}, "a")], ("b",), id="seeded-cycle"),
        pytest.param(
            set(),
            [(set(), "a"), ({"a"}, "b"), ({"a"}, "b")],
            ("a", "b"),
            id="unconditional-and-duplicate",
        ),
        pytest.param(
            {"a"},
            [({"b"}, "c"), ({"a"}, "b"), ({"b"}, "d")],
            ("b", "d", "c"),
            id="ordered-rounds",
        ),
        pytest.param({"a"}, [({"a", "b"}, "c")], (), id="missing-condition"),
        pytest.param({"a"}, [({"a"}, "a")], (), id="self-rule"),
        pytest.param({"a"}, [({"a"}, ""), ({""}, "b")], ("", "b"), id="empty-label"),
        pytest.param(
            {"a"},
            [({"d"}, "e"), ({"c"}, "d"), ({"b"}, "c"), ({"a"}, "b")],
            ("b", "c", "d", "e"),
            id="reverse-chain",
        ),
    ],
)
def test_order_cycles_and_empty_conditions(
    facts: set[str], rules: list[Rule], derived: tuple[str, ...]
) -> None:
    expected = Inference(frozenset(facts | set(derived)), derived)
    assert infer(facts, rules) == expected


def test_classroom_example_and_owned_result(capsys: pytest.CaptureFixture[str]) -> None:
    facts = {"发烧", "咳嗽"}
    conditions = facts.copy()
    rules: list[Rule] = [
        (conditions, "可能流感"),
        ({"可能流感"}, "进行流感检测"),
        ({"进行流感检测"}, "等待检测结果"),
    ]
    expected = Inference(
        frozenset({"发烧", "咳嗽", "可能流感", "进行流感检测", "等待检测结果"}),
        ("可能流感", "进行流感检测", "等待检测结果"),
    )
    result = infer(facts, rules)
    assert result == expected
    assert infer(set(), rules) == Inference(frozenset(), ())
    facts.clear()
    conditions.clear()
    rules.clear()
    assert result == expected
    assert capsys.readouterr() == ("", "")
