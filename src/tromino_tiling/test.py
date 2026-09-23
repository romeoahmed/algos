from collections import Counter
from itertools import product

import pytest
from hypothesis import given
from hypothesis import strategies as st

from src.tromino_tiling.solution import Cell, Tile, tromino_tiling


def check_cover(size: int, hole: Cell, tiles: tuple[Tile, ...]) -> None:
    assert len(tiles) == (size * size - 1) // 3
    for tile in tiles:
        assert len(set(tile)) == 3
        assert max(cell.row for cell in tile) - min(cell.row for cell in tile) == 1
        assert (
            max(cell.column for cell in tile) - min(cell.column for cell in tile) == 1
        )
    assert Counter(cell for tile in tiles for cell in tile) == Counter(
        Cell(row, column)
        for row, column in product(range(size), repeat=2)
        if Cell(row, column) != hole
    )


@given(st.integers(0, 6), st.data())
def test_covers_every_other_cell_once(exponent: int, data: st.DataObject) -> None:
    size = 1 << exponent
    hole = Cell(
        data.draw(st.integers(0, size - 1)), data.draw(st.integers(0, size - 1))
    )
    check_cover(size, hole, tromino_tiling(size, hole))


@pytest.mark.parametrize("size", [1, 2, 4, 8])
def test_every_small_hole(size: int) -> None:
    for row, column in product(range(size), repeat=2):
        hole = Cell(row, column)
        check_cover(size, hole, tromino_tiling(size, hole))


def test_results_survive_other_calls() -> None:
    first = tromino_tiling(4, Cell(0, 0))
    second = tromino_tiling(8, Cell(7, 7))
    assert tromino_tiling(1, Cell(0, 0)) == ()
    check_cover(4, Cell(0, 0), first)
    check_cover(8, Cell(7, 7), second)
