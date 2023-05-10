#include "utility/cmd_options.h"
#include <ostream>

CmdOptions::CmdOptions(int argc, char **argv) {
    this->arguments = std::vector<std::string>(argv, argv + argc);
}

void CmdOptions::printHelp(std::vector<CmdOptions::Option> options) {
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
              << color_reset << std::endl << std::endl;

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

    for (int i = 0; i < (int)options.size(); i++) {
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

std::vector<std::string>::iterator CmdOptions::getOption(std::string option,
                                                         bool is_value_required) {
    auto found =
        std::find(this->arguments.begin(), this->arguments.end(), option);

    if (found == this->arguments.end() || !is_value_required ||
        found + 1 == this->arguments.end()) {
        return found;
    }

    return found;
}

bool CmdOptions::isOptionExists(CmdOptions::Option option) {
    return this->getOption("-" + option.short_cmd, option.is_value_required) !=
               this->arguments.end() ||
           this->getOption("--" + option.long_cmd, option.is_value_required) !=
               this->arguments.end();
}

std::string CmdOptions::getOptionValue(CmdOptions::Option option) {
    if (!option.is_value_required || !this->isOptionExists(option)) {
        return "";
    }

    auto value = this->getOption("-" + option.short_cmd, true);

    if (value != this->arguments.end()) {
        return *(value + 1);
    }

    value = this->getOption("--" + option.long_cmd, true);

    if (value != this->arguments.end()) {
        return *(value + 1);
    }

    return "";
}
