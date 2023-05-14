#include <ncurses.h>
#include <stdlib.h>

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include "algorithm/maze_generator/block_maze_generator.h"
#include "algorithm/maze_generator/depth_first_search_maze_generator.h"
#include "algorithm/pathfinder/a_star_search.h"
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
        // arguments
        unsigned traverse_delay
            = terminal.getOptionValue<unsigned>(terminal.options[Terminal::Options::TRAVERSE_DELAY], 40);
        unsigned step_delay  = terminal.getOptionValue<unsigned>(terminal.options[Terminal::Options::STEP_DELAY], 1);
        bool     is_parallel = terminal.isOptionExists(terminal.options[Terminal::Options::PARALLEL]);

        auto addArgument = [terminal](std::vector<Terminal::Options> &vec, Terminal::Options opt) {
            if (terminal.isOptionExists(terminal.options[opt]))
            {
                vec.push_back(opt);
            }
        };

        std::vector<Terminal::Options> algorithms;

        addArgument(algorithms, Terminal::Options::DIJKSTRA_ALGORITHM);
        addArgument(algorithms, Terminal::Options::A_STAR_ALGORITHM);

        if (algorithms.empty())
        {
            throw std::invalid_argument(
                "Argument exception: Cannot start a program. Must include at least one algorithm."
            );
        }

        std::vector<Terminal::Options> maze_generators;

        addArgument(maze_generators, Terminal::Options::DEPTH_FIRST_SEARCH_MAZE_GENERATOR);
        addArgument(maze_generators, Terminal::Options::BLOCK_MAZE_GENERATOR);

        if (maze_generators.empty())
        {
            throw std::invalid_argument(
                "Argument exception: Cannot start a program. Must include at least one maze generator."
            );
        }

        GridRenderer renderer(algorithms.size(), traverse_delay, step_delay);
        Grid         grid(renderer.grid_width, renderer.grid_height);

        int start_x = grid.width / 2;
        int start_y = grid.height / 2;

        Grid::Location start{start_x % 2 == 0 ? ++start_x : start_x, start_y % 2 == 0 ? ++start_y : start_y};
        Grid::Location end{(int)grid.width - 1, (int)grid.height - 2};

        for (Terminal::Options maze_option : maze_generators)
        {
            std::vector<Grid::ChangeRecord> maze_record;

            DepthFirstSearchMazeGenerator depth_first_search_maze_generator;
            BlockMazeGenerator            block_maze_generator;

            switch (maze_option)
            {
            case Terminal::Options::DEPTH_FIRST_SEARCH_MAZE_GENERATOR:
                depth_first_search_maze_generator.generate(grid, start, end, maze_record);
                break;

            case Terminal::Options::BLOCK_MAZE_GENERATOR:
                block_maze_generator.generate(grid, start, end, maze_record);
                break;

            default:
                continue;
            }

            std::vector<std::string>                     algorithm_indexes;
            std::vector<std::vector<Grid::ChangeRecord>> traversed;
            std::vector<std::vector<Grid::Location>>     path;

            for (Terminal::Options algorithm_option : algorithms)
            {
                switch (algorithm_option)
                {
                case Terminal::Options::DIJKSTRA_ALGORITHM:
                    algorithm_indexes.push_back("Dijkstra Algorithm");
                    traversed.push_back(std::vector<Grid::ChangeRecord>());
                    path.push_back(DijkstraSearch<Grid>::search(grid, start, end, traversed.back()));
                    break;

                case Terminal::Options::A_STAR_ALGORITHM:
                    algorithm_indexes.push_back("A* Algorithm");
                    traversed.push_back(std::vector<Grid::ChangeRecord>());
                    path.push_back(AStarSearch<Grid>::search(grid, start, end, Grid::heuristic, traversed.back()));
                    break;

                default:
                    break;
                }
            }

            renderer.createWindows(algorithm_indexes);

            renderer.drawMazes(maze_record);
            getch();

            renderer.drawPath(is_parallel, algorithm_indexes, traversed, path);
            getch();
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        terminal.error(e.what());
        return EXIT_FAILURE;
    }
}
