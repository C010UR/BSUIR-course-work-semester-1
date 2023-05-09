#include <ncurses.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "algorithm/a_star_search.h"
#include "algorithm/dijkstra_search.h"
#include "algorithm/grid_maze_generator.h"
#include "data_structure/grid.h"
#include "renderer/grid_renderer.h"

int main() {
    std::string dijkstra_title = "Dijkstra Algorithm";
    std::string a_star_title = "A* Algorithm";

    GridRenderer renderer({dijkstra_title, a_star_title});

    Grid grid(renderer.grid_width, renderer.grid_height);

    Grid::Location start{1, 1};
    Grid::Location end{grid.width - 1, grid.height - 2};

    std::vector<Grid::ChangeRecord> maze_record;
    GridMazeGenerator::generate(grid, start, end, maze_record);
    renderer.drawMazes(maze_record);
    getch();

    std::vector<Grid::ChangeRecord> dijkstra_traversed;
    std::unordered_map<Grid::Location, Grid::cost_t> dijkstra_cost;
    auto dijkstra_path = DijkstraSearch<Grid>::search(
        grid, start, end, dijkstra_traversed, dijkstra_cost);

    GridRenderer::GridWindow dijkstra_window =
        renderer.findWindow(dijkstra_title);
    auto dijkstra_info = renderer.drawTraversedPath(
        dijkstra_window, dijkstra_traversed, dijkstra_cost);
    renderer.drawFinalPath(dijkstra_window, dijkstra_info, dijkstra_path);

    std::vector<Grid::ChangeRecord> a_star_traversed;
    std::unordered_map<Grid::Location, Grid::cost_t> a_star_cost;
    auto a_star_path = AStarSearch<Grid>::search(
        grid, start, end, Grid::heuristic, a_star_traversed, a_star_cost);

    GridRenderer::GridWindow a_star_window = renderer.findWindow(a_star_title);
    auto a_star_info = renderer.drawTraversedPath(
        a_star_window, a_star_traversed, a_star_cost);
    renderer.drawFinalPath(a_star_window, a_star_info, a_star_path);

    getch();

    return 0;
}
