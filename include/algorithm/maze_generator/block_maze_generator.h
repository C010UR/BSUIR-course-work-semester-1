#pragma once

#include <chrono>
#include <random>
#include <stdexcept>
#include <string>

#include "algorithm/maze_generator/base_maze_generator.h"
#include "data_structure/grid.h"
#include "utility/timer.h"

class BlockMazeGenerator : BaseMazeGenerator
{
  public:
    /**
     * @brief Generate a maze from `start` to `goal`
     *
     * @param grid - grid in which the maze will be generated
     * @param start
     * @param goal
     * @param record - list of steps taken by the algorithm. Saves location
     * (`Location`) and time taken (`std::chrono::microseconds`)
     */
    void generate(
        Grid &grid, const Grid::Location &start, const Grid::Location &goal, std::vector<Grid::ChangeRecord> &record
    );

  private:
    void removeWall(
        Grid                                                                 &grid,
        Grid::Location                                                        from,
        Grid::Location                                                        to,
        std::vector<Grid::ChangeRecord>                                      &record,
        Timer<std::chrono::microseconds, std::chrono::high_resolution_clock> &timer,
        unsigned                                                             &step
    );
};
