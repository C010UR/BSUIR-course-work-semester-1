#include "utility/terminal.h"

const std::vector<Terminal::Option> Terminal::options = {
    {"h", "help",                    false, "",               "",     "Show this help message"                  },
    {"t", "traverse-delay",          true,  "",               "40ms", "Set path traverse step (in milliseconds)"},
    {"d", "step-delay",              true,  "",               "1ms",  "Set step delay (in milliseconds)"        },
    {"p", "parallel",                false, "",               "",     "Toggle path parallel draw"               },
    {"",  "breadth-first-search",    false, "pathfinder",     "",     "Breadth First Search Algorithm"          },
    {"",  "dijkstra",                false, "pathfinder",     "",     "Dijkstra Search Algorithm"               },
    {"",  "a-star",                  false, "pathfinder",     "",     "A* Search Algorithm"                     },
    {"",  "maze-depth-first-search", false, "maze generator", "",     "Depth First Search Maze Generator"       },
    {"",  "maze-block",              false, "maze generator", "",     "Block Maze Generator"                    }
};

Terminal::Terminal(int argc, char **argv)
{
    this->arguments = std::vector<std::string>(argv, argv + argc);

    if (this->isOptionExists(this->options[Terminal::Options::HELP]))
    {
        this->help(this->options);
    }
}

void Terminal::help(std::vector<Terminal::Option> options) const
{
    // don't use ncurses
    std::string color_reset  = "\033[0m";
    std::string color_red    = "\033[31m";
    std::string color_green  = "\033[32m";
    std::string color_yellow = "\033[33m";
    std::string color_blue   = "\033[34m";
    std::string color_cyan   = "\033[36m";

    std::cout << "This project shows the difference between path finding algorithms." << std::endl
              << color_red << "If program halts, press any button to continue/exit." << color_reset << std::endl
              << std::endl;

    if (options.empty())
    {
        exit(EXIT_SUCCESS);
    }

    size_t description_space = 0;

    // Find longest long option
    for (auto command : options)
    {
        if (command.long_cmd.size() > description_space)
        {
            description_space = command.long_cmd.size();
        }
    }

    std::cout << "Options: " << std::endl;

    for (size_t i = 0; i < options.size(); i++)
    {
        auto current = options[i];

        std::cout << "\t";

        if (!current.short_cmd.empty())
        {
            std::cout << color_blue << "-" << current.short_cmd << color_reset;

            if (current.is_value_required)
            {
                std::cout << color_yellow << " VALUE" << color_reset;
            }

            // Place comma if needed

            if (!current.long_cmd.empty())
            {
                std::cout << (current.is_value_required ? ", " : ", " + std::string(6, ' '));
            }
        }
        else
        {
            std::cout << std::string(10, ' ');
        }

        if (!current.long_cmd.empty())
        {
            std::cout << color_blue << "--" << current.long_cmd << color_reset;

            if (current.is_value_required)
            {
                std::cout << color_yellow << " VALUE" << color_reset;
            }
        }
        else
        {
            std::cout << std::string(10, ' ');
        }

        std::cout << std::string(description_space - current.long_cmd.size() - (current.is_value_required ? 6 : 0), ' ')
                  << " - ";

        std::cout << color_cyan << (current.prefix.empty() ? "" : "[" + current.prefix + "] ") << color_reset;

        std::cout << current.description;

        std::cout << color_green
                  << (current.default_value.empty() ? "" : " [Defaults to " + current.default_value + "]")
                  << color_reset;

        std::cout << std::endl;
    }

    exit(EXIT_SUCCESS);
}

void Terminal::error(std::string error) const
{
    std::cout << "\033[31m" << error << "\033[0m" << std::endl;
}

std::vector<std::string>::const_iterator Terminal::getOption(std::string option, bool is_value_required) const
{
    auto found = std::find(this->arguments.begin(), this->arguments.end(), option);

    if (found == this->arguments.end() || !is_value_required || found + 1 == this->arguments.end())
    {
        return found;
    }

    return found;
}

bool Terminal::isOptionExists(Terminal::Option option) const
{
    return this->getOption("-" + option.short_cmd, option.is_value_required) != this->arguments.end()
           || this->getOption("--" + option.long_cmd, option.is_value_required) != this->arguments.end();
}
