#include <ncurses.h>
#include <stdlib.h>

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include "algorithm/grid_maze_generator.h"
#include "algorithm/pathfinder/a_star_search.h"
#include "algorithm/pathfinder/breadth_first_search.h"
#include "algorithm/pathfinder/dijkstra_search.h"
#include "data_structure/grid.h"
#include "renderer/grid_renderer.h"
#include "utility/terminal.h"

int main(int argc, char **argv)
{
    Terminal terminal(argc, argv);

    // global catch to edit exception error output
    try
    {
        unsigned traverse_delay, step_delay;

        // options
        traverse_delay = terminal.getOptionValue<unsigned>(terminal.options[Terminal::Options::TRAVERSE_DELAY], 40);

        step_delay = terminal.getOptionValue<unsigned>(terminal.options[Terminal::Options::TRAVERSE_DELAY], 1);

        bool is_parallel = false;

        if (terminal.isOptionExists(terminal.options[Terminal::Options::PARALLEL]))
        {
            is_parallel = true;
        }

        int algorithms_amount
            = terminal.isOptionExists(terminal.options[Terminal::Options::BREADTH_FIRST_SEARCH_ALGORITHM])
              + terminal.isOptionExists(terminal.options[Terminal::Options::DIJKSTRA_ALGORITHM])
              + terminal.isOptionExists(terminal.options[Terminal::Options::A_STAR_ALGORITHM]);

        if (algorithms_amount == 0)
        {
            throw std::invalid_argument(
                "Argument exception: Cannot start a program. Must include at least one algorithm."
            );
        }

        GridRenderer renderer(algorithms_amount, traverse_delay, step_delay);
        Grid         grid(renderer.grid_width, renderer.grid_height);

        int            start_x = grid.width / 2;
        int            start_y = grid.height / 2;
        Grid::Location start{start_x % 2 == 0 ? ++start_x : start_x, start_y % 2 == 0 ? ++start_y : start_y};
        Grid::Location end{(int)grid.width - 1, (int)grid.height - 2};

        std::vector<Grid::ChangeRecord> maze_record;
        GridMazeGenerator::generate(grid, start, end, maze_record);

        std::vector<std::vector<Grid::ChangeRecord>> traversed;
        std::vector<std::vector<Grid::Location>>     path;

        // algorithms
        std::vector<std::string> titles;

        if (terminal.isOptionExists(terminal.options[Terminal::Options::BREADTH_FIRST_SEARCH_ALGORITHM]))
        {
            titles.push_back("Breadth First Search Algorithm");
            traversed.push_back(std::vector<Grid::ChangeRecord>());
            path.push_back(BreadthFirstSearch<Grid>::search(grid, start, end, traversed.back()));
        }

        if (terminal.isOptionExists(terminal.options[Terminal::Options::DIJKSTRA_ALGORITHM]))
        {
            titles.push_back("Dijkstra Algorithm");
            traversed.push_back(std::vector<Grid::ChangeRecord>());
            path.push_back(DijkstraSearch<Grid>::search(grid, start, end, traversed.back()));
        }

        if (terminal.isOptionExists(terminal.options[Terminal::Options::A_STAR_ALGORITHM]))
        {
            titles.push_back("A* Algorithm");
            traversed.push_back(std::vector<Grid::ChangeRecord>());
            path.push_back(AStarSearch<Grid>::search(grid, start, end, Grid::heuristic, traversed.back()));
        }

        renderer.createWindows(titles);

        renderer.drawMazes(maze_record);
        getch();

        renderer.drawPath(is_parallel, titles, traversed, path);
        getch();

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        terminal.error(e.what());
        return EXIT_FAILURE;
    }
}
