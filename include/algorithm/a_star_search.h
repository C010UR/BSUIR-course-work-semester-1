#pragma once

#include <vector>

#include "data_structure/priority_queue.h"
#include "utility/timer.h"

template <typename Graph>
class AStarSearch {
   private:
    typedef typename Graph::Location Location;
    typedef typename Graph::cost_t cost_t;
    typedef typename Graph::ChangeRecord ChangeRecord;

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
    static std::vector<Location> search(
        Graph &graph, Location start, Location goal,
        std::function<cost_t(Location, Location)> heuristic,
        std::vector<ChangeRecord> &record) {
        std::unordered_map<Location, cost_t> cost_so_far;
        PriorityQueue<Location, cost_t> frontier;
        std::vector<Location> neighbors;
        std::unordered_map<Location, Location> came_from;
        frontier.put(start, cost_t(0));

        came_from[start] = start;
        cost_so_far[start] = cost_t(0);
        Timer timer;

        while (!frontier.empty()) {
            Location current = frontier.get();
            timer.tock();
            record.push_back(
                {current, timer.duration(), 0, cost_so_far[current]});

            if (current == goal) {
                break;
            }

            neighbors = graph.neighbors(current);
            for (Location next : neighbors) {
                cost_t new_cost =
                    cost_so_far[current] + graph.cost(current, next);
                if (cost_so_far.find(next) == cost_so_far.end() ||
                    new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    cost_t priority = new_cost + heuristic(next, goal);
                    frontier.put(next, priority);
                    came_from[next] = current;
                }
            }
        }

        return AStarSearch::reconstruct_path(start, goal, came_from);
    }

    /**
     * @brief Reconstruct path from `start` to `goal`
     *
     * @param start - start position
     * @param goal - end position
     * @param came_from - map of ways to location
     *
     * @return std::vector<Location>
     */
    static std::vector<Location> reconstruct_path(
        Location start, Location goal,
        std::unordered_map<Location, Location> &came_from) {
        std::vector<Location> path;
        Location current = goal;

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
