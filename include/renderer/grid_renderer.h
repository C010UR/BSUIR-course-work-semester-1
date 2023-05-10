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

    /** Holds everything needed for grid window */
    struct GridWindow {
        std::string title;
        WINDOW *grid;
        WINDOW *status;
    };

    int grid_height;
    int grid_width;
    int traverse_delay;
    int step_delay;

    /**
     * @brief Construct a new Grid Renderer object with
     * `GridRenderer::GridWindow` for each title
     *
     * @param titles
     * @param traverse_delay - delay for pathfinder path traversal
     * @param step_delay - any other delay
     */
    GridRenderer(std::vector<std::string> titles, int traverse_delay = 40,
                 int step_delay = 1);

    /**
     * @brief Update grid window cell at specified location using attribute
     *
     * @param window
     * @param attribute
     * @param location
     */
    static void updateGridCell(WINDOW *window, attr_t attribute,
                               Grid::Location location);

    /**
     * @brief Find `GridRenderer::GridWindow` by title
     *
     * @param name
     * @return GridRenderer::GridWindow
     */
    GridRenderer::GridWindow findWindow(std::string name);

    /**
     * @brief Draw maze using `maze_record` from maze generator for each stored
     * `GridRenderer::GridWindow`
     *
     * @param maze_record
     */
    void drawMazes(const std::vector<Grid::ChangeRecord> &maze_record);

    /**
     * @brief Draw pathfinder path traversal using `traversed` and `path`
     * provided by path finder algorithms for a specified window
     *
     * @param is_parallel
     * @param titles
     * @param traversed
     * @param path
     */
    void drawPath(bool is_parallel, std::vector<std::string> titles,
                  std::vector<std::vector<Grid::ChangeRecord>> traversed,
                  std::vector<std::vector<Grid::Location>> path);

   private:
    std::vector<GridWindow> windows;

    /**
     * @brief Draw pathfinder path traversal using `traversed` and `path`
     * provided by path finder algorithms for a specified window
     *
     * @param window
     * @param traversed
     * @param path
     */
    void drawPath(GridRenderer::GridWindow window,
                  std::vector<Grid::ChangeRecord> traversed,
                  std::vector<Grid::Location> path);

    /**
     * @brief Draw path in parallel for each window
     *
     * @param titles
     * @param traversed
     * @param path
     */
    void drawPathParallel(
        std::vector<GridRenderer::GridWindow> windows,
        std::vector<std::vector<Grid::ChangeRecord>> traversed,
        std::vector<std::vector<Grid::Location>> path);

    /**
     * @brief Draw path linear for each window
     *
     * @param titles
     * @param traversed
     * @param path
     */
    void drawPathLinear(std::vector<GridRenderer::GridWindow> windows,
                        std::vector<std::vector<Grid::ChangeRecord>> traversed,
                        std::vector<std::vector<Grid::Location>> path);

    /**
     * @brief Render a step in maze generation
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateMaze(bool is_end, GridRenderer::GridWindow window,
                    Grid::ChangeRecord information,
                    std::optional<Grid::Location> previous =
                        std::optional<Grid::Location>());

    /**
     * @brief Render a step in path traversal
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateTraversedPath(bool is_end, GridRenderer::GridWindow window,
                             Grid::ChangeRecord information,
                             std::optional<Grid::Location> previous =
                                 std::optional<Grid::Location>());

    /**
     * @brief Render a step in path solution traversal
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateFinalPath(bool is_end, GridRenderer::GridWindow window,
                         Grid::ChangeRecord information,
                         std::optional<Grid::Location> previous =
                             std::optional<Grid::Location>());

    /**
     * @brief Render status for maze generation
     *
     * @param window
     * @param top_text
     * @param information
     */
    void mazeStatus(WINDOW *window, std::string top_text,
                    Grid::ChangeRecord information);

    /**
     * @brief Render status for path traversal
     *
     * @param window
     * @param top_text
     * @param information
     */
    void pathStatus(WINDOW *window, std::string top_text,
                    Grid::ChangeRecord information);
};
