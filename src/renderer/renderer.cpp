#include "renderer/renderer.h"

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "ncurses.h"

Renderer::~Renderer() {
    destroyWindow(this->first_grid);
    destroyWindow(this->first_grid_status);
    destroyWindow(this->second_grid);
    destroyWindow(this->second_grid_status);

    endwin();
}

Renderer::Renderer() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    int min_width = Renderer::MIN_GRID_WIDTH + Renderer::WINDOW_SPACING +
                    Renderer::MIN_STATUS_WIDTH;
    int min_height = Renderer::MIN_GRID_HEIGHT * 2 + Renderer::WINDOW_SPACING;
    std::string size_error_msg = "Minimum, required size of the terminal is " +
                                 std::to_string(min_width) + "x" +
                                 std::to_string(min_height);
    std::string color_error_msg = "Your terminal does not support color.";
    std::string refresh_msg = "Resize the window and restart the program.";

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
    int height = (LINES - Renderer::WINDOW_SPACING) / 2;
    height = height % 2 == 0 ? height - 1 : height;

    int grid_width =
        (COLS - Renderer::WINDOW_SPACING - Renderer::MIN_STATUS_WIDTH);
    grid_width = grid_width % 2 == 0 ? grid_width - 1 : grid_width;

    int status_width = (COLS - Renderer::WINDOW_SPACING - grid_width);
    int second_offset_y = height + Renderer::WINDOW_SPACING;
    int status_offset_x = grid_width + Renderer::WINDOW_SPACING;

    this->grid_height = height;
    this->grid_width = grid_width;

    this->first_grid = this->createWindow(height, grid_width, 0, 0);
    this->second_grid =
        this->createWindow(height, grid_width, 0, second_offset_y);
    this->first_grid_status =
        this->createWindow(height, status_width, status_offset_x, 0);
    this->second_grid_status = this->createWindow(
        height, status_width, status_offset_x, second_offset_y);

    // create color pairs
    init_pair(Renderer::ColorPairs::TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::VALUE, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::EMPTY, COLOR_BLACK, COLOR_BLACK);
    init_pair(Renderer::ColorPairs::WALL, COLOR_WHITE, COLOR_WHITE);
    init_pair(Renderer::ColorPairs::MAZE_CURRENT, COLOR_RED, COLOR_RED);
    init_pair(Renderer::ColorPairs::MAZE_TRAVERSED, COLOR_BLUE, COLOR_BLUE);
    init_pair(Renderer::ColorPairs::PATHFINDER_CURRENT, COLOR_RED, COLOR_RED);
    init_pair(Renderer::ColorPairs::PATHFINDER_TRAVERSED, COLOR_GREEN,
              COLOR_GREEN);
}

WINDOW *Renderer::createWindow(int height, int width, int start_x,
                               int start_y) {
    WINDOW *window;

    window = subwin(stdscr, height, width, start_y, start_x);
    box(window, 0, 0);
    wrefresh(window);

    return window;
}

void Renderer::destroyWindow(WINDOW *window) {
    wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(window);
    delwin(window);
}

void Renderer::drawMaze(std::vector<Grid::Location> path) {
    int step_delay = Renderer::MAZE_DRAW_TIME_MS / path.size();

    this->fillWindow(this->first_grid,
                     COLOR_PAIR(Renderer::ColorPairs::WALL) | A_INVIS, '#');
    this->fillWindow(this->second_grid,
                     COLOR_PAIR(Renderer::ColorPairs::WALL) | A_INVIS, '#');

    for (int i = 0; i < (int)path.size(); i++) {
        this->updateCharacter(
            this->first_grid,
            COLOR_PAIR(Renderer::ColorPairs::MAZE_CURRENT) | A_INVIS, '#',
            path[i].x, path[i].y);

        this->updateCharacter(
            this->second_grid,
            COLOR_PAIR(Renderer::ColorPairs::MAZE_CURRENT) | A_INVIS, '#',
            path[i].x, path[i].y);

        if (i > 0) {
            this->updateCharacter(
                this->first_grid,
                COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS, '#',
                path[i - 1].x, path[i - 1].y);

            this->updateCharacter(
                this->second_grid,
                COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS, '#',
                path[i - 1].x, path[i - 1].y);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(step_delay));
    }

    this->updateCharacter(
        this->first_grid,
        COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS, '#',
        path.back().x, path.back().y);

    this->updateCharacter(
        this->second_grid,
        COLOR_PAIR(Renderer::ColorPairs::MAZE_TRAVERSED) | A_INVIS, '#',
        path.back().x, path.back().y);
}

void Renderer::fillWindow(WINDOW *window, int attributes, char ch) {
    wattron(window, attributes);

    int rows, cols;
    getmaxyx(window, rows, cols);

    std::string line(cols, ch);

    for (int i = 0; i <= rows; i++) {
        wprintw(window, "%s", line.c_str());
    }

    wattroff(window, attributes);

    wrefresh(window);
}

void Renderer::updateCharacter(WINDOW *window, int attributes, char ch, int x,
                               int y) {
    wattron(window, attributes);
    mvwaddch(window, y, x, ch);
    wattroff(window, attributes);

    wrefresh(window);
}
