from typing import NamedTuple


class Cell(NamedTuple):
    row: int
    column: int


type Tile = tuple[Cell, Cell, Cell]


def tromino_tiling(size: int, hole: Cell) -> tuple[Tile, ...]:
    """Return L-shaped tiles covering the board except for hole.

    Requires a power-of-two size >= 1 and 0 <= hole coordinates < size.
    Tile and cell order are unspecified. Coordinates are zero-based.
    """
    tiles: list[Tile] = []

    def cover(origin: Cell, side: int, missing: Cell) -> None:
        if side == 1:
            return
        top, left = origin
        half = side // 2
        quadrant = 2 * (missing.row >= top + half) + (missing.column >= left + half)
        centers = tuple(
            Cell(top + half - 1 + q // 2, left + half - 1 + q % 2) for q in range(4)
        )
        a, b, c = (cell for q, cell in enumerate(centers) if q != quadrant)
        tiles.append((a, b, c))
        for q, center in enumerate(centers):
            cover(
                Cell(top + q // 2 * half, left + q % 2 * half),
                half,
                missing if q == quadrant else center,
            )

    cover(Cell(0, 0), size, hole)
    return tuple(tiles)
