#pragma once

#include <algorithm>
#include <functional>
#include <istream>
#include <ratio>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <chrono>

struct Location {
    int x;
    int y;
};

struct Record {
    Location location;
    std::chrono::microseconds time;
};

class Grid {
   public:
    typedef Location Location;
    typedef Record Record;
    typedef int cost_t;
    enum CellType { EMPTY, WALL };

    static const std::array<Grid::Location, 4> DIRECTIONS;

    int width, height;
    std::vector<std::vector<Grid::CellType>> grid;

    Grid(int width, int height);
    inline bool in_bounds(Grid::Location location) const;
    inline bool passable(Grid::Location location) const;
    std::vector<Grid::Location> neighbors(Grid::Location location,
                                          int distance = 0,
                                          bool passable = true) const;

    static cost_t cost(Grid::Location from_node, Grid::Location to_node);
    static Grid::cost_t heuristic(Grid::Location a, Grid::Location b);
};

namespace std {
/* implement hash function so I can put Grid::Location into an unordered_set */
template <>
struct hash<Location> {
    std::size_t operator()(const Grid::Location &location) const noexcept {
        return std::hash<int>()(location.x ^ (location.y << 16));
    }
};
}  // namespace std

// Comparison operators
bool operator==(Grid::Location a, Grid::Location b);
bool operator!=(Grid::Location a, Grid::Location b);
bool operator<(Grid::Location a, Grid::Location b);
bool operator>(Grid::Location a, Grid::Location b);

// Stream operators
std::basic_iostream<char>::basic_ostream &operator<<(
    std::basic_iostream<char>::basic_ostream &out,
    const Grid::Location &location);
