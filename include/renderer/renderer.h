#pragma once

#include <ncurses.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "renderer/renderer.h"
#include "utility/timer.h"

class Renderer {
   public:
    /** Available color types (initialized by `Renderer::validateAndStartColor`)
     */
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

    // error messages;
    const std::string size_error_msg =
        "Terminal size is too small.\n\nEnlarge the terminal and restart the "
        "program.";
    const std::string color_error_msg =
        "Your terminal does not support colors.";

    /**
     * @brief Destroy the Renderer object and end `ncurses` mode
     *
     */
    ~Renderer();

    /**
     * @brief Construct a new Renderer object and start `ncurses` mode
     *
     */
    Renderer();

    /**
     * @brief Validate terminal size, exits on failure
     *
     * @param min_width
     * @param min_height
     */
    void validateTerminalSize(int min_width, int min_height);

    /**
     * @brief Validate if terminal supports colors and starts `ncurses` color
     * mode, exits on failure
     *
     */
    void validateColor();

    /**
     * @brief Create `ncurses` window with borders and a title
     *
     * @param height
     * @param width
     * @param start_x
     * @param start_y
     * @param title
     * @return WINDOW*
     */
    static WINDOW *createWindow(int height, int width, int start_x, int start_y,
                                std::string title = "");

    /**
     * @brief Destroy `ncurses` windows and clears window on the screen
     *
     * @param window
     */
    static void destroyWindow(WINDOW *window);

    /**
     * @brief Fill window (excluding borders) with `character` with `attribute`
     *
     * @param window
     * @param attribute
     * @param character
     */
    static void fillWindow(WINDOW *window, int attribute, char character);

    /**
     * @brief Clear window excluding borders
     *
     * @param window
     */
    static void clearWindow(WINDOW *window);

    // default ncurses printw are uncomfortable, so create own
    /**
     * @brief Move cursor and print `line` with `attribute` in a window
     *
     * @param window
     * @param attribute
     * @param x
     * @param y
     * @param line
     */
    static void attrMoveWindowPrint(WINDOW *window, attr_t attribute, int x,
                                    int y, std::string line);

    /**
     * @brief Print `line` with `attribute` in a window
     *
     * @param window
     * @param attribute
     * @param line
     */
    static void attrWindowPrint(WINDOW *window, attr_t attribute,
                                std::string line);

    /**
     * @brief Move cursor and print `line` in a window
     *
     * @param window
     * @param x
     * @param y
     * @param line
     */
    static void moveWindowPrint(WINDOW *window, int x, int y, std::string line);
};
