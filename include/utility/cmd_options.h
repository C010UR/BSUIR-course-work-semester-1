#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class CmdOptions {
   public:
    struct Option {
        std::string short_cmd;
        std::string long_cmd;
        bool is_value_required = false;
        std::string description;
    };

    std::vector<std::string> arguments;

    /**
     * @brief Construct a new Cmd Options object and parses `argv`
     *
     * @param argc
     * @param argv
     */
    CmdOptions(int argc, char **argv);

    /**
     * @brief Print help for the list of commands
     *
     * @param commands
     * @param is_argument_error
     */
    void printHelp(std::vector<CmdOptions::Option> commands);

    /**
     * @brief Get option value if present, otherwise return an empty string
     *
     * @param option
     * @return std::string
     */
    std::string getOptionValue(CmdOptions::Option option);

    /**
     * @brief Check if option object exists and has value (if option requires
     * it)
     *
     * @param option
     * @return true
     * @return false
     */
    bool isOptionExists(CmdOptions::Option option);

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
};
