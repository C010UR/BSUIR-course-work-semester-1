#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

template <typename Graph>
class BasePathFinder {
   public:
    /**
     * @brief Reconstruct path from `start` to `goal`
     *
     * @param start - start position
     * @param goal - end position
     * @param came_from - map of ways to location
     *
     * @return std::vector<Location>
     */
    static std::vector<typename Graph::Location> reconstruct_path(
        const typename Graph::Location &start,
        const typename Graph::Location &goal,
        std::unordered_map<typename Graph::Location,
                                 typename Graph::Location> &came_from) {
        std::vector<typename Graph::Location> path;
        typename Graph::Location current = goal;

        if (came_from.find(goal) == came_from.end()) {
            return path;  // no path can be found
        }

        while (current != start) {
            path.push_back(current);
            current = came_from[current];
        }

        path.push_back(start);

        // reconstructed path will start from end, so reverse
        std::reverse(path.begin(), path.end());

        return path;
    }
};
