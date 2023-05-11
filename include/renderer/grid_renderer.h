#pragma once

#include <ncurses.h>

#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "data_structure/grid.h"
#include "renderer/renderer.h"

class GridRenderer : Renderer
{
  public:
    static const size_t MIN_GRID_HEIGHT  = 7;
    static const size_t MIN_GRID_WIDTH   = 41;
    static const size_t MIN_STATUS_WIDTH = 38;

    /** Holds everything needed for grid window */
    struct GridWindow
    {
        std::string title;
        WINDOW     *grid;
        WINDOW     *status;
    };

    size_t   windows_amount;
    size_t   grid_height;
    size_t   grid_width;
    unsigned traverse_delay;
    unsigned step_delay;

    /**
     * @brief Construct a new Grid Renderer object and set approximate grid
     * height and grid width
     *
     * @param windows_amount
     * @param traverse_delay - delay for pathfinder path traversal
     * @param step_delay - any other delay
     */
    GridRenderer(const size_t windows_amount, const unsigned traverse_delay = 40, const unsigned step_delay = 1);

    /**
     * @brief Create `GridRenderer::GridWindow` for each title
     *
     * @param titles
     */
    void createWindows(const std::vector<std::string> &titles);

    /**
     * @brief Update grid window cell at specified location using attribute
     *
     * @param window
     * @param attribute
     * @param location
     */
    static void updateGridCell(WINDOW *window, const attr_t attribute, const Grid::Location &location);

    /**
     * @brief Find `GridRenderer::GridWindow` by title
     *
     * @param name
     * @return GridRenderer::GridWindow
     */
    GridRenderer::GridWindow findWindow(const std::string &name);

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
    void drawPath(
        const bool                                          is_parallel,
        const std::vector<std::string>                     &titles,
        const std::vector<std::vector<Grid::ChangeRecord>> &traversed,
        const std::vector<std::vector<Grid::Location>>     &path
    );

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
    void drawPath(
        const GridRenderer::GridWindow        &window,
        const std::vector<Grid::ChangeRecord> &traversed,
        const std::vector<Grid::Location>     &path
    );

    /**
     * @brief Render a step in maze generation
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateMaze(
        const bool                           is_end,
        const GridRenderer::GridWindow      &window,
        const Grid::ChangeRecord            &information,
        const std::optional<Grid::Location> &previous = std::optional<Grid::Location>()
    );

    /**
     * @brief Render a step in path traversal
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateTraversedPath(
        const bool                           is_end,
        const GridRenderer::GridWindow      &window,
        const Grid::ChangeRecord            &information,
        const std::optional<Grid::Location> &previous = std::optional<Grid::Location>()
    );

    /**
     * @brief Render a step in path solution traversal
     *
     * @param is_end
     * @param window
     * @param information
     * @param previous
     */
    void updateTraversedFinalPath(
        const bool                           is_end,
        const GridRenderer::GridWindow      &window,
        const Grid::ChangeRecord            &information,
        const std::optional<Grid::Location> &previous = std::optional<Grid::Location>()
    );

    /**
     * @brief Render status for maze generation
     *
     * @param window
     * @param top_text
     * @param information
     */
    void mazeStatus(WINDOW *window, const std::string &top_text, const Grid::ChangeRecord &information);

    /**
     * @brief Render status for path traversal
     *
     * @param window
     * @param top_text
     * @param information
     */
    void pathStatus(WINDOW *window, const std::string &top_text, const Grid::ChangeRecord &information);
};
