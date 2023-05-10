#include "data_structure/grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    // set entire grid to wall
    this->grid.reserve(std::max(0, height));

    for (int i = 0; i < height; i++) {
        this->grid.push_back(std::vector<Grid::CellType>(std::max(0, width),
                                                         Grid::CellType::WALL));
    }
}

bool Grid::isInBounds(Grid::Location location) const {
    return 0 <= location.x && location.x < width && 0 <= location.y &&
           location.y < height;
}

bool Grid::isPassable(Grid::Location location) const {
    return this->grid[location.y][location.x] == Grid::CellType::EMPTY;
}

std::vector<Grid::Location> Grid::neighbors(Grid::Location location,
                                            int distance,
                                            bool is_passable) const {
    std::vector<Grid::Location> result;

    for (Grid::Location direction : this->DIRECTIONS) {
        Grid::Location next{
            location.x + direction.x + (distance * direction.x),
            location.y + direction.y + (distance * direction.y)};
        if (this->isInBounds(next) && !(is_passable ^ this->isPassable(next))) {
            result.push_back(next);
        }
    }

    // makes paths look better
    if ((location.x + location.y) % 2 == 0) {
        std::reverse(result.begin(), result.end());
    }

    return result;
}

Grid::cost_t Grid::cost(Grid::Location from, Grid::Location to) {
    // grid will not have different costs
    (void)from;
    (void)to;
    return 1;
}

Grid::cost_t Grid::heuristic(Grid::Location from, Grid::Location to) {
    return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}

Grid::CellType &Grid::operator[](Grid::Location location) {
    return this->grid[location.y][location.x];
}

// Location
bool Grid::Location::operator==(const Grid::Location &other) const {
    return this->x == other.x && this->y == other.y;
}

bool Grid::Location::operator!=(const Grid::Location &other) const {
    return this->x != other.x || this->y != other.y;
}

bool Grid::Location::operator<(const Grid::Location &other) const {
    if (this->x == other.x) {
        return this->y < other.y;
    } else {
        return this->x < other.x;
    }
}
