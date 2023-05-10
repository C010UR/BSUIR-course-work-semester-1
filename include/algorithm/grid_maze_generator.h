#pragma once

#include <random>
#include <stack>

#include "data_structure/grid.h"
#include "utility/timer.h"

class GridMazeGenerator {
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
    static void generate(Grid &grid, Grid::Location start, Grid::Location goal,
                         std::vector<Grid::ChangeRecord> &record);
};
