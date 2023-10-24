#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <liblog/logger.h>
#include <argparse/Argparse.h>

std::unordered_map<std::string, std::pair<bool, std::string>> arg_is_switch {
        {"--output", {false, "the location of the output file"}}
};

std::unordered_map<std::string, std::string> short_form_expansion {
        {"-o", "--output"}
};

int main(int argc, char** argv) {
    argparse::Parser argument_parser = argparse::Parser(arg_is_switch, short_form_expansion, argc, argv);
    std::vector<argparse::Arg> args = argument_parser.ParseArgs();
    std::vector<argparse::Arg> positional;
    std::vector<argparse::Arg> other;
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
        argument_parser.PrintHelp();
    }

    return 0;
}
