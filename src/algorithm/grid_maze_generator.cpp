#include "algorithm/grid_maze_generator.h"

void GridMazeGenerator::generate(Grid &grid, Grid::Location start,
                                 Grid::Location end,
                                 std::vector<Grid::ChangeRecord> &record) {
    // set entire grid to wall
    for (int i = 0; i < grid.height; i++) {
        std::fill(grid.grid[i].begin(), grid.grid[i].end(),
                  Grid::CellType::WALL);
    }

    // random setup
    std::random_device rd;
    std::mt19937::result_type seed =
        rd() ^
        ((std::mt19937::result_type)
             std::chrono::duration_cast<std::chrono::seconds>(
                 std::chrono::system_clock::now().time_since_epoch())
                 .count() +
         (std::mt19937::result_type)
             std::chrono::duration_cast<std::chrono::microseconds>(
                 std::chrono::high_resolution_clock::now().time_since_epoch())
                 .count());

    std::mt19937 gen(seed);

    grid[start] = Grid::CellType::EMPTY;
    grid[end] = Grid::CellType::EMPTY;

    std::stack<Grid::Location> to_visit;
    to_visit.push(start);

    record.push_back({start, std::chrono::microseconds(0)});
    record.push_back({end, std::chrono::microseconds(0)});

    Timer timer;

    while (!to_visit.empty()) {
        Grid::Location current = to_visit.top();
        to_visit.pop();

        std::vector<Grid::Location> neighbors =
            grid.neighbors(current, 1, false);

        if (neighbors.empty()) {
            continue;
        }

        std::uniform_int_distribution<int> dist(0, neighbors.size() - 1);
        Grid::Location random_neighbor = neighbors[dist(gen)];

        to_visit.push(current);
        to_visit.push(random_neighbor);

        Grid::Location neighbor_link{(random_neighbor.x + current.x) / 2,
                                     (random_neighbor.y + current.y) / 2};
        grid[neighbor_link] = Grid::CellType::EMPTY;
        grid[random_neighbor] = Grid::CellType::EMPTY;

        timer.tock();

        record.push_back({neighbor_link, timer.duration()});
        record.push_back({random_neighbor, timer.duration()});
    }
}
