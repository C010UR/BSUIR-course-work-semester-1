#include <ncurses.h>

#include <iostream>
#include <vector>

#include "graph/grid.h"
#include "graph/maze_generator.h"
#include "renderer/renderer.h"

int main() {
    Renderer renderer;

    Grid grid(renderer.grid_width, renderer.grid_height);
    std::vector<Grid::Location> maze_path;

    MazeGenerator::generate(grid, {1, 1}, {grid.width - 1, grid.height - 2},
                            maze_path);

    renderer.drawMaze(maze_path);
    getch(); /* Wait for user input */

    return 0;
}

// #include <windows.h>

// #include <cstdlib>
// #include <iostream>

// #include "graph/grid.h"
// #include "graph/maze_generator.h"

// int main(void) {
//     int height = 11;

//     Grid grid(41, height);
//     std::vector<Grid::Location> history = MazeGenerator::generate(
//         grid, Grid::Location{1, 1}, Grid::Location{40, 3});

//     ShowCursor(false);

//     std::cout << "\033[?25l"
//               << "Dijkstra algorithm:"
//               << "\n";

//     for (Grid::Location change : history) {
//         grid.grid[change.y][change.x] = Grid::CellType::EMPTY;

//         for (int i = 0; i < grid.height; i++) {
//             for (int j = 0; j < grid.width; j++) {
//                 std::cout << (grid.grid[i][j] == Grid::CellType::WALL
//                                   ? "\033[8;47m##\033[0m"
//                                   : "  ");
//             }
//             std::cout << "\n";
//         }

//         std::cout << "\n\n"
//                   << "A* algorithm:"
//                   << "\n";

//         for (int i = 0; i < grid.height; i++) {
//             for (int j = 0; j < grid.width; j++) {
//                 std::cout << (grid.grid[i][j] == Grid::CellType::WALL
//                                   ? "\033[8;47m##\033[0m"
//                                   : "  ");
//             }

//             std::cout << "\n";
//         }

//         std::cout << std::endl << "\033[" << height * 2 + 4 << "F";
//     }
//     std::cout << "\033[" << height * 2 + 4 << "E";
//     std::cout << "\033[?25h";

//     return EXIT_SUCCESS;
// }
