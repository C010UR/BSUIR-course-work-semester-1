#include <chrono>

#include "renderer/grid_renderer.h"

GridRenderer::GridRenderer(std::vector<std::string> titles) : Renderer() {
    this->validateAndStartColor();

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
            this->updateMaze(false, window,
                             {steps, maze_record[steps].time_taken, 0},
                             maze_record[steps].location,
                             steps == 0 ? std::optional<Grid::Location>()
                                        : maze_record[steps - 1].location);
        }

        // create any delay make maze generation smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (GridRenderer::GridWindow window : this->windows) {
        this->updateMaze(
            true, window,
            {(int)maze_record.size(), maze_record.back().time_taken, 0},
            maze_record.back().location);
    }
}

void GridRenderer::updateMaze(bool is_end, GridRenderer::GridWindow window,
                              GridRenderer::Information information,
                              Grid::Location current,
                              std::optional<Grid::Location> previous) {
    // status window
    this->mazeStatus(
        window.status,
        is_end ? "The maze was generated!" : "Generating the maze...",
        information, current);

    // grid window
    auto current_color = is_end ? GridRenderer::ColorType::MAZE_TRAVERSED
                                : GridRenderer::ColorType::MAZE_CURRENT;

    GridRenderer::updateGridCell(window.grid,
                                 COLOR_PAIR(current_color) | A_INVIS, current);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::MAZE_TRAVERSED) | A_INVIS,
            previous.value());
    }
}

GridRenderer::Information GridRenderer::drawTraversedPath(
    const GridRenderer::GridWindow window,
    const std::vector<Grid::ChangeRecord> &traversed,
    const std::unordered_map<Grid::Location, Grid::cost_t> &cost) {
    for (int steps = 0; steps < (int)traversed.size(); steps++) {
        this->updateTraversedPath(false, window,
                                  {steps, traversed[steps].time_taken,
                                   cost.at(traversed[steps].location)},
                                  traversed[steps].location,
                                  steps == 0 ? std::optional<Grid::Location>()
                                             : traversed[steps - 1].location);

        // create any delay make traversing smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    GridRenderer::Information result{(int)traversed.size(),
                                     traversed.back().time_taken,
                                     cost.at(traversed.back().location)};

    this->updateTraversedPath(true, window, result, traversed.back().location);

    return result;
}

void GridRenderer::updateTraversedPath(bool is_end,
                                       GridRenderer::GridWindow window,
                                       GridRenderer::Information information,
                                       Grid::Location current,
                                       std::optional<Grid::Location> previous) {
    // status window
    this->pathStatus(window.status,
                     is_end ? "The path was found!" : "Finding the path...",
                     information, current);

    // grid window
    auto current_color = is_end ? GridRenderer::ColorType::PATHFINDER_TRAVERSED
                                : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(window.grid,
                                 COLOR_PAIR(current_color) | A_INVIS, current);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_TRAVERSED) | A_INVIS,
            previous.value());
    }
}

void GridRenderer::drawFinalPath(GridRenderer::GridWindow window,
                                 GridRenderer::Information information,
                                 const std::vector<Grid::Location> &path) {
    for (int steps = 0; steps < (int)path.size(); steps++) {
        this->updateFinalPath(
            false, window, information, path[steps],
            steps == 0 ? std::optional<Grid::Location>() : path[steps - 1]);

        // create any delay make traversing smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    this->updateFinalPath(true, window, information, path.back());
}

void GridRenderer::updateFinalPath(bool is_end, GridRenderer::GridWindow window,
                                   GridRenderer::Information information,
                                   Grid::Location current,
                                   std::optional<Grid::Location> previous) {
    // status window
    this->pathStatus(
        window.status,
        is_end ? "The path was traversed!" : "Traversing the path...",
        information, current);

    // grid window
    auto current_color =
        is_end ? GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED
               : GridRenderer::ColorType::PATHFINDER_CURRENT;

    GridRenderer::updateGridCell(window.grid,
                                 COLOR_PAIR(current_color) | A_INVIS, current);

    if (previous.has_value()) {
        GridRenderer::updateGridCell(
            window.grid,
            COLOR_PAIR(GridRenderer::ColorType::PATHFINDER_FINAL_TRAVERSED) |
                A_INVIS,
            previous.value());
    }
}

void GridRenderer::mazeStatus(WINDOW *window, std::string top_text,
                              GridRenderer::Information information,
                              Grid::Location current) {
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
                                  std::to_string(current));

    Timer timer;

    GridRenderer::moveWindowPrint(window, 1, 5, "Time elapsed: ");
    GridRenderer::attrWindowPrint(window,
                                  COLOR_PAIR(GridRenderer::ColorType::VALUE),
                                  timer.format(information.time_taken));

    wrefresh(window);
}

void GridRenderer::pathStatus(WINDOW *window, std::string top_text,
                              GridRenderer::Information information,
                              Grid::Location current) {
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
                                  std::to_string(current));

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
