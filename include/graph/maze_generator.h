#pragma once

#include <graph/grid.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <stack>
#include <vector>
#include <chrono>

class MazeGenerator {
   public:
    static void generate(Grid &grid, Grid::Location start, Grid::Location end,
                         std::vector<Grid::Location> &path);
};
