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
        {"p", "parallel", false, "Toggle path parallel draw"}};

    CmdOptions cmd(argc, argv);

    if (cmd.isOptionExists(options[0])) {
        cmd.printHelp(options);
    }

    int traverse_delay = 40, step_delay = 1;
    bool is_parallel = false;

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

    if (cmd.isOptionExists(options[3])) {
        is_parallel = true;
    }

    std::vector<std::string> titles = {"Dijkstra Algorithm", "A* Algorithm"};

    GridRenderer renderer(titles, traverse_delay, step_delay);

    Grid grid(renderer.grid_width, renderer.grid_height);

    Grid::Location start{1, 1};
    Grid::Location end{grid.width - 1, grid.height - 2};

    std::vector<Grid::ChangeRecord> maze_record;
    GridMazeGenerator::generate(grid, start, end, maze_record);

    std::vector<std::vector<Grid::ChangeRecord>> traversed(3);
    std::vector<std::vector<Grid::Location>> path(3);

    path[0] = DijkstraSearch<Grid>::search(grid, start, end, traversed[0]);
    path[1] = AStarSearch<Grid>::search(grid, start, end, Grid::heuristic,
                                        traversed[1]);

    renderer.drawMazes(maze_record);
    getch();

    renderer.drawPath(is_parallel, titles, traversed, path);
    getch();

    return 0;
}
