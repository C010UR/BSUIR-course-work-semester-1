#include "utility/terminal.h"

const std::vector<Terminal::Option> Terminal::options = {
    {"h", "help", false, "Show this help message"},
    {"t", "traverse-delay", true, "Set path traverse step (in milliseconds)"},
    {"d", "step-delay", true, "Set step delay (in milliseconds)"},
    {"p", "parallel", false, "Toggle path parallel draw"},
    {"", "breadth-first-search", false,
     "Include Breadth First Search Algorithm to comparison"},
    {"", "dijkstra", false, "Include Dijkstra Search Algorithm to comparison"},
    {"", "a-star", false, "Include A* Search Algorithm to comparison"}};

Terminal::Terminal(int argc, char **argv) {
    this->arguments = std::vector<std::string>(argv, argv + argc);

    if (this->isOptionExists(this->options[Terminal::Options::HELP])) {
        this->help(this->options);
    }
}

void Terminal::help(std::vector<Terminal::Option> options) {
    // don't use ncurses
    std::string color_reset = "\033[0m";
    std::string color_red = "\033[31m";
    std::string color_yellow = "\033[33m";
    std::string color_blue = "\033[34m";

    std::cout << "This project shows the difference between 2 very popular "
                 "path finding algorithms: "
              << color_yellow << "A*" << color_reset << " and " << color_yellow
              << "Dijkstra" << color_reset << std::endl
              << color_red
              << "If program halts, press any button to continue/exit."
              << color_reset << std::endl
              << std::endl;

    if (options.empty()) {
        exit(EXIT_SUCCESS);
    }

    size_t description_space = 0;

    // Find longest long option
    for (auto command : options) {
        if (command.long_cmd.size() > description_space) {
            description_space = command.long_cmd.size();
        }
    }

    std::cout << "Options: " << std::endl;

    for (size_t i = 0; i < options.size(); i++) {
        auto current = options[i];

        std::cout << "\t";

        if (!current.short_cmd.empty()) {
            std::cout << color_blue << "-" << current.short_cmd << color_reset;

            if (current.is_value_required) {
                std::cout << color_yellow << " VALUE" << color_reset;
            }

            // Place comma if needed

            if (!current.long_cmd.empty()) {
                std::cout << (current.is_value_required ? ", " : ",       ");
            }
        }

        if (!current.long_cmd.empty()) {
            std::cout << color_blue << "--" << current.long_cmd << color_reset;

            if (current.is_value_required) {
                std::cout << color_yellow << " VALUE" << color_reset;
            }
        }

        std::cout << std::string(description_space - current.long_cmd.size() +
                                     (current.is_value_required ? 0 : 6),
                                 ' ')
                  << " - " << current.description << std::endl;
    }

    exit(EXIT_SUCCESS);
}

void Terminal::error(std::string error) {
    std::cout << "\033[31m" << error << "\033[0m" << std::endl;
}

std::vector<std::string>::iterator Terminal::getOption(std::string option,
                                                       bool is_value_required) {
    auto found =
        std::find(this->arguments.begin(), this->arguments.end(), option);

    if (found == this->arguments.end() || !is_value_required ||
        found + 1 == this->arguments.end()) {
        return found;
    }

    return found;
}

bool Terminal::isOptionExists(Terminal::Option option) {
    return this->getOption("-" + option.short_cmd, option.is_value_required) !=
               this->arguments.end() ||
           this->getOption("--" + option.long_cmd, option.is_value_required) !=
               this->arguments.end();
}
