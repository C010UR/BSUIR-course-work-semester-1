#pragma once

#include <ncurses.h>

#include <algorithm>
#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "graph/grid.h"
#include "utility/timer.h"

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
        PATHFINDER_TRAVERSED,
        PATHFINDER_SEARCHED,
    };

    static const int MIN_GRID_HEIGHT = 7;
    static const int MIN_GRID_WIDTH = 41;
    static const int WINDOW_SPACING = 1;
    static const int MIN_STATUS_WIDTH = 38;

    WINDOW *first_grid;
    WINDOW *second_grid;
    WINDOW *first_grid_status;
    WINDOW *second_grid_status;

    int grid_height;
    int grid_width;

    ~Renderer();
    Renderer();

    WINDOW *createWindow(int height, int width, int start_x, int start_y,
                         std::string title = "");
    void destroyWindow(WINDOW *window);
    void drawMaze(std::vector<Grid::Record> path);
    void drawPath(std::vector<Grid::Location> first_searched,
                  std::unordered_map<Grid::Location, Grid::cost_t> first_cost,
                  std::vector<Grid::Location> second_searched,
                  std::unordered_map<Grid::Location, Grid::cost_t> second_cost);
    void drawFinalPath(std::vector<Grid::Location> first,
                       std::vector<Grid::Location> second);

   private:
    void fillWindow(WINDOW *window, int attributes, char ch);
    void updateMazeCharacter(WINDOW *window, int attributes,
                             Grid::Location location);
    void clearWindow(WINDOW *window);

    void drawMazeStep(WINDOW *grid, WINDOW *status, int step,
                      std::chrono::microseconds time, Grid::Location current,
                      std::optional<Grid::Location> previous);
    void drawPathStep(WINDOW *grid, WINDOW *status, int step, Grid::cost_t cost,
                      Grid::Location current,
                      std::optional<Grid::Location> previous);
    void drawFinalPathStep(WINDOW *grid, Grid::Location current,
                           std::optional<Grid::Location> previous);
};
