#pragma once

#include <random>
#include <stack>

#include "data_structure/grid.h"
#include "utility/timer.h"

class GridMazeGenerator {
   public:
    static void generate(Grid &grid, Grid::Location start, Grid::Location end,
                         std::vector<Grid::ChangeRecord> &record);
};
