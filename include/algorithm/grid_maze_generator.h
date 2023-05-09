#pragma once

#include "data_structure/grid.h"
#include "utility/timer.h"

#include <random>
#include <stack>

class GridMazeGenerator {
   public:
    static void generate(Grid &grid, Grid::Location start, Grid::Location end,
                         std::vector<Grid::ChangeRecord> &record);
};
