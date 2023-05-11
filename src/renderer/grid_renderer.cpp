#include <string>

#include "renderer/grid_renderer.h"

GridRenderer::GridRenderer(size_t windows_amount, unsigned traverse_delay, unsigned step_delay) : Renderer()
{
    if (windows_amount == 0)
    {
        throw std::invalid_argument(
            "Grid Renderer exception: Cannot initialize windows. Amount of windows must be greater than 0."
        );
    }

    this->validateColor();

    size_t window_height = LINES / windows_amount;
    size_t grid_width    = COLS - this->MIN_STATUS_WIDTH;

    // don't count borders
    this->grid_height = (window_height % 2 == 0 ? --window_height : window_height) - 2;
    this->grid_width  = (grid_width % 2 == 0 ? --grid_width : grid_width) - 2;

    this->windows_amount = windows_amount;
    this->traverse_delay = traverse_delay;
    this->step_delay     = step_delay;
}

void GridRenderer::createWindows(const std::vector<std::string> &titles)
{
    size_t grid_windows = titles.size();

    if (grid_windows == 0)
    {
        throw std::invalid_argument(
            "Grid Renderer exception: Cannot initialize windows. Amount of windows must be greater than 0."
        );
    }

    if (grid_windows != this->windows_amount)
    {
        throw std::invalid_argument("Grid Renderer exception: Cannot initialize windows. Size of vector of titles is "
                                    "not equal to provided amount of windows to create.");
    }

    size_t window_height = this->grid_height + 2; // add borders
    size_t grid_width    = this->grid_width + 2;

    size_t status_width = COLS - grid_width;

    size_t offset_y = window_height;
    size_t offset_x = grid_width;

    this->grid_height = window_height - 2; // don't count borders
    this->grid_width  = grid_width - 2;

    for (size_t i = 0; i < grid_windows; i++)
    {
        GridRenderer::GridWindow window;

        window.title = titles[i];

        window.grid   = GridRenderer::createWindow(window_height, grid_width, 0, offset_y * i, titles[i]);
        window.status = GridRenderer::createWindow(window_height, status_width, offset_x, offset_y * i, "Information");

        this->windows.push_back(window);
    }
}

void GridRenderer::updateGridCell(WINDOW *window, const attr_t attribute, const Grid::Location &location)
{
    wattron(window, attribute);
    mvwaddch(window, location.y + 1, location.x + 1, ' ');
    wattroff(window, attribute);

    wrefresh(window);
}

GridRenderer::GridWindow GridRenderer::findWindow(const std::string &name)
{
    for (GridRenderer::GridWindow window : this->windows)
    {
        if (window.title == name)
        {
            return window;
        }
    }

    return GridWindow();
}

void GridRenderer::drawMazes(const std::vector<Grid::ChangeRecord> &maze_record)
{
    if (this->windows.empty())
    {
        throw std::invalid_argument("Grid Renderer exception: Cannot draw path traversal. No windows to update.");
    }

    if (maze_record.empty())
    {
        throw std::invalid_argument("Grid Renderer exception: Cannot draw mazes. Maze record is empty.");
    }

    // draw walls
    for (GridRenderer::GridWindow window : this->windows)
    {
        GridRenderer::fillWindow(window.grid, COLOR_PAIR(GridRenderer::ColorType::WALL) | A_INVIS, ' ');
    }

    for (size_t steps = 0; steps < maze_record.size(); steps++)
    {
        for (GridRenderer::GridWindow window : this->windows)
        {
            this->updateMaze(
                false,
                window,
                Grid::ChangeRecord{ maze_record[steps].location, maze_record[steps].time_taken, steps },
                steps == 0 ? std::optional<Grid::Location>() : maze_record[steps - 1].location
            );
        }

        // create any delay make maze generation smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(this->step_delay));
    }

    for (GridRenderer::GridWindow window : this->windows)
    {
        this->updateMaze(
            true, window, { maze_record.back().location, maze_record.back().time_taken, maze_record.size() }
        );
    }
}

void GridRenderer::updateMaze(
    const bool                           is_end,
    const GridRenderer::GridWindow      &window,
    const Grid::ChangeRecord            &information,
    const std::optional<Grid::Location> &previous
)
{
    // status window
    this->mazeStatus(window.status, is_end ? "The maze was generated!" : "Generating the maze...", information);

    // grid window
    auto current_color = is_end ? GridRenderer::ColorType::MAZE_TRAVERSED : GridRenderer::ColorType::MAZE_CURRENT;

    GridRenderer::updateGridCell(window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value())
    {
        GridRenderer::updateGridCell(
            window.grid, COLOR_PAIR(GridRenderer::ColorType::MAZE_TRAVERSED) | A_INVIS, previous.value()
        );
    }
}

