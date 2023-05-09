#pragma once

#include <ncurses.h>

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "graph/grid.h"

class Renderer {
   public:
    enum ColorPairs {
        TEXT,
        VALUE,
        WALL,
        EMPTY,
        MAZE_CURRENT,
        MAZE_TRAVERSED,
        PATHFINDER_CURRENT,
        PATHFINDER_TRAVERSED
    };

    static const int MIN_GRID_HEIGHT = 7;
    static const int MIN_GRID_WIDTH = 41;
    static const int WINDOW_SPACING = 1;
    static const int MIN_STATUS_WIDTH = 38;
    static const int MAZE_DRAW_TIME_MS = 3000;

    WINDOW *first_grid;
    WINDOW *second_grid;
    WINDOW *first_grid_status;
    WINDOW *second_grid_status;

    int grid_height;
    int grid_width;

    ~Renderer();
    Renderer();

    WINDOW *createWindow(int height, int width, int start_x, int start_y);
    void destroyWindow(WINDOW *window);
    void drawMaze(std::vector<Grid::Location> path);

   private:
    void fillWindow(WINDOW *window, int attributes, char ch);
    void updateCharacter(WINDOW *window, int attributes, char ch, int x, int y);
};
