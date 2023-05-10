#pragma once

#include <ncurses.h>

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

#include "renderer/renderer.h"
#include "utility/timer.h"

class Renderer {
   public:
    /** Color types (initialized by `Renderer::validateAndStartColor`) */
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

    static const std::vector<Renderer::ColorPair> color_pairs;

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
    void validateTerminalResolution(const size_t min_width, const size_t min_height);

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
    static WINDOW *createWindow(const size_t height, const size_t width,
                                const size_t start_x, const size_t start_y,
                                const std::string &title = "");

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
    static void fillWindow(WINDOW *window, const int attribute,
                           const char character);

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
    static void attrMoveWindowPrint(WINDOW *window, const attr_t attribute,
                                    const size_t x, const size_t y,
                                    const std::string &line);

    /**
     * @brief Print `line` with `attribute` in a window
     *
     * @param window
     * @param attribute
     * @param line
     */
    static void attrWindowPrint(WINDOW *window, const attr_t attribute,
                                const std::string &line);

    /**
     * @brief Move cursor and print `line` in a window
     *
     * @param window
     * @param x
     * @param y
     * @param line
     */
    static void moveWindowPrint(WINDOW *window, const size_t x, const size_t y,
                                const std::string &line);
};
