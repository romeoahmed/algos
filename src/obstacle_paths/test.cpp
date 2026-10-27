#include "solution.hpp"

#include <bit>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <rapidcheck.h>
#include <string>
#include <utility>
#include <vector>

namespace {
// Walk every arrangement of right and down moves and count the clear ones.
auto walk_all(const std::vector<std::string> &grid) -> std::uint64_t {
    const auto rows = grid.size();
    const auto columns = grid.front().size();
    const auto moves = rows + columns - 2;
    auto clear_paths = std::uint64_t{0};
    for (auto downs = std::size_t{0}; downs < std::size_t{1} << moves;
         ++downs) {
        if (static_cast<std::size_t>(std::popcount(downs)) != rows - 1) {
            continue;
        }
        auto row = std::size_t{0};
        auto column = std::size_t{0};
        auto clear = grid.front().front() != '#';
        for (auto step = std::size_t{0}; step < moves && clear; ++step) {
            if ((downs >> step & 1U) != 0) {
                ++row;
            } else {
                ++column;
            }
            clear = grid.at(row).at(column) != '#';
        }
        if (clear) {
            ++clear_paths;
        }
    }
    return clear_paths;
}

// An open square whose end cell is an obstacle.
auto blocked_end(std::size_t size) -> std::vector<std::string> {
    auto grid = std::vector<std::string>(size, std::string(size, '.'));
    grid.back().back() = '#';
    return grid;
}
} // namespace

auto main() -> int try {
    using namespace boost::ut;

    "matches walking every path"_test = [] -> void {
        expect(rc::check([] -> void {
            const auto rows = *rc::gen::inRange<std::size_t>(1, 7);
            const auto columns = *rc::gen::inRange<std::size_t>(1, 7);
            auto grid = *rc::gen::container<std::vector<std::string>>(
                rows, rc::gen::container<std::string>(
                          columns, rc::gen::weightedElement<char>(
                                       {{3, '.'}, {1, '#'}})));
            const auto before = grid;
            RC_ASSERT(algos::obstacle_paths(grid) == walk_all(grid));
            RC_ASSERT(grid == before);
        }));
    };

    "single cells, blocked ends, cut rows, and the largest square"_test =
        [](const auto &example) -> void {
        const auto &[grid, expected] = example;
        expect(that % algos::obstacle_paths(grid) == expected);
    } | std::vector<std::pair<std::vector<std::string>, std::uint64_t>>{
            {{"."}, 1},
            {{"#"}, 0},
            {{"#.", ".."}, 0},
            {{"..", ".#"}, 0},
            {{"...", ".#.", "..."}, 2},
            {{".#.", "..."}, 1},
            {{"..#.."}, 0},
            {{"..#", ".#.", "#.."}, 0},
            {{"...", "...", "..."}, 6},
            {std::vector<std::string>(34, std::string(34, '.')),
             7'219'428'434'016'265'740U},
            {blocked_end(34), 0}};
} catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
}
