#include "ncurses.h"
#include "renderer/renderer.h"

Renderer::~Renderer() { endwin(); }

Renderer::Renderer() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
}

void Renderer::validateTerminalSize(int min_width, int min_height) {
    if (LINES < min_height && COLS < min_width) {
        printw("%s", this->size_error_msg.c_str());

        refresh();
        getch();

        exit(EXIT_FAILURE);
    }
}

void Renderer::validateColor() {
    if (!has_colors()) {
        printw("%s", this->color_error_msg.c_str());

        refresh();
        getch();

        exit(EXIT_FAILURE);
    }

    start_color();

    // add all color pairs
    for (Renderer::ColorPair pair : this->color_pairs) {
        init_pair(pair.type, pair.foreground, pair.background);
    }
}

WINDOW *Renderer::createWindow(int height, int width, int start_x, int start_y,
                               std::string title) {
    WINDOW *window = subwin(stdscr, height, width, start_y, start_x);

    box(window, 0, 0);
    Renderer::attrMoveWindowPrint(
        window, COLOR_PAIR(Renderer::ColorType::VALUE), 1, 0, title);
    wrefresh(window);

    return window;
}

void Renderer::destroyWindow(WINDOW *window) {
    wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    Renderer::clearWindow(window);
    wrefresh(window);
    delwin(window);
}

void Renderer::fillWindow(WINDOW *window, int attribute, char character) {
    wattron(window, attribute);

    int rows, cols;
    getmaxyx(window, rows, cols);

    std::string line(cols - 2, character);

    for (int i = 1; i <= rows - 2; i++) {
        Renderer::moveWindowPrint(window, 1, i, line);
    }

    wattroff(window, attribute);
    wrefresh(window);
}

void Renderer::clearWindow(WINDOW *window) {
    Renderer::fillWindow(window, A_NORMAL, ' ');
}

// custom prints
void Renderer::attrMoveWindowPrint(WINDOW *window, attr_t attribute, int x,
                                   int y, std::string line) {
    wattron(window, attribute);
    mvwprintw(window, y, x, "%s", line.c_str());
    wattroff(window, attribute);
}

void Renderer::attrWindowPrint(WINDOW *window, attr_t attribute,
                               std::string line) {
    wattron(window, attribute);
    wprintw(window, "%s", line.c_str());
    wattroff(window, attribute);
}

void Renderer::moveWindowPrint(WINDOW *window, int x, int y, std::string line) {
    mvwprintw(window, y, x, "%s", line.c_str());
}
