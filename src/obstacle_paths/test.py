from itertools import accumulate, combinations

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.obstacle_paths.solution import obstacle_paths


def walk_all(grid: list[str]) -> int:
    """Walk every arrangement of right and down moves and count the clear ones."""
    rows, columns = len(grid), len(grid[0])
    moves = rows + columns - 2

    def clear(downs: tuple[int, ...]) -> bool:
        steps = ((1, 0) if step in downs else (0, 1) for step in range(moves))
        cells = accumulate(
            steps, lambda at, by: (at[0] + by[0], at[1] + by[1]), initial=(0, 0)
        )
        return all(grid[row][column] != "#" for row, column in cells)

    return sum(clear(downs) for downs in combinations(range(moves), rows - 1))


@st.composite
def grids(draw: st.DrawFn) -> list[str]:
    columns = draw(st.integers(1, 6))
    row = st.text(alphabet=".#", min_size=columns, max_size=columns)
    return draw(st.lists(row, min_size=1, max_size=6))


@given(grids())
def test_matches_walking_every_path(grid: list[str]) -> None:
    before = grid.copy()
    expected = walk_all(grid)
    assert obstacle_paths(grid) == expected
    assert obstacle_paths(tuple(grid)) == expected
    assert grid == before


@pytest.mark.parametrize(
    ("grid", "expected"),
    [
        pytest.param(["."], 1, id="single-cell"),
        pytest.param(["#"], 0, id="single-obstacle"),
        pytest.param(["#.", ".."], 0, id="blocked-start"),
        pytest.param(["..", ".#"], 0, id="blocked-end"),
        pytest.param(["...", ".#.", "..."], 2, id="example"),
        pytest.param([".#.", "..."], 1, id="cut-first-row"),
        pytest.param(["..#.."], 0, id="cut-single-row"),
        pytest.param(["..#", ".#.", "#.."], 0, id="wall"),
        pytest.param(["..."] * 3, 6, id="open"),
        pytest.param(["." * 34] * 34, 7219428434016265740, id="uint64-square"),
        pytest.param(["." * 34] * 33 + ["." * 33 + "#"], 0, id="large-blocked-end"),
    ],
)
def test_boundaries(grid: list[str], expected: int) -> None:
    assert obstacle_paths(grid) == expected
