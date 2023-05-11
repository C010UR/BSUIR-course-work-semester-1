#include "renderer/renderer.h"

#include "ncurses.h"

const std::vector<Renderer::ColorPair> Renderer::color_pairs = {
    {Renderer::ColorType::TEXT,                       COLOR_WHITE,  COLOR_BLACK},
    {Renderer::ColorType::VALUE,                      COLOR_YELLOW, COLOR_BLACK},
    {Renderer::ColorType::EMPTY,                      COLOR_BLACK,  COLOR_BLACK},
    {Renderer::ColorType::MAZE_TRAVERSED,             -1,           -1         },
    {Renderer::ColorType::MAZE_CURRENT,               COLOR_RED,    COLOR_RED  },
    {Renderer::ColorType::WALL,                       COLOR_WHITE,  COLOR_WHITE},
    {Renderer::ColorType::PATHFINDER_CURRENT,         COLOR_RED,    COLOR_RED  },
    {Renderer::ColorType::PATHFINDER_TRAVERSED,       COLOR_BLUE,   COLOR_BLUE },
    {Renderer::ColorType::PATHFINDER_FINAL_TRAVERSED, COLOR_GREEN,  COLOR_GREEN}
};

Renderer::~Renderer()
{
    endwin();
}

Renderer::Renderer()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
}

void Renderer::validateTerminalResolution(const size_t min_width, const size_t min_height)
{
    if ((size_t)LINES < min_height && (size_t)COLS < min_width)
    {
        throw std::invalid_argument(
            "Terminal error: Cannot start program. Terminal resolution is too small. Minimum terminal resolution is "
            + std::to_string(min_width) + "x" + std::to_string(min_height) + "."
        );
    }
}

void Renderer::validateColor()
{
    if (!has_colors())
    {
        throw std::invalid_argument("Terminal error: Cannot start program. Terminal does not support colored output.");
    }

    start_color();

    // add all color pairs
    for (Renderer::ColorPair pair : this->color_pairs)
    {
        init_pair(pair.type, pair.foreground, pair.background);
    }
}

WINDOW *Renderer::createWindow(
    const size_t height, const size_t width, const size_t start_x, const size_t start_y, const std::string &title
)
{
    WINDOW *window = subwin(stdscr, height, width, start_y, start_x);

    box(window, 0, 0);
    Renderer::attrMoveWindowPrint(window, COLOR_PAIR(Renderer::ColorType::VALUE), 1, 0, title);
    wrefresh(window);

    return window;
}

void Renderer::destroyWindow(WINDOW *window)
{
    wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    Renderer::clearWindow(window);
    wrefresh(window);
    delwin(window);
}

void Renderer::fillWindow(WINDOW *window, const int attribute, const char character)
{
    wattron(window, attribute);

    size_t rows, cols;
    getmaxyx(window, rows, cols);

    std::string line(cols - 2, character);

    for (size_t i = 1; i <= rows - 2; i++)
    {
        Renderer::moveWindowPrint(window, 1, i, line);
    }

    wattroff(window, attribute);
    wrefresh(window);
}

void Renderer::clearWindow(WINDOW *window)
{
    Renderer::fillWindow(window, A_NORMAL, ' ');
}

// custom prints
void Renderer::attrMoveWindowPrint(
    WINDOW *window, const attr_t attribute, const size_t x, const size_t y, const std::string &line
)
{
    wattron(window, attribute);
    mvwprintw(window, y, x, "%s", line.c_str());
    wattroff(window, attribute);
}

void Renderer::attrWindowPrint(WINDOW *window, const attr_t attribute, const std::string &line)
{
    wattron(window, attribute);
    wprintw(window, "%s", line.c_str());
    wattroff(window, attribute);
}

void Renderer::moveWindowPrint(WINDOW *window, const size_t x, const size_t y, const std::string &line)
{
    mvwprintw(window, y, x, "%s", line.c_str());
}
