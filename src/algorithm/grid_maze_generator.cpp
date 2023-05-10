#include "algorithm/grid_maze_generator.h"

void GridMazeGenerator::generate(Grid &grid, const Grid::Location &start,
                                 const Grid::Location &goal,
                                 std::vector<Grid::ChangeRecord> &record) {
    if (grid.height <= GridMazeGenerator::min_height ||
        grid.width <= GridMazeGenerator::min_width) {
        throw std::invalid_argument(
            "Grid Maze Generator exception: Cannot generate maze. Provided "
            "grid is too small to generate a maze.\nMinimum grid size "
            "is " +
            std::to_string(GridMazeGenerator::min_width) + "x" +
            std::to_string(GridMazeGenerator::min_height) + ", grid of size " +
            std::to_string(grid.width) + "x" + std::to_string(grid.height) +
            " was provided.");
    }

    if (!grid.isInBounds(start) || !grid.isInBounds(goal)) {
        throw std::invalid_argument(
            "Grid Maze Generator exception: Cannot generate maze. Provided "
            "start and/or goal are invalid.\nStart: " +
            std::to_string(start) + "; Goal: " + std::to_string(goal) +
            "; Grid Size is " + std::to_string(grid.width) + "x" +
            std::to_string(grid.height) + ".");
    }

    for (size_t i = 0; i < grid.height; i++) {
        std::fill(grid.grid[i].begin(), grid.grid[i].end(),
                  Grid::CellType::WALL);
    }

    record.clear();

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
    record.push_back({start, std::chrono::microseconds(0)});

    grid[goal] = Grid::CellType::EMPTY;
    record.push_back({goal, std::chrono::microseconds(0)});

    std::stack<Grid::Location> to_visit;
    to_visit.push(start);

    Timer timer;

    while (!to_visit.empty()) {
        Grid::Location current = to_visit.top();
        to_visit.pop();

        std::vector<Grid::Location> neighbors =
            grid.neighbors(current, 1, false);

        if (neighbors.empty()) {
            continue;
        }

        std::uniform_int_distribution<size_t> dist(0, neighbors.size() - 1);
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
