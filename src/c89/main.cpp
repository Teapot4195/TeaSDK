#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <liblog/logger.h>

struct Arg {
    bool positional;
    bool is_switch;
    std::size_t id;
    std::string key;
    std::string value;
};

std::unordered_map<std::string, std::pair<bool, std::string>> arg_is_switch {
        {"--output", {false, "the location of the output file"}}
};

std::unordered_map<std::string, std::string> short_form_expansion {
        {"-o", "--output"}
};

void PrintHelp(int argc, char** argv) {
    // TODO: dynamically print the help menu from is_switch!
    std::exit(1);
}

std::vector<Arg> ParseArgs(int argc, char** argv) {
    std::string program_name = argv[0];
    std::vector<Arg> args;

    std::size_t positional_id;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintHelp(argc, argv);
        }

        if (arg.starts_with("-")) {
            if (!arg.starts_with("--")) {
                if (!short_form_expansion.contains(arg)) {
                    std::cout << "Unknown Short Argument \"" << arg << "\", --help for more information" << std::endl;
                    std::exit(1);
                }
                arg = short_form_expansion[arg];
            }
            if (!arg_is_switch.contains(arg)) {
                std::cout << "Unknown argument \"" << arg << "\", --help for more information" << std::endl;
                std::exit(1);
            } else {
                if (arg_is_switch[arg].first) {
                    args.emplace_back(false, true, 0, arg, "");
                } else if (i + 1 < argc) {
                    args.emplace_back(false, false, 0, arg, argv[i + 1]);
                    i++;
                } else {
                    std::cout << "Argument \"" << arg << "\" requires an additional value, --help for more information" << std::endl;
                    std::exit(1);
                }
            }
        } else {
            args.emplace_back(true, true, positional_id, "", arg);
        }
    }

    return args;
}

int main(int argc, char** argv) {
    std::vector<Arg> args = ParseArgs(argc, argv);
    std::vector<Arg> positional;
    std::vector<Arg> other;
    bool has_positional = false;

    for (auto & arg : args) {
        if (arg.positional) {
            has_positional = true;
            break;
        }
    }

    if (has_positional) {

        //TODO: Compiling files
    } else {
        warn("at least one input file must be provided.");
        PrintHelp(argc, argv);
    }

    return 0;
}
