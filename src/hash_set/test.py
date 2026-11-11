import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.hash_set.solution import ChainedHashSet, ProbingHashSet

type HashSet = type[ChainedHashSet] | type[ProbingHashSet]
SETS = [ChainedHashSet, ProbingHashSet]

# Small keys hit each other, multiples of 64 share low bits, and the rest
# cover arbitrary integers.
keys = st.integers(-16, 16) | st.integers(-16, 16).map(lambda k: 64 * k) | st.integers()


@pytest.mark.parametrize("hash_set", SETS)
@given(operations=st.lists(st.tuples(st.booleans(), keys)))
def test_matches_builtin_set(
    hash_set: HashSet, operations: list[tuple[bool, int]]
) -> None:
    """True adds and False removes; Python's built-in set is the reference."""
    table = hash_set()
    model: set[int] = set()
    for adding, key in operations:
        if adding:
            assert table.add(key) == (key not in model)
            model.add(key)
        else:
            assert table.remove(key) == (key in model)
            model.discard(key)
        assert len(table) == len(model)
    for _, key in operations:
        assert (key in table) == (key in model)


@pytest.mark.parametrize("hash_set", SETS)
def test_remove_keeps_other_keys(hash_set: HashSet) -> None:
    table = hash_set()
    for key in (10, 18, 26, 34):
        assert table.add(key)
    assert table.remove(18)
    assert 26 in table
    assert 18 not in table
    assert table.add(42)
    assert not table.add(26)
    assert [key in table for key in (10, 18, 26, 34, 42)] == [
        True,
        False,
        True,
        True,
        True,
    ]


@pytest.mark.parametrize("hash_set", SETS)
def test_churn_clears_tombstones(hash_set: HashSet) -> None:
    table = hash_set()
    for key in range(100_000):
        assert table.add(key)
        assert table.remove(key)
    assert len(table) == 0
    assert 0 not in table


@pytest.mark.parametrize("hash_set", SETS)
def test_many_keys(hash_set: HashSet) -> None:
    table = hash_set()
    size = 20_000
    assert all(table.add(key) for key in range(-size, size))
    assert all(table.remove(key) for key in range(-size, size, 2))
    assert len(table) == size
    assert all((key in table) == (key % 2 == 1) for key in range(-size, size))
