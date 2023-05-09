#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

class Grid {
   public:
    typedef int cost_t;

    class Location {
       public:
        int x;
        int y;

        bool operator==(const Location &other) const;
        bool operator!=(const Location &other) const;
        bool operator<(const Location &other) const;
        bool operator>(const Location &other) const;
    };

    struct ChangeRecord {
        Grid::Location location;
        std::chrono::microseconds time_taken;
    };

    enum CellType { EMPTY, WALL };

    const std::array<Grid::Location, 4> DIRECTIONS = {
        // East, West, North, South
        Grid::Location{1, 0}, Grid::Location{-1, 0}, Grid::Location{0, -1},
        Grid::Location{0, 1}};

    int width, height;
    std::vector<std::vector<Grid::CellType>> grid;

    Grid(int width, int height);

    bool in_bounds(Grid::Location location) const;
    bool passable(Grid::Location location) const;
    std::vector<Grid::Location> neighbors(Grid::Location location,
                                          int distance = 0,
                                          bool passable = true) const;

    static cost_t cost(Grid::Location from_node, Grid::Location to_node);
    static Grid::cost_t heuristic(Grid::Location a, Grid::Location b);

    Grid::CellType &operator[](Grid::Location location);
};

namespace std {
// implement hash function so I can put Grid::Location into an unordered_set
template <>
struct hash<Grid::Location> {
    std::size_t operator()(const Grid::Location &location) const noexcept {
        return std::hash<int>()(location.x ^ (location.y << 16));
    }
};

// implement to_string function for Location
inline std::string to_string(const Grid::Location &location) noexcept {
    return "(" + std::to_string(location.x) + "; " +
           std::to_string(location.y) + ")";
}
}  // namespace std
