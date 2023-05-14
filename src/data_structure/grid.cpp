#include "data_structure/grid.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <vector>

const std::array<Grid::Location, 4> Grid::directions = {
  // East, West, North, South
    Grid::Location{1,  0 },
    Grid::Location{-1, 0 },
    Grid::Location{0,  -1},
    Grid::Location{0,  1 }
};

Grid::Grid(const size_t width, const size_t height) : width(width), height(height)
{
    // set entire grid to wall
    this->grid.reserve(height);

    for (size_t i = 0; i < height; i++)
    {
        this->grid.push_back(std::vector<Grid::CellType>(width, Grid::CellType::WALL));
    }
}

bool Grid::isInBounds(const Grid::Location &location) const
{
    return 0 <= location.x && (size_t)location.x < width && 0 <= location.y && (size_t)location.y < height;
}

bool Grid::isPassable(const Grid::Location &location) const
{
    return this->grid[location.y][location.x] == Grid::CellType::EMPTY;
}

std::vector<Grid::Location> Grid::neighbors(
    const Grid::Location &location, const unsigned distance, const bool is_passable
) const
{
    std::vector<Grid::Location> result;

    for (Grid::Location direction : this->directions)
    {
        Grid::Location next{
            (int)(location.x + direction.x + (distance * direction.x)),
            (int)(location.y + direction.y + (distance * direction.y))};
        if (this->isInBounds(next) && !(is_passable ^ this->isPassable(next)))
        {
            result.push_back(next);
        }
    }

    // makes paths look better
    if ((location.x + location.y) % 2 == 0)
    {
        std::reverse(result.begin(), result.end());
    }

    return result;
}

Grid::cost_t Grid::cost(const Grid::Location &from, const Grid::Location &to)
{
    // grid will not have different costs
    (void)from;
    (void)to;
    return 1;
}

Grid::cost_t Grid::heuristic(const Grid::Location &from, const Grid::Location &to)
{
    return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}

Grid::CellType &Grid::operator[](const Grid::Location &location)
{
    return this->grid[location.y][location.x];
}

// Location
bool Grid::Location::operator==(const Grid::Location &other) const
{
    return this->x == other.x && this->y == other.y;
}

bool Grid::Location::operator!=(const Grid::Location &other) const
{
    return this->x != other.x || this->y != other.y;
}

bool Grid::Location::operator<(const Grid::Location &other) const
{
    if (this->x == other.x)
    {
        return this->y < other.y;
    }
    else
    {
        return this->x < other.x;
    }
}
