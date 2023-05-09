#include "data_structure/grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    // set entire grid to wall
    this->grid.reserve(std::max(0, height));

    for (int i = 0; i < height; i++) {
        this->grid.push_back(std::vector<Grid::CellType>(std::max(0, width),
                                                         Grid::CellType::WALL));
    }
}

bool Grid::in_bounds(Grid::Location location) const {
    return 0 <= location.x && location.x < width && 0 <= location.y &&
           location.y < height;
}

bool Grid::passable(Grid::Location location) const {
    return this->grid[location.y][location.x] == Grid::CellType::EMPTY;
}

std::vector<Grid::Location> Grid::neighbors(Grid::Location location,
                                            int distance, bool passable) const {
    std::vector<Grid::Location> result;

    for (Grid::Location direction : this->DIRECTIONS) {
        Grid::Location next{
            location.x + direction.x + (distance * direction.x),
            location.y + direction.y + (distance * direction.y)};
        if (this->in_bounds(next) && !(passable ^ this->passable(next))) {
            result.push_back(next);
        }
    }

    // makes paths look better
    if ((location.x + location.y) % 2 == 0) {
        std::reverse(result.begin(), result.end());
    }

    return result;
};

Grid::cost_t Grid::cost(Grid::Location from_node, Grid::Location to_node) {
    // grid will not have different costs
    (void)from_node;
    (void)to_node;
    return 1;
}

Grid::cost_t Grid::heuristic(Grid::Location a, Grid::Location b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
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

bool Grid::Location::operator>(const Grid::Location &other) const {
    if (this->x == other.x) {
        return this->y > other.y;
    } else {
        return this->x > other.x;
    }
}
