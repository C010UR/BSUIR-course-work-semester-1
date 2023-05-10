#include "renderer/grid_renderer.h"

#include <chrono>
#include <vector>

GridRenderer::GridRenderer(std::vector<std::string> titles, int traverse_delay,
                           int step_delay)
    : Renderer() {
    this->validateColor();

    int grid_windows = titles.size();

    int window_height = LINES / grid_windows;
    window_height = window_height % 2 == 0 ? window_height - 1 : window_height;

    int grid_width = COLS - this->MIN_STATUS_WIDTH;
    grid_width = grid_width % 2 == 0 ? grid_width - 1 : grid_width;

    int status_width = COLS - grid_width;

    int offset_y = window_height;
    int offset_x = grid_width;

    this->grid_height = window_height - 2;  // don't count borders
    this->grid_width = grid_width - 2;

    for (int i = 0; i < grid_windows; i++) {
        GridRenderer::GridWindow window;

        window.title = titles[i];

        window.grid = GridRenderer::createWindow(window_height, grid_width, 0,
                                                 offset_y * i, titles[i]);
        window.status = GridRenderer::createWindow(
            window_height, status_width, offset_x, offset_y * i, "Information");

        this->windows.push_back(window);
    }

    this->traverse_delay = traverse_delay;
    this->step_delay = step_delay;
}

void GridRenderer::updateGridCell(WINDOW *window, attr_t attribute,
                                  Grid::Location location) {
    wattron(window, attribute);
    mvwaddch(window, location.y + 1, location.x + 1, ' ');
    wattroff(window, attribute);

    wrefresh(window);
}

GridRenderer::GridWindow GridRenderer::findWindow(std::string name) {
    for (GridRenderer::GridWindow window : this->windows) {
        if (window.title == name) {
            return window;
        }
    }

    return GridWindow();
}

void GridRenderer::drawMazes(
    const std::vector<Grid::ChangeRecord> &maze_record) {
    // draw walls
    for (GridRenderer::GridWindow window : this->windows) {
        GridRenderer::fillWindow(
            window.grid, COLOR_PAIR(GridRenderer::ColorType::WALL) | A_INVIS,
            ' ');
    }

    for (int steps = 0; steps < (int)maze_record.size(); steps++) {
        for (GridRenderer::GridWindow window : this->windows) {
            this->updateMaze(
                false, window,
                Grid::ChangeRecord{maze_record[steps].location,
                                   maze_record[steps].time_taken, steps},
                steps == 0 ? std::optional<Grid::Location>()
                           : maze_record[steps - 1].location);
        }

        // create any delay make maze generation smooth
        std::this_thread::sleep_for(
            std::chrono::milliseconds(this->step_delay));
    }

    for (GridRenderer::GridWindow window : this->windows) {
        this->updateMaze(
            true, window,
            {maze_record.back().location, maze_record.back().time_taken,
             (int)maze_record.size()});
    }
}

void GridRenderer::updateMaze(bool is_end, GridRenderer::GridWindow window,
                              Grid::ChangeRecord information,
                              std::optional<Grid::Location> previous) {
    // status window
    this->mazeStatus(
        window.status,
        is_end ? "The maze was generated!" : "Generating the maze...",
        information);

    // grid window
    auto current_color = is_end ? GridRenderer::ColorType::MAZE_TRAVERSED
                                : GridRenderer::ColorType::MAZE_CURRENT;

    GridRenderer::updateGridCell(
        window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::MAZE_TRAVERSED) | A_INVIS,
            previous.value());
    }
}

void GridRenderer::drawPath(
    bool is_parallel, std::vector<std::string> titles,
    std::vector<std::vector<Grid::ChangeRecord>> traversed,
    std::vector<std::vector<Grid::Location>> path) {
    // find all windows
    std::vector<GridRenderer::GridWindow> windows;

    for (std::string title : titles) {
        windows.push_back(this->findWindow(title));
    }

    if (!is_parallel) {
        for (int window = 0; window < (int)windows.size(); window++) {
            this->drawPath(windows[window], traversed[window], path[window]);
        }
    } else {
        std::vector<std::thread> threads;

        for (int window = 0; window < (int)windows.size(); window++) {
            threads.push_back(
                std::thread([this, windows, traversed, path, window] {
                    this->drawPath(windows[window], traversed[window],
                                   path[window]);
                }));
        }

        for (int thread = 0; thread < (int)threads.size(); thread++) {
            threads[thread].join();
        }
    }
}

