#include "solution.hpp"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <set>
#include <span>
#include <utility>
#include <vector>

namespace {
auto valid_cover(std::size_t size, algos::Cell hole,
                 std::span<const algos::Tile> tiles) -> bool {
    if (tiles.size() != (size * size - 1) / 3) {
        return false;
    }
    auto covered = std::set<std::pair<std::size_t, std::size_t>>{};
    for (const auto &tile : tiles) {
        const auto rows = std::ranges::minmax(tile, {}, &algos::Cell::row);
        const auto columns =
            std::ranges::minmax(tile, {}, &algos::Cell::column);
        if (rows.max.row - rows.min.row != 1 ||
            columns.max.column - columns.min.column != 1) {
            return false;
        }
        for (const auto cell : tile) {
            if (cell.row >= size || cell.column >= size || cell == hole ||
                !covered.emplace(cell.row, cell.column).second) {
                return false;
            }
        }
    }
    return covered.size() == size * size - 1;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;
    "covers every other cell exactly once"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto size = std::size_t{1} << *rc::gen::inRange(0U, 7U);
            const auto coordinate = rc::gen::inRange(std::size_t{0}, size);
            const auto hole =
                algos::Cell{.row = *coordinate, .column = *coordinate};
            RC_ASSERT(
                valid_cover(size, hole, algos::tromino_tiling(size, hole)));
        }));
    };
    "every small hole"_test = [](std::size_t size) -> void {
        for (auto row = std::size_t{0}; row < size; ++row) {
            for (auto column = std::size_t{0}; column < size; ++column) {
                const auto hole = algos::Cell{.row = row, .column = column};
                expect(
                    valid_cover(size, hole, algos::tromino_tiling(size, hole)));
            }
        }
    } | std::vector<std::size_t>{1, 2, 4, 8};
    "independent results"_test = [] -> void {
        const auto hole = algos::Cell{.row = 0, .column = 0};
        const auto first = algos::tromino_tiling(4, hole);
        auto second = algos::tromino_tiling(8, {.row = 7, .column = 7});
        expect(fatal(!second.empty()));
        second.front().front().row = 8;
        expect(valid_cover(4, hole, first));
        second.clear();
        expect(algos::tromino_tiling(1, hole).empty());
        expect(valid_cover(4, hole, first));
        expect(valid_cover(8, {.row = 7, .column = 7},
                           algos::tromino_tiling(8, {.row = 7, .column = 7})));
    };
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
