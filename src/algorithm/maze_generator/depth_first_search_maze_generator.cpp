#include "algorithm/maze_generator/depth_first_search_maze_generator.h"
#include <random>

void DepthFirstSearchMazeGenerator::generate(
    Grid &grid, const Grid::Location &start, const Grid::Location &goal, std::vector<Grid::ChangeRecord> &record
)
{
    this->validateArguments(grid, start, goal);
    record.clear();

    Timer        timer;
    std::mt19937 gen = this->getRandomGenerator();

    for (size_t i = 0; i < grid.height; i++)
    {
        std::fill(grid.grid[i].begin(), grid.grid[i].end(), Grid::CellType::WALL);
    }

    grid[start] = Grid::CellType::EMPTY;
    record.push_back({start, std::chrono::microseconds(0)});

    grid[goal] = Grid::CellType::EMPTY;
    record.push_back({goal, std::chrono::microseconds(0)});

    std::stack<Grid::Location> to_visit;
    to_visit.push(start);

    while (!to_visit.empty())
    {
        Grid::Location current = to_visit.top();
        to_visit.pop();

        std::vector<Grid::Location> neighbors = grid.neighbors(current, 1, false);

        if (neighbors.empty())
        {
            continue;
        }

        std::uniform_int_distribution<size_t> dist(0, neighbors.size() - 1);
        Grid::Location                        random_neighbor = neighbors[dist(gen)];

        to_visit.push(current);
        to_visit.push(random_neighbor);

        Grid::Location neighbor_link{(random_neighbor.x + current.x) / 2, (random_neighbor.y + current.y) / 2};
        grid[neighbor_link]   = Grid::CellType::EMPTY;
        grid[random_neighbor] = Grid::CellType::EMPTY;

        timer.tock();

        record.push_back({neighbor_link, timer.duration()});
        record.push_back({random_neighbor, timer.duration()});
    }
}
