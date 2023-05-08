#pragma once

#include <graph/grid.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

class MazeGenerator {
   public:
    static std::vector<Grid::Location> generate(Grid grid, Grid::Location start,
                                                Grid::Location end);
};
