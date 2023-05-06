#pragma once

#include <functional>
#include <tuple>
#include <istream>

struct GridLocation {
    int x, y;
};

namespace std {
/* implement hash function so I can put GridLocation into an unordered_set */
template <>
struct hash<GridLocation> {
    std::size_t operator()(const GridLocation &id) const noexcept {
        return std::hash<int>()(id.x ^ (id.y << 16));
    }
};
} // namespace std

// Comparison operators
bool operator==(GridLocation a, GridLocation b);
bool operator!=(GridLocation a, GridLocation b);
bool operator<(GridLocation a, GridLocation b);
bool operator>(GridLocation a, GridLocation b);

// Stream operators
std::basic_iostream<char>::basic_ostream &operator<<(std::basic_iostream<char>::basic_ostream &out, const GridLocation &loc);
