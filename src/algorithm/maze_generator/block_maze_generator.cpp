#include "algorithm/maze_generator/block_maze_generator.h"
#include <random>
#include <stdexcept>
#include <string>

void BlockMazeGenerator::generate(
    Grid &grid, const Grid::Location &start, const Grid::Location &goal, std::vector<Grid::ChangeRecord> &record
)
{
    this->validateArguments(grid, start, goal);
    record.clear();

    Timer        timer;
    std::mt19937 gen  = this->getRandomGenerator();
    unsigned     step = 0;

    for (size_t i = 0; i < grid.height; i++)
    {
        std::fill(grid.grid[i].begin(), grid.grid[i].end(), Grid::CellType::WALL);
    }

    std::uniform_int_distribution<int> block_space_offset_dist(0, (int)grid.height - 3);
    std::uniform_int_distribution<int> block_space_height_dist(2, 5);
    std::uniform_int_distribution<int> block_space_width_dist(2, 8);
    std::uniform_int_distribution<int> vertical_line_width_dist(2, 8);
    std::uniform_int_distribution<int> floating_offset(2, 3);
    std::bernoulli_distribution        is_floating_dist;

    int current = 0;

    while ((size_t)current < grid.width)
    {
        int vertical_line_width = vertical_line_width_dist(gen);
        this->removeWall(
            grid, {current, 0}, {current + vertical_line_width, (int)grid.height - 1}, record, timer, step
        );

        current += vertical_line_width + 1;

        int block_space_width = block_space_width_dist(gen);

        Grid::Location from;
        Grid::Location to;

        if (start.x >= current && start.x <= current + block_space_width)
        {
            from = {current, start.y - 1};
            to   = {current + block_space_width, start.y - 1 + block_space_height_dist(gen)};
        }
        else if (goal.x >= current && goal.x <= current + block_space_width)
        {
            from = {current, goal.y - 1};
            to   = {current + block_space_width, goal.y - 1 + block_space_height_dist(gen)};
        }
        else
        {
            int block_space_offset = block_space_offset_dist(gen);

            from = {current, block_space_offset};
            to   = {current + block_space_width, block_space_offset + block_space_height_dist(gen)};
        }

        this->removeWall(grid, from, to, record, timer, step);

        if (is_floating_dist(gen))
        {
            this->removeWall(grid, {from.x, 0}, {to.x, floating_offset(gen)}, record, timer, step);
        }

        if (is_floating_dist(gen))
        {
            this->removeWall(
                grid,
                {from.x, (int)grid.height - 1 - floating_offset(gen)},
                {to.x, (int)grid.height - 1},
                record,
                timer,
                step
            );
        }

        current += block_space_width + 1;
    }
}

void BlockMazeGenerator::removeWall(
    Grid                                                                 &grid,
    Grid::Location                                                        from,
    Grid::Location                                                        to,
    std::vector<Grid::ChangeRecord>                                      &record,
    Timer<std::chrono::microseconds, std::chrono::high_resolution_clock> &timer,
    unsigned                                                             &step
)
{
    auto moveInBounds = [](int &value, int max) {
        value = value < 0 ? 0 : value;
        value = value > max ? max : value;
    };

    moveInBounds(from.x, grid.width - 1);
    moveInBounds(from.y, grid.height - 1);
    moveInBounds(to.x, grid.width - 1);
    moveInBounds(to.y, grid.height - 1);

    for (int y = from.y; y <= to.y; y++)
    {
        for (int x = from.x; x <= to.x; x++)
        {
            grid[{x, y}] = Grid::CellType::EMPTY;

            timer.tock();
            record.push_back({
                {x, y},
                timer.duration(), step++
            });
        }
    }
}
