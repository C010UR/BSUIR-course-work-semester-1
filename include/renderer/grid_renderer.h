#pragma once

#include <ncurses.h>

#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "data_structure/grid.h"
#include "renderer/renderer.h"

class GridRenderer : Renderer {
   public:
    static const int MIN_GRID_HEIGHT = 7;
    static const int MIN_GRID_WIDTH = 41;
    static const int MIN_STATUS_WIDTH = 38;

    struct GridWindow {
        std::string title;
        WINDOW *grid;
        WINDOW *status;
    };

    struct Information {
        int step;
        std::chrono::microseconds time_taken;
        Grid::cost_t cost;
    };

    int grid_height;
    int grid_width;

    GridRenderer(std::vector<std::string> titles);

    static void updateGridCell(WINDOW *window, attr_t attribute,
                               Grid::Location location);

    GridRenderer::GridWindow findWindow(std::string name);

    void drawMazes(const std::vector<Grid::ChangeRecord> &maze_record);
    GridRenderer::Information drawTraversedPath(
        GridRenderer::GridWindow window,
        const std::vector<Grid::ChangeRecord> &traversed,
        const std::unordered_map<Grid::Location, Grid::cost_t> &cost);
    void drawFinalPath(GridRenderer::GridWindow window,
                       GridRenderer::Information information,
                       const std::vector<Grid::Location> &path);

   private:
    std::vector<GridWindow> windows;

    void updateMaze(bool is_end, GridRenderer::GridWindow window,
                    GridRenderer::Information information,
                    Grid::Location current,
                    std::optional<Grid::Location> previous = std::optional<Grid::Location>());

    void updateTraversedPath(bool is_end, GridRenderer::GridWindow window,
                            GridRenderer::Information information,
                            Grid::Location current,
                            std::optional<Grid::Location> previous = std::optional<Grid::Location>());

    void updateFinalPath(bool is_end, GridRenderer::GridWindow window,
                         GridRenderer::Information information,
                         Grid::Location current,
                         std::optional<Grid::Location> previous = std::optional<Grid::Location>());

    void mazeStatus(WINDOW *window, std::string top_text, GridRenderer::Information information, Grid::Location current);
    void pathStatus(WINDOW *window, std::string top_text, GridRenderer::Information information, Grid::Location current);
};
