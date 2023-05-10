#pragma once

#include <functional>
#include <queue>
#include <vector>

#include "algorithm/pathfinder/base_path_finder.h"
#include "utility/timer.h"

template <typename Graph>
class BreadthFirstSearch : BasePathFinder<Graph> {
   private:
   public:
    /**
     * @brief Search a path from `start` to `goal` in a graph
     *
     * @param graph - graph to search
     * @param start
     * @param goal
     * @param heuristic - heuristic to determine distance from the goal
     * @param record - list of steps taken by the algorithm. Saves location
     * (`Location`),time taken (`std::chrono::microseconds`) , and a cost of
     * location (`Graph::cost_t`)
     *
     * @return std::vector<Location> - path from `start` to `goal`
     */
    static std::vector<typename Graph::Location> search(
        const Graph &graph, const typename Graph::Location &start,
        const typename Graph::Location &goal,
        std::vector<typename Graph::ChangeRecord> &record) {
        std::queue<typename Graph::Location> frontier;
        std::unordered_map<typename Graph::Location, typename Graph::Location>
            came_from;

        frontier.push(start);
        came_from[start] = start;

        Timer timer;

        while (!frontier.empty()) {
            typename Graph::Location current = frontier.front();
            frontier.pop();

            timer.tock();
            record.push_back({current, timer.duration(), 0, 0});

            if (current == goal) {
                break;
            }

            for (typename Graph::Location next : graph.neighbors(current)) {
                if (came_from.find(next) == came_from.end()) {
                    frontier.push(next);
                    came_from[next] = current;
                }
            }
        }

        return BreadthFirstSearch::reconstruct_path(start, goal, came_from);
    }
};
