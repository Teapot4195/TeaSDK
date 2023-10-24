//
// Created by teapot on 24/10/23.
//

#include "Argparse.h"

#include <utility>
#include <iostream>

namespace argparse {
    Parser::Parser(std::unordered_map<std::string, std::pair<bool, std::string>> args,
                   std::unordered_map<std::string, std::string> short_form_expansion, int argc, char **argv) :
                   args(std::move(args)),
                   short_form_expansion(std::move(short_form_expansion)),
                   argc(argc),
                   argv(argv)
    {}

    std::vector<Arg> Parser::ParseArgs() {
        std::string program_name = argv[0];
        std::vector<Arg> result;

        std::size_t positional_id;
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--help" || arg == "-h") {
                PrintHelp();
            }

            if (arg.starts_with("-")) {
                if (!arg.starts_with("--")) {
                    if (!short_form_expansion.contains(arg)) {
                        std::cout << "Unknown Short Argument \"" << arg << "\", --help for more information" << std::endl;
                        std::exit(1);
                    }
                    arg = short_form_expansion[arg];
                }
                if (!args.contains(arg)) {
                    std::cout << "Unknown argument \"" << arg << "\", --help for more information" << std::endl;
                    std::exit(1);
                } else {
                    if (args[arg].first) {
                        result.emplace_back(false, true, 0, arg, "");
                    } else if (i + 1 < argc) {
                        result.emplace_back(false, false, 0, arg, argv[i + 1]);
                        i++;
                    } else {
                        std::cout << "Argument \"" << arg << "\" requires an additional value, --help for more information" << std::endl;
                        std::exit(1);
                    }
                }
            } else {
                result.emplace_back(true, true, positional_id, "", arg);
            }
        }

        return result;
    }

    void Parser::PrintHelp() {
        // TODO: dynamically print the help menu from is_switch!
        std::exit(1);
    }
}
