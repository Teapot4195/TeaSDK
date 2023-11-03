#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <liblog/logger.h>
#include <argparse/Argparse.h>

#include "Compiler_Diagnostics.h"
#include "Tokenizer.h"

namespace tcc89 {
    std::vector<argparse::Arg> args;

    std::unordered_map<std::string, std::pair<bool, std::string>> arg_is_switch{
            {"--output", {false, "the location of the output file"}}
    };

    std::unordered_map<std::string, std::string> short_form_expansion{
            {"-o", "--output"}
    };

    int main(int argc, char **argv) {
        argparse::Parser argument_parser = argparse::Parser(arg_is_switch, short_form_expansion, argc, argv);
        args = argument_parser.ParseArgs();
        std::vector<argparse::Arg> positional;
        std::vector<argparse::Arg> other;
        bool has_positional = false;

        for (auto &arg: args) {
            if (arg.positional) {
                has_positional = true;
                positional.push_back(arg);
            } else {
                other.push_back(arg);
            }
        }

        if (has_positional) {
            for (auto &file : positional) {
                Tokenizer tokenize(preprocessor_symbols{}, file.value);
                std::vector<Token> tokens = tokenize.tokenize();

                tokenize.print(tokens);

                // TODO: Done tokenize, need parsing
            }
        } else {
            warn("at least one input file must be provided.");
            argument_parser.PrintHelp();
        }

        diagnostics.print_diagnostics();

        return 0;
    }
}

int main(int argc, char **argv) {
    return tcc89::main(argc, argv);
}
