#pragma once

#include <graph/grid.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "utility/timer.h"
class MazeGenerator {
   public:
    static void generate(Grid &grid, Grid::Location start, Grid::Location end,
                         std::vector<Grid::Record> &record);
};
