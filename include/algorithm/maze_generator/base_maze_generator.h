#pragma once

#include <random>

#include "data_structure/grid.h"

class BaseMazeGenerator
{
  public:
    static const size_t min_width  = 3;
    static const size_t min_height = 3;

    /**
     * @brief Generate a maze from `start` to `goal`
     *
     * @param grid - grid in which the maze will be generated
     * @param start
     * @param goal
     * @param record - list of steps taken by the algorithm. Saves location
     * (`Location`) and time taken (`std::chrono::microseconds`)
     */
    virtual void generate(
        Grid &grid, const Grid::Location &start, const Grid::Location &goal, std::vector<Grid::ChangeRecord> &record
    ) = 0;

    /**
     * @brief Validate whether `grid` has proper dimensions and `start` and `goal` are inbounds
     *
     * @param grid
     * @param start
     * @param goal
     */
    void validateArguments(const Grid &grid, const Grid::Location &start, const Grid::Location &goal);

    /**
     * @brief Get the Random Generator
     *
     * @return std::mt19937
     */
    std::mt19937 getRandomGenerator();
};
