#include "grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
}

const std::array<GridLocation, 4> Grid::DIRS = {
    // East, West, North, South
    GridLocation{1, 0}, GridLocation{-1, 0}, GridLocation{0, -1}, GridLocation{0, 1}};

bool Grid::in_bounds(GridLocation id) const {
    return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
}

bool Grid::passable(GridLocation id) const {
    return this->walls.find(id) == this->walls.end();
}

std::vector<GridLocation> Grid::neighbors(GridLocation id) const {
    std::vector<GridLocation> result;

    for (GridLocation dir : DIRS) {
        GridLocation next{id.x + dir.x, id.y + dir.y};
        if (this->in_bounds(next) && this->passable(next)) {
            result.push_back(next);
        }
    }

    // makes paths look better
    if ((id.x + id.y) % 2 == 0) {
        std::reverse(result.begin(), result.end());
    }

    return result;
};
