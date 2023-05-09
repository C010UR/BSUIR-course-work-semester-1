#include "renderer/renderer.h"

#include <chrono>
#include <optional>
#include <ratio>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "ncurses.h"

Renderer::~Renderer() {
    // destroy windows
    destroyWindow(this->first_grid);
    destroyWindow(this->first_grid_status);
    destroyWindow(this->second_grid);
    destroyWindow(this->second_grid_status);

    endwin();
}

Renderer::Renderer() {
    // setup
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    int min_width = Renderer::MIN_GRID_WIDTH + Renderer::MIN_STATUS_WIDTH;
    int min_height = Renderer::MIN_GRID_HEIGHT * 2;

    std::string size_error_msg = "Minimum, required size of the terminal is " +
                                 std::to_string(min_width) + "x" +
                                 std::to_string(min_height);
    std::string color_error_msg = "Your terminal does not support color.";
    std::string refresh_msg = "Resize the window and restart the program.";

    // check if terminal is big enough
    if (LINES < min_height && COLS < min_width) {
        mvprintw(std::min(1, ((LINES) / 2) - 1),
                 std::min((size_t)1, (COLS - size_error_msg.size()) / 2), "%s",
                 size_error_msg.c_str());
        mvprintw(std::min(3, (LINES / 2) + 1),
                 std::min((size_t)1, (COLS - refresh_msg.size()) / 2), "%s",
                 refresh_msg.c_str());

        refresh();
        getch();

        throw std::invalid_argument(size_error_msg);
    }

    // check if terminal supports colors
    if (!has_colors() && !can_change_color()) {
        mvprintw(std::min(1, ((LINES) / 2) - 1),
                 std::min((size_t)1, (COLS - color_error_msg.size()) / 2), "%s",
                 color_error_msg.c_str());

        refresh();
        getch();

        throw std::invalid_argument(color_error_msg);
    }

    start_color();

    // create windows
    int height = LINES / 2;
    height = height % 2 == 0 ? height - 1 : height;

    int grid_width = COLS - Renderer::MIN_STATUS_WIDTH;
    grid_width = grid_width % 2 == 0 ? grid_width - 1 : grid_width;

    int status_width = COLS - grid_width;
    int second_offset_y = height;
    int status_offset_x = grid_width;

    this->grid_height = height - 2;
    this->grid_width = grid_width - 2;

    this->first_grid =
        this->createWindow(height, grid_width, 0, 0, "Dijkstra algorithm");
    this->second_grid = this->createWindow(height, grid_width, 0,
                                           second_offset_y, "A* Algorithm");
    this->first_grid_status =
        this->createWindow(height, status_width, status_offset_x, 0, "Status");
    this->second_grid_status = this->createWindow(
        height, status_width, status_offset_x, second_offset_y, "Status");

    // create color pairs
    init_pair(Renderer::ColorPairs::TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::VALUE, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::EMPTY, COLOR_BLACK, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::MAZE_TRAVERSED, -1, -1);
    init_pair(Renderer::ColorPairs::MAZE_CURRENT, COLOR_RED, COLOR_RED);
    init_pair(Renderer::ColorPairs::WALL, COLOR_WHITE, COLOR_WHITE);
    init_pair(Renderer::ColorPairs::PATHFINDER_CURRENT, COLOR_RED, COLOR_RED);
    init_pair(Renderer::ColorPairs::PATHFINDER_TRAVERSED, COLOR_GREEN,
              COLOR_GREEN);
    init_pair(Renderer::ColorPairs::PATHFINDER_SEARCHED, COLOR_BLUE,
              COLOR_BLUE);
}

