#pragma once

#include <vector>

#include "data_structure/priority_queue.h"
#include "utility/timer.h"

template <typename Graph>
class DijkstraSearch {
   private:
    typedef typename Graph::Location Location;
    typedef typename Graph::cost_t cost_t;
    typedef typename Graph::Record Record;

   public:
    static std::vector<Location> search(
        Graph &graph, Location start, Location goal,
        std::vector<Record> &record,
        std::unordered_map<Location, cost_t> &cost_so_far) {
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
            record.push_back(Record{current, timer.duration()});

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
                    came_from[next] = current;
                    frontier.put(next, new_cost);
                }
            }
        }

        return DijkstraSearch::reconstruct_path(start, goal, came_from);
    };

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
        std::reverse(path.begin(), path.end());

        return path;
    }
};
