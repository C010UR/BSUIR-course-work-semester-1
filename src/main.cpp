#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <queue>
#include <vector>
#include <iomanip>
#include "grid_location.h"
#include "grid.h"

template <class Graph>
void draw_grid(const Graph &graph, std::unordered_map<GridLocation, double> *distances = nullptr,
               std::unordered_map<GridLocation, GridLocation> *point_to = nullptr, std::vector<GridLocation> *path = nullptr,
               GridLocation *start = nullptr, GridLocation *goal = nullptr) {
    const int field_width = 3;
    std::cout << std::string(field_width * graph.width, '_') << '\n';
    for (int y = 0; y != graph.height; ++y) {
        for (int x = 0; x != graph.width; ++x) {
            GridLocation id{x, y};
            if (graph.walls.find(id) != graph.walls.end()) {
                std::cout << std::string(field_width, '#');
            } else if (start && id == *start) {
                std::cout << " A ";
            } else if (goal && id == *goal) {
                std::cout << " Z ";
            } else if (path != nullptr && find(path->begin(), path->end(), id) != path->end()) {
                std::cout << " @ ";
            } else if (point_to != nullptr && point_to->count(id)) {
                GridLocation next = (*point_to)[id];
                if (next.x == x + 1) {
                    std::cout << " > ";
                } else if (next.x == x - 1) {
                    std::cout << " < ";
                } else if (next.y == y + 1) {
                    std::cout << " v ";
                } else if (next.y == y - 1) {
                    std::cout << " ^ ";
                } else {
                    std::cout << " * ";
                }
            } else if (distances != nullptr && distances->count(id)) {
                std::cout << ' ' << std::left << std::setw(field_width - 1) << (*distances)[id];
            } else {
                std::cout << " . ";
            }
        }
        std::cout << '\n';
    }
    std::cout << std::string(field_width * graph.width, '~') << '\n';
}

void add_rect(Grid &grid, int x1, int y1, int x2, int y2) {
    for (int x = x1; x < x2; ++x) {
        for (int y = y1; y < y2; ++y) {
            grid.walls.insert(GridLocation{x, y});
        }
    }
}

Grid make_diagram1() {
    Grid grid(30, 15);
    add_rect(grid, 3, 3, 5, 12);
    add_rect(grid, 13, 4, 15, 15);
    add_rect(grid, 21, 0, 23, 7);
    add_rect(grid, 23, 5, 26, 7);
    return grid;
}

template<typename Location, typename Graph>
std::unordered_map<Location, Location>
breadth_first_search(Graph graph, Location start, Location goal) {
  std::queue<Location> frontier;
  frontier.push(start);

  std::unordered_map<Location, Location> came_from;
  came_from[start] = start;

  while (!frontier.empty()) {
    Location current = frontier.front();
    frontier.pop();

    if (current == goal) {
        break;
    }

    for (Location next : graph.neighbors(current)) {
      if (came_from.find(next) == came_from.end()) {
        frontier.push(next);
        came_from[next] = current;
      }
    }
  }
  return came_from;
}

int main(void) {

    Grid grid = make_diagram1();
    GridLocation start{8, 7}, goal{17, 2};
    auto parents = breadth_first_search(grid, start, goal);
    draw_grid(grid, nullptr, &parents, nullptr, &start, &goal);
}
