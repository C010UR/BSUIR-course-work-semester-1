#include <ncurses.h>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "graph/grid.h"
#include "graph/maze_generator.h"
#include "path_finder/a_star_search.h"
#include "path_finder/dijkstra_search.h"
#include "renderer/renderer.h"

int main() {
    Renderer renderer;

    Grid grid(renderer.grid_width, renderer.grid_height);

    Grid::Location start{1, 1};
    Grid::Location end{grid.width - 1, grid.height - 2};

    std::vector<Grid::Record> maze_path;
    MazeGenerator::generate(grid, start, end, maze_path);
    renderer.drawMaze(maze_path);

    std::vector<Grid::Record> dijkstra_searched;
    std::unordered_map<Grid::Location, Grid::cost_t> dijkstra_cost;
    std::vector<Grid::Location> dijkstra_path = DijkstraSearch<Grid>::search(
        grid, start, end, dijkstra_searched, dijkstra_cost);

    std::vector<Grid::Record> a_star_searched;
    std::unordered_map<Grid::Location, Grid::cost_t> a_star_cost;
    std::vector<Grid::Location> a_star_path = AStarSearch<Grid>::search(
        grid, start, end, Grid::heuristic, a_star_searched, a_star_cost);

    renderer.drawPath(dijkstra_searched, dijkstra_cost, a_star_searched,
                      a_star_cost);

    renderer.drawFinalPath(dijkstra_path, a_star_path);

    getch();

    return 0;
}