void GridRenderer::drawPath(GridRenderer::GridWindow window,
                            std::vector<Grid::ChangeRecord> traversed,
                            std::vector<Grid::Location> path) {
    for (int steps = 0; steps < (int)traversed.size(); steps++) {
        traversed[steps].step = steps;

        this->updateTraversedPath(false, window, traversed[steps],
                                  steps == 0 ? std::optional<Grid::Location>()
                                             : traversed[steps - 1].location);

        // create any delay make traversing smooth
        std::this_thread::sleep_for(
            std::chrono::milliseconds(this->traverse_delay));
    }
    Grid::ChangeRecord result_info = traversed.back();

    this->updateTraversedPath(true, window, result_info);

    for (int steps = 0; steps < (int)path.size(); steps++) {
        this->updateFinalPath(
            false, window,
            {path[steps], result_info.time_taken, result_info.step,
             result_info.cost},
            steps == 0 ? std::optional<Grid::Location>() : path[steps - 1]);

        // create any delay make traversing smooth
        std::this_thread::sleep_for(
            std::chrono::milliseconds(this->step_delay));
    }

    this->updateFinalPath(true, window, result_info, path.back());
}

void GridRenderer::updateTraversedPath(bool is_end,
                                       GridRenderer::GridWindow window,
                                       Grid::ChangeRecord information,
                                       std::optional<Grid::Location> previous) {
    // status window
    this->pathStatus(window.status,
                     is_end ? "The path was found!" : "Finding the path...",
                     information);

    // grid window
    auto current_color = is_end ? GridRenderer::ColorType::PATHFINDER_TRAVERSED
                                : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(
        window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_TRAVERSED) | A_INVIS,
            previous.value());
    }
}

void GridRenderer::updateFinalPath(bool is_end, GridRenderer::GridWindow window,
                                   Grid::ChangeRecord information,
                                   std::optional<Grid::Location> previous) {
    // status window
    this->pathStatus(
        window.status,
        is_end ? "The path was traversed!" : "Traversing the path...",
        information);

    // grid window
    auto current_color =
        is_end ? GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED
               : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(
        window.grid, COLOR_PAIR(current_color) | A_INVIS, information.location);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED) |
                A_INVIS,
            previous.value());
    }
}

void GridRenderer::mazeStatus(WINDOW *window, std::string top_text,
                              Grid::ChangeRecord information) {
    // status window
    GridRenderer::clearWindow(window);
    GridRenderer::moveWindowPrint(window, 1, 1, top_text);

    GridRenderer::moveWindowPrint(window, 1, 3, "Total steps: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  std::to_string(information.step + 1));

    GridRenderer::moveWindowPrint(window, 1, 4, "Last position: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  std::to_string(information.location));

    Timer timer;

    GridRenderer::moveWindowPrint(window, 1, 5, "Time elapsed: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  timer.format(information.time_taken));

    wrefresh(window);
}

void GridRenderer::pathStatus(WINDOW *window, std::string top_text,
                              Grid::ChangeRecord information) {
    // status window
    GridRenderer::clearWindow(window);
    GridRenderer::moveWindowPrint(window, 1, 1, top_text);

    GridRenderer::moveWindowPrint(window, 1, 3, "Total steps: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  std::to_string(information.step + 1));

    GridRenderer::moveWindowPrint(window, 1, 4, "Last position: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  std::to_string(information.location));

    GridRenderer::moveWindowPrint(window, 1, 5, "Last cost: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  std::to_string(information.cost));

    Timer timer;

    GridRenderer::moveWindowPrint(window, 1, 6, "Time elapsed: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  timer.format(information.time_taken));

    wrefresh(window);
}
