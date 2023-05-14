#include "algorithm/maze_generator/base_maze_generator.h"

#include <random>
#include <stdexcept>

void BaseMazeGenerator::validateArguments(const Grid &grid, const Grid::Location &start, const Grid::Location &goal)
{
    if (grid.height <= BaseMazeGenerator::min_height || grid.width <= BaseMazeGenerator::min_width)
    {
        throw std::invalid_argument(
            "Maze Generator exception: Cannot generate maze. Provided grid is too small to generate a maze.\nMinimum "
            "grid size is "
            + std::to_string(BaseMazeGenerator::min_width) + "x" + std::to_string(BaseMazeGenerator::min_height)
            + ", grid of size " + std::to_string(grid.width) + "x" + std::to_string(grid.height) + " was provided."
        );
    }

    if (!grid.isInBounds(start) || !grid.isInBounds(goal))
    {
        throw std::invalid_argument(
            "Maze Generator exception: Cannot generate maze. Provided start and/or goal are invalid.\nStart: "
            + std::to_string(start) + "; Goal: " + std::to_string(goal) + "; Grid Size is " + std::to_string(grid.width)
            + "x" + std::to_string(grid.height) + "."
        );
    }
}

std::mt19937 BaseMazeGenerator::getRandomGenerator()
{
    std::random_device        rd;
    std::mt19937::result_type seed
        = rd()
          ^ ((std::mt19937::result_type
             )std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())
                 .count()
             + (std::mt19937::result_type)std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch()
             )
                   .count());

    return std::mt19937(seed);
}
