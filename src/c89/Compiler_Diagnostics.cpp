//
// Created by teapot on 26/10/23.
//

#include "Compiler_Diagnostics.h"

#include <unordered_map>

#include <liblog/logger.h>

namespace tcc89 {
    Compiler_Diagnostics diagnostics;

    std::unordered_map<Diagnostic_type, std::string> diag_map {
            {Diagnostic_type::Integer_Constant_Overflow, "Integer Constant Overflow"},
            {Diagnostic_type::Oversized_Character_Constant, "Character Constant Oversized"},
            {Diagnostic_type::Undersized_Character_Constant, "Character Constant Undersized"},
            {Diagnostic_type::Undersized_Character_Escape, "Character Escape Undersized"},
            {Diagnostic_type::Oversized_Character_Escape, "Character Escape Oversized"},
            {Diagnostic_type::Bad_Trigraph, "Unrecognized trigraph sequence"},
            {Diagnostic_type::Bad_Escape, "Unregnized Escape sequence"},

            {Diagnostic_type::I_ICE, "Internal Compiler Error"}
    };

    void Compiler_Diagnostics::add_diagnostic(const Diagnostic& diag) {
        diags.push_back(diag);
    }

    void Compiler_Diagnostics::print_diagnostics() {
        for (const Diagnostic& entry: diags) {
            print_diagnostic(entry);
        }
    }

    void Compiler_Diagnostics::print_diagnostic(const Diagnostic &diag) {
        info("");
        info("at file " + diag.file + ":" + std::to_string(diag.linecount));
        info("due to " + diag_map[diag.type]);
        info(diag.message);
        info("at " + diag.raised_by);
    }

    void ICE(const IICE_DIAGNOSTIC& diag) {
        diagnostics.print_diagnostics();

        fatal("Internal Compiler Error");

        diagnostics.print_diagnostic(diag.diag);

        info("The stacktrace is as follows");

        info("\n" + boost::stacktrace::to_string(diag.stacktrace));
    }
}
