//
// Created by teapot on 24/10/23.
//

#ifndef TEASDK_ARGPARSE_H
#define TEASDK_ARGPARSE_H

#include <string>
#include <vector>
#include <unordered_map>

namespace argparse {
/**
 * @brief parsed argument data
 */
    struct Arg {
        bool positional;
        bool is_switch;
        std::size_t id;
        std::string key;
        std::string value;
    };

    class Parser {
    public:
        Parser(std::unordered_map<std::string, std::pair<bool, std::string>> args,
               std::unordered_map<std::string, std::string> short_form_expansion,
               int argc,
               char** argv);

        std::vector<Arg> ParseArgs();

        void PrintHelp();

    private:
        std::unordered_map<std::string, std::pair<bool, std::string>> args;
        std::unordered_map<std::string, std::string> short_form_expansion;
        int argc;
        char** argv;
    };
}

#endif //TEASDK_ARGPARSE_H
