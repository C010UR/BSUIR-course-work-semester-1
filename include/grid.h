#pragma once

#include <vector>
#include <unordered_set>
#include <algorithm>
#include "grid_location.h"

class Grid {
  public:
    static const std::array<GridLocation, 4> DIRS;

    int width, height;
    std::unordered_set<GridLocation> walls;

    Grid(int width, int _height);
    bool in_bounds(GridLocation id) const;
    bool passable(GridLocation id) const;
    std::vector<GridLocation> neighbors(GridLocation id) const;
};
