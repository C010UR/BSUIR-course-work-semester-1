#pragma once

#include <ncurses.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "renderer/renderer.h"
#include "utility/timer.h"

class Renderer {
   public:
    enum ColorType {
        TEXT,
        VALUE,
        WALL,
        EMPTY,
        MAZE_CURRENT,
        MAZE_TRAVERSED,
        PATHFINDER_CURRENT,
        PATHFINDER_TRAVERSED,
        PATHFINDER_FINAL_TRAVERSED,
    };

    struct ColorPair {
        Renderer::ColorType type;
        int foreground;
        int background;
    };

    struct CustomColor {
        attr_t to_change;
        short red;
        short green;
        short blue;
    };

    const std::vector<Renderer::ColorPair> color_pairs = {
        {Renderer::ColorType::TEXT, COLOR_WHITE, COLOR_BLACK},
        {Renderer::ColorType::VALUE, COLOR_YELLOW, COLOR_BLACK},
        {Renderer::ColorType::EMPTY, COLOR_BLACK, COLOR_BLACK},
        {Renderer::ColorType::MAZE_TRAVERSED, -1, -1},
        {Renderer::ColorType::MAZE_CURRENT, COLOR_RED, COLOR_RED},
        {Renderer::ColorType::WALL, COLOR_WHITE, COLOR_WHITE},
        {Renderer::ColorType::PATHFINDER_CURRENT, COLOR_RED, COLOR_RED},
        {Renderer::ColorType::PATHFINDER_TRAVERSED, COLOR_BLUE, COLOR_BLUE},
        {Renderer::ColorType::PATHFINDER_FINAL_TRAVERSED, COLOR_GREEN,
         COLOR_GREEN}};

    const std::vector<Renderer::CustomColor> custom_colors = {
        {COLOR_BLACK, 0, 0, 0},
        {COLOR_RED, 235, 77, 75},
        {COLOR_GREEN, 106, 176, 76},
        {COLOR_YELLOW, 249, 202, 36},
        {COLOR_BLUE, 48, 51, 107},
        {COLOR_MAGENTA, 190, 46, 221},
        {COLOR_CYAN, 34, 166, 179},
        {COLOR_WHITE, 229, 229, 229}
    };

    const std::string size_error_msg =
        "Terminal size is too small.\n\nEnlarge the terminal and restart the "
        "program.";
    const std::string color_error_msg = "Your terminal does not support colors.";

    ~Renderer();
    Renderer();

    void validateTerminalSize(int min_width, int min_height);
    void validateAndStartColor();

    static WINDOW *createWindow(int height, int width, int start_x, int start_y,
                                std::string title = "");
    static void destroyWindow(WINDOW *window);
    static void fillWindow(WINDOW *window, int attribute, char character);
    static void clearWindow(WINDOW *window);

    // default ncurses printw are uncomfortable, so create own
    static void attrMoveWindowPrint(WINDOW *window, attr_t attribute, int x,
                                    int y, std::string line);
    static void attrMovePrint(attr_t attribute, int x, int y, std::string line);
    static void attrWindowPrint(WINDOW *window, attr_t attribute,
                                std::string line);
    static void attrPrint(attr_t attribute, std::string line);
    static void moveWindowPrint(WINDOW *window, int x, int y, std::string line);
    static void movePrint(int x, int y, std::string line);
    static void windowPrint(WINDOW *window, std::string line);
    static void print(std::string line);
};
