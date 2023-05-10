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
#include "utility/cmd_options.h"

int main(int argc, char **argv) {
    std::vector<CmdOptions::Option> options = {
        {"h", "help", false, "Show this help message"},
        {"t", "traverse-delay", true,
         "Set path traverse step (in milliseconds)"},
        {"d", "step-delay", true, "Set step delay (in milliseconds)"},
    };

    CmdOptions cmd(argc, argv);

    if (cmd.isOptionExists(options[0])) {
        cmd.printHelp(options);
    }

    int traverse_delay = 40, step_delay = 1;

    std::string option_value = cmd.getOptionValue(options[1]);

    if (!option_value.empty() &&
        option_value.find_first_not_of("0123456789") == std::string::npos) {
        traverse_delay = std::stoi(option_value);
    }

    option_value = cmd.getOptionValue(options[2]);

    if (!option_value.empty() &&
        option_value.find_first_not_of("0123456789") == std::string::npos) {
        step_delay = std::stoi(option_value);
    }

    std::string dijkstra_title = "Dijkstra Algorithm";
    std::string a_star_title = "A* Algorithm";

    GridRenderer renderer({dijkstra_title, a_star_title}, traverse_delay,
                          step_delay);

    Grid grid(renderer.grid_width, renderer.grid_height);

    Grid::Location start{1, 1};
    Grid::Location end{grid.width - 1, grid.height - 2};

    std::vector<Grid::ChangeRecord> maze_record;
    GridMazeGenerator::generate(grid, start, end, maze_record);
    renderer.drawMazes(maze_record);
    getch();

    std::vector<Grid::ChangeRecord> dijkstra_traversed;
    auto dijkstra_path =
        DijkstraSearch<Grid>::search(grid, start, end, dijkstra_traversed);

    GridRenderer::GridWindow dijkstra_window =
        renderer.findWindow(dijkstra_title);
    auto dijkstra_info =
        renderer.drawTraversedPath(dijkstra_window, dijkstra_traversed);
    renderer.drawFinalPath(dijkstra_window, dijkstra_info, dijkstra_path);

    std::vector<Grid::ChangeRecord> a_star_traversed;
    auto a_star_path = AStarSearch<Grid>::search(
        grid, start, end, Grid::heuristic, a_star_traversed);

    GridRenderer::GridWindow a_star_window = renderer.findWindow(a_star_title);
    auto a_star_info =
        renderer.drawTraversedPath(a_star_window, a_star_traversed);
    renderer.drawFinalPath(a_star_window, a_star_info, a_star_path);

    getch();

    return 0;
}
