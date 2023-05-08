#include "graph/grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    this->grid.reserve(std::max(0, height));

    for (int i = 0; i < height; i++) {
        this->grid.push_back(std::vector<Grid::CellType>(std::max(0, width),
                                                         Grid::CellType::WALL));
    }
}

const std::array<Grid::Location, 4> Grid::DIRECTIONS = {
    // East, West, North, South
    Grid::Location{1, 0}, Grid::Location{-1, 0}, Grid::Location{0, -1},
    Grid::Location{0, 1}};

inline bool Grid::in_bounds(Grid::Location location) const {
    return 0 <= location.x && location.x < width && 0 <= location.y &&
           location.y < height;
}

inline bool Grid::passable(Grid::Location location) const {
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

inline Grid::cost_t Grid::cost(Grid::Location from_node,
                               Grid::Location to_node) {
    // grid will not have different costs
    (void)from_node;
    (void)to_node;
    return 1;
}

inline Grid::cost_t Grid::heuristic(Grid::Location a, Grid::Location b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Comparison operators
bool operator==(Grid::Location a, Grid::Location b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(Grid::Location a, Grid::Location b) { return !(a == b); }

bool operator<(Grid::Location a, Grid::Location b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

bool operator>(Grid::Location a, Grid::Location b) {
    return std::tie(a.x, a.y) > std::tie(b.x, b.y);
}

// Stream operators
std::basic_iostream<char>::basic_ostream &operator<<(
    std::basic_iostream<char>::basic_ostream &out,
    const Grid::Location &location) {
    out << '(' << location.x << ", " << location.y << ')';
    return out;
}