WINDOW *Renderer::createWindow(int height, int width, int start_x, int start_y,
                               std::string title) {
    WINDOW *window;

    window = subwin(stdscr, height, width, start_y, start_x);
    box(window, 0, 0);

    wattron(window, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    mvwprintw(window, 0, 1, "%s", title.c_str());
    wattroff(window, COLOR_PAIR(Renderer::ColorPairs::VALUE));

    wrefresh(window);

    return window;
}

void Renderer::destroyWindow(WINDOW *window) {
    wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(window);
    delwin(window);
}

void Renderer::fillWindow(WINDOW *window, int attributes, char ch) {
    wattron(window, attributes);

    int rows, cols;
    getmaxyx(window, rows, cols);

    std::string line(cols - 2, ch);

    for (int i = 1; i <= rows - 2; i++) {
        mvwprintw(window, i, 1, "%s", line.c_str());
    }

    wattroff(window, attributes);

    wrefresh(window);
}

void Renderer::clearWindow(WINDOW *window) {
    this->fillWindow(window, A_NORMAL, ' ');
}

void Renderer::updateMazeCharacter(WINDOW *window, int attributes,
                                   Grid::Location location) {
    wattron(window, attributes);
    mvwaddch(window, location.y + 1, location.x + 1, ' ');
    wattroff(window, attributes);

    wrefresh(window);
}

void Renderer::drawMaze(std::vector<Grid::Record> maze_steps) {
    // create walls
    this->fillWindow(this->first_grid,
                     COLOR_PAIR(Renderer::ColorPairs::WALL) | A_INVIS, ' ');
    this->fillWindow(this->second_grid,
                     COLOR_PAIR(Renderer::ColorPairs::WALL) | A_INVIS, ' ');

    for (int i = 0; i < (int)maze_steps.size(); i++) {
        this->drawMazeStep(this->first_grid, this->first_grid_status, i,
                           maze_steps[i].time, maze_steps[i].location,
                           i == 0 ? std::optional<Grid::Location>()
                                  : maze_steps[i - 1].location);

        this->drawMazeStep(this->second_grid, this->second_grid_status, i,
                           maze_steps[i].time, maze_steps[i].location,
                           i == 0 ? std::optional<Grid::Location>()
                                  : maze_steps[i - 1].location);

        // create any delay make maze generation smooth
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // update last characters
    this->updateMazeCharacter(
        this->first_grid,
        COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS,
        maze_steps.back().location);

    this->updateMazeCharacter(
        this->second_grid,
        COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS,
        maze_steps.back().location);
}

void Renderer::drawMazeStep(WINDOW *grid, WINDOW *status, int step,
                            std::chrono::microseconds time,
                            Grid::Location current,
                            std::optional<Grid::Location> previous) {
    // status
    this->clearWindow(status);
    mvwprintw(status, 1, 1, "Generating maze...");

    mvwprintw(status, 3, 1, "Steps taken: ");
    wattron(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    wprintw(status, "%d", step + 1);
    wattroff(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));

    Timer timer;

    mvwprintw(status, 5, 1, "Time elapsed: ");
    wattron(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    wprintw(status, "%s", timer.format(time).c_str());
    wattroff(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    wrefresh(status);

    // maze block
    this->updateMazeCharacter(
        grid, COLOR_PAIR(Renderer::ColorPairs::MAZE_CURRENT) | A_INVIS,
        current);

    if (previous.has_value()) {
        this->updateMazeCharacter(
            grid, COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS,
            previous.value());
    }
}

void Renderer::drawPath(
    std::vector<Grid::Location> first_searched,
    std::unordered_map<Grid::Location, Grid::cost_t> first_cost,
    std::vector<Grid::Location> second_searched,
    std::unordered_map<Grid::Location, Grid::cost_t> second_cost) {
    int first_searched_size = first_searched.size();
    int second_searched_size = second_searched.size();

    for (int i = 0; i < first_searched_size; i++) {
        drawPathStep(
            this->first_grid, this->first_grid_status, i,
            first_cost[first_searched[i]], first_searched[i],
            i == 0 ? std::optional<Grid::Location>() : first_searched[i - 1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (int i = 0; i < second_searched_size; i++) {
        drawPathStep(
            this->second_grid, this->second_grid_status, i,
            second_cost[second_searched[i]], second_searched[i],
            i == 0 ? std::optional<Grid::Location>() : second_searched[i - 1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // update last characters
    this->updateMazeCharacter(
        this->first_grid,
        COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_SEARCHED) | A_INVIS,
        first_searched.back());

    this->updateMazeCharacter(
        this->second_grid,
        COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_SEARCHED) | A_INVIS,
        second_searched.back());
}

void Renderer::drawPathStep(WINDOW *grid, WINDOW *status, int step,
                            Grid::cost_t cost, Grid::Location current,
                            std::optional<Grid::Location> previous) {
    // status
    this->clearWindow(status);
    mvwprintw(status, 1, 1, "Traversing the path...");

    mvwprintw(status, 3, 1, "Steps taken: ");
    wattron(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    wprintw(status, "%d", step + 1);
    wattroff(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));

    mvwprintw(status, 5, 1, "Current cost: ");
    wattron(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));
    wprintw(status, "%d", cost);
    wattroff(status, COLOR_PAIR(Renderer::ColorPairs::VALUE));

    wrefresh(status);

    // maze block
    this->updateMazeCharacter(
        grid, COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_CURRENT) | A_INVIS,
        current);

    if (previous.has_value()) {
        this->updateMazeCharacter(
            grid,
            COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_SEARCHED) | A_INVIS,
            previous.value());
    }
}

void Renderer::drawFinalPath(std::vector<Grid::Location> first,
                             std::vector<Grid::Location> second) {
    int cycles = std::max(first.size(), second.size());
    int first_size = first.size();
    int second_size = second.size();

    for (int i = 0; i < cycles; i++) {
        if (i < first_size) {
            drawFinalPathStep(
                this->first_grid, first[i],
                i == 0 ? std::optional<Grid::Location>() : first[i - 1]);
        }

        if (i < second_size) {
            drawFinalPathStep(
                this->second_grid, second[i],
                i == 0 ? std::optional<Grid::Location>() : second[i - 1]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // update last characters
    this->updateMazeCharacter(
        this->first_grid,
        COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_TRAVERSED) | A_INVIS,
        first.back());

    this->updateMazeCharacter(
        this->second_grid,
        COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_TRAVERSED) | A_INVIS,
        second.back());
}

void Renderer::drawFinalPathStep(WINDOW *grid, Grid::Location current,
                                 std::optional<Grid::Location> previous) {
    // maze block
    this->updateMazeCharacter(
        grid, COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_CURRENT) | A_INVIS,
        current);

    if (previous.has_value()) {
        this->updateMazeCharacter(
            grid,
            COLOR_PAIR(Renderer::ColorPairs::PATHFINDER_TRAVERSED) | A_INVIS,
            previous.value());
    }
}