void GridRenderer::drawPath(
    const bool                                          is_parallel,
    const std::vector<std::string>                     &titles,
    const std::vector<std::vector<Grid::ChangeRecord>> &traversed,
    const std::vector<std::vector<Grid::Location>>     &path
)
{
    if (titles.empty())
    {
        throw std::invalid_argument("Grid Renderer exception: Cannot draw path traversal. No windows to update.");
    }

    if (traversed.empty() || path.empty())
    {
        throw std::invalid_argument(
            "Grid Renderer exception: Cannot draw path traversal. Path traversal record is empty."
        );
    }

    if (titles.size() != traversed.size() && titles.size() != path.size())
    {
        throw std::invalid_argument(
            "Grid Renderer exception: Cannot draw path traversal. Amount of windows and path traversal records are not "
            "equal. Amount of windows: "
            + std::to_string(titles.size()) + "; amount of traversal: " + std::to_string(traversed.size())
            + "; amount of paths: " + std::to_string(path.size()) + "."
        );
    }

    // find all windows
    std::vector<GridRenderer::GridWindow> windows;

    for (std::string title : titles)
    {
        windows.push_back(this->findWindow(title));
    }

    if (!is_parallel)
    {
        for (size_t window = 0; window < windows.size(); window++)
        {
            this->drawPath(windows[window], traversed[window], path[window]);
        }
    }
    else
    {
        std::vector<std::thread> threads;

        for (size_t window = 0; window < windows.size(); window++)
        {
            threads.push_back(std::thread([this, windows, traversed, path, window] {
                this->drawPath(windows[window], traversed[window], path[window]);
            }));
        }

        for (size_t thread = 0; thread < threads.size(); thread++)
        {
            threads[thread].join();
        }
    }
}

void GridRenderer::drawPath(
    const GridRenderer::GridWindow        &window,
    const std::vector<Grid::ChangeRecord> &traversed,
    const std::vector<Grid::Location>     &path
)
{
    if (traversed.empty() || path.empty())
    {
        throw std::invalid_argument(
            "Grid Renderer exception: Cannot draw path traversal. Path traversal record is empty."
        );
    }

    for (size_t steps = 0; steps < traversed.size(); steps++)
    {
        Grid::ChangeRecord current = traversed[steps];
        current.step               = steps;

        this->updateTraversedPath(
            false, window, current, steps == 0 ? std::optional<Grid::Location>() : traversed[steps - 1].location
        );

        // create any delay make traversing smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(this->traverse_delay));
    }
    Grid::ChangeRecord result_info = traversed.back();

    this->updateTraversedPath(true, window, result_info);

    for (size_t steps = 0; steps < path.size(); steps++)
    {
        this->updateTraversedFinalPath(
            false,
            window,
            { path[steps], result_info.time_taken, result_info.step, result_info.cost },
            steps == 0 ? std::optional<Grid::Location>() : path[steps - 1]
        );

        // create any delay make traversing smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(this->step_delay));
    }

    this->updateTraversedFinalPath(true, window, result_info, path.back());
}

void GridRenderer::updateTraversedPath(
    const bool                           is_end,
    const GridRenderer::GridWindow      &window,
    const Grid::ChangeRecord            &information,
    const std::optional<Grid::Location> &previous
)
{
    // status window
    this->pathStatus(window.status, is_end ? "The path was found!" : "Finding the path...", information);

    // grid window
    auto current_color
        = is_end ? GridRenderer::ColorType::PATHFINDER_TRAVERSED : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value())
    {
        GridRenderer::updateGridCell(
            window.grid, COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_TRAVERSED) | A_INVIS, previous.value()
        );
    }
}

void GridRenderer::updateTraversedFinalPath(
    const bool                           is_end,
    const GridRenderer::GridWindow      &window,
    const Grid::ChangeRecord            &information,
    const std::optional<Grid::Location> &previous
)
{
    // status window
    this->pathStatus(window.status, is_end ? "The path was traversed!" : "Traversing the path...", information);

    // grid window
    auto current_color
        = is_end ? GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value())
    {
        GridRenderer::updateGridCell(
            window.grid, COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED) | A_INVIS, previous.value()
        );
    }
}

void GridRenderer::mazeStatus(WINDOW *window, const std::string &top_text, const Grid::ChangeRecord &information)
{
    // status window
    GridRenderer::clearWindow(window);
    GridRenderer::moveWindowPrint(window, 1, 1, top_text);

    GridRenderer::moveWindowPrint(window, 1, 3, "Total steps: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), std::to_string(information.step + 1)
    );

    GridRenderer::moveWindowPrint(window, 1, 4, "Last position: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), std::to_string(information.location)
    );

    Timer timer;

    GridRenderer::moveWindowPrint(window, 1, 5, "Time elapsed: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), timer.format(information.time_taken)
    );

    wrefresh(window);
}

void GridRenderer::pathStatus(WINDOW *window, const std::string &top_text, const Grid::ChangeRecord &information)
{
    // status window
    GridRenderer::clearWindow(window);
    GridRenderer::moveWindowPrint(window, 1, 1, top_text);

    GridRenderer::moveWindowPrint(window, 1, 3, "Total steps: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), std::to_string(information.step + 1)
    );

    GridRenderer::moveWindowPrint(window, 1, 4, "Last position: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), std::to_string(information.location)
    );

    GridRenderer::moveWindowPrint(window, 1, 5, "Last cost: ");
    GridRenderer::attrWindowPrint(window, COLOR_PAIR(GridRenderer::ColorType::VALUE), std::to_string(information.cost));

    Timer timer;

    GridRenderer::moveWindowPrint(window, 1, 6, "Time elapsed: ");
    GridRenderer::attrWindowPrint(
        window, COLOR_PAIR(GridRenderer::ColorType::VALUE), timer.format(information.time_taken)
    );

    wrefresh(window);
}
