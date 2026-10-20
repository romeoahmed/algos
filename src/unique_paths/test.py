from math import comb

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.unique_paths.solution import unique_paths


@given(st.integers(1, 80), st.integers(1, 80))
def test_matches_binomial(rows: int, columns: int) -> None:
    expected = comb(rows + columns - 2, rows - 1)
    assert unique_paths(rows, columns) == expected
    assert unique_paths(columns, rows) == expected


@pytest.mark.parametrize(
    ("rows", "columns", "expected"),
    [
        pytest.param(1, 1, 1, id="single-cell"),
        pytest.param(3, 2, 3, id="small"),
        pytest.param(3, 4, 10, id="example"),
        pytest.param(3, 7, 28, id="wide"),
        pytest.param(1, 100_000, 1, id="one-row"),
        pytest.param(100_000, 1, 1, id="one-column"),
        pytest.param(34, 34, 7219428434016265740, id="uint64-square"),
    ],
)
def test_boundaries(rows: int, columns: int, expected: int) -> None:
    assert unique_paths(rows, columns) == expected
