#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <vector>

class Grid
{
  public:
    /** Type of cost of the cell */
    typedef unsigned cost_t;

    /** Position of each cell in a grid */
    class Location
    {
      public:
        int x;
        int y;

        bool operator==(const Location &other) const;
        bool operator!=(const Location &other) const;
        bool operator<(const Location &other) const;
    };

    /** Record of a change in a grid */
    struct ChangeRecord
    {
        Grid::Location            location;
        std::chrono::microseconds time_taken;
        size_t                    step = 0; // used by GridRenderer
        Grid::cost_t              cost = 0;
    };

    enum CellType
    {
        EMPTY,
        WALL
    };

    static const std::array<Grid::Location, 4> directions;

    size_t                                   width, height;
    std::vector<std::vector<Grid::CellType>> grid;

    /**
     * @brief Construct a new Grid object with dimensions `width` x `height` of
     * cell type `WALL`
     *
     * @param width
     * @param height
     */
    Grid(const size_t width, const size_t height);

    /**
     * @brief Check if `location` is in bounds
     *
     * @param location
     * @return true
     * @return false
     */
    bool isInBounds(const Grid::Location &location) const;

    /**
     * @brief Check if `location` is passable
     *
     * @param location
     * @return true
     * @return false
     */
    bool isPassable(const Grid::Location &location) const;

    /**
     * @brief Find all passable (or not passable) neighbors within a given
     * distance
     *
     * @param location
     * @param distance
     * @param passable
     * @return std::vector<Grid::Location>
     */
    std::vector<Grid::Location> neighbors(
        const Grid::Location &location, const unsigned distance = 0, const bool is_passable = true
    ) const;

    /**
     * @brief Return cost of moving from cell `from` to cell `to`
     *
     * @param from
     * @param to
     * @return cost_t
     */
    static cost_t cost(const Grid::Location &from, const Grid::Location &to);

    /**
     * @brief Calculate a distance between locations `from` and `to`
     *
     * @param from
     * @param to
     * @return Grid::cost_t
     */
    static Grid::cost_t heuristic(const Grid::Location &from, const Grid::Location &to);

    Grid::CellType &operator[](const Grid::Location &location);
};

namespace std
{
// Implement hash function so Grid::Location can be put into std::unordered_set
template <> struct hash<Grid::Location>
{
    std::size_t operator()(const Grid::Location &location) const noexcept
    {
        return std::hash<size_t>()(location.x ^ (location.y << 16));
    }
};

// Implement std::to_string function for Grid::Location
inline std::string to_string(const Grid::Location &location) noexcept
{
    return "(" + std::to_string(location.x) + "; " + std::to_string(location.y) + ")";
}
} // namespace std
