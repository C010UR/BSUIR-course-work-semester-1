#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Terminal {
   public:
    struct Option {
        std::string short_cmd;
        std::string long_cmd;
        bool is_value_required = false;
        std::string prefix;
        std::string default_value;
        std::string description;
    };

    enum Options {
        HELP,
        TRAVERSE_DELAY,
        STEP_DELAY,
        PARALLEL,
        BREADTH_FIRST_SEARCH_ALGORITHM,
        DIJKSTRA_ALGORITHM,
        A_STAR_ALGORITHM
    };

    static const std::vector<Terminal::Option> options;
    std::vector<std::string> arguments;

    /**
     * @brief Construct a new Cmd Options object and parses `argv`
     *
     * @param argc
     * @param argv
     */
    Terminal(int argc, char **argv);

    /**
     * @brief Print help for the list of commands and exit
     *
     * @param commands
     * @param is_argument_error
     */
    void help(std::vector<Terminal::Option> commands);

    /**
     * @brief Print error message and exit
     *
     * @param error
     */
    void error(std::string error);

    /**
     * @brief Check if option object exists and has value (if option requires
     * it)
     *
     * @param option
     * @return true
     * @return false
     */
    bool isOptionExists(Terminal::Option option);

    /**
     * @brief Get pointer to option if it is present and has value (if option
     * requires it)
     *
     * @param option
     * @param has_value
     * @return std::vector<std::string>::iterator
     */
    std::vector<std::string>::iterator getOption(
        std::string option, bool is_value_required = false);

    /**
     * @brief Get option value if present, otherwise fail
     *
     * @tparam T
     * @param option
     * @param _default - if option was not specified
     * @return T
     */
    template <typename T>
    T getOptionValue(Terminal::Option option, T _default) {
        if (!option.is_value_required || !this->isOptionExists(option)) {
            return _default;
        }

        auto value = this->getOption("-" + option.short_cmd, true);

        if (value != this->arguments.end()) {
            return Terminal::convertString<T>(*(value + 1));
        }

        value = this->getOption("--" + option.long_cmd, true);

        if (value != this->arguments.end()) {
            return Terminal::convertString<T>(*(value + 1));
        }

        throw std::invalid_argument(
            "Argument error: Cannot get value for option '" + option.short_cmd +
            (option.short_cmd.empty() && option.long_cmd.empty() ? "" : ", ") +
            option.long_cmd + "'.");
    }

   private:
    /**
     * @brief Convert string to T
     *
     * @tparam T
     * @param data
     * @return T
     */
    template <typename T>
    static T convertString(const std::string &data) {
        if (!data.empty()) {
            T result;
            std::istringstream iss(data);
            if (data.find("0x") != std::string::npos) {
                iss >> std::hex >> result;
            } else {
                iss >> std::dec >> result;
            }

            if (iss.fail()) {
                throw std::invalid_argument(
                    "Argument convert error: Cannot convert string '" + data +
                    "' to value.");
                return T();
            }
            return result;
        }
        return T();
    }
};
