#pragma once

#include <functional>
#include <queue>
#include <vector>

#include "algorithm/pathfinder/base_path_finder.h"
#include "data_structure/priority_queue.h"
#include "utility/timer.h"

template <typename Graph>
class DijkstraSearch : BasePathFinder<Graph> {
   private:
   public:
    /**
     * @brief Search a path from `start` to `goal` in a graph
     *
     * @param graph - graph to search
     * @param start
     * @param goal
     * @param record - list of steps taken by the algorithm. Saves location
     * (`Location`), time taken (`std::chrono::microseconds`), and a cost of
     * location (`Graph::cost_t`)
     *
     * @return std::vector<Location> - path from `start` to `goal`
     */
    static std::vector<typename Graph::Location> search(
        const Graph &graph, const typename Graph::Location &start,
        const typename Graph::Location &goal,
        std::vector<typename Graph::ChangeRecord> &record) {
        std::unordered_map<typename Graph::Location, typename Graph::cost_t>
            cost_so_far;
        PriorityQueue<typename Graph::Location, typename Graph::cost_t>
            frontier;
        std::unordered_map<typename Graph::Location, typename Graph::Location>
            came_from;

        frontier.push(start, typename Graph::cost_t(0));
        came_from[start] = start;
        cost_so_far[start] = typename Graph::cost_t(0);

        Timer timer;

        while (!frontier.empty()) {
            typename Graph::Location current = frontier.pop();

            timer.tock();
            record.push_back(
                {current, timer.duration(), 0, cost_so_far[current]});

            if (current == goal) {
                break;
            }

            for (typename Graph::Location next : graph.neighbors(current)) {
                typename Graph::cost_t new_cost =
                    cost_so_far[current] + graph.cost(current, next);
                if (cost_so_far.find(next) == cost_so_far.end() ||
                    new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    came_from[next] = current;
                    frontier.push(next, new_cost);
                }
            }
        }

        return DijkstraSearch::reconstruct_path(start, goal, came_from);
    };
};
