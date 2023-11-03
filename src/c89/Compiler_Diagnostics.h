//
// Created by teapot on 26/10/23.
//

#ifndef TEASDK_COMPILER_DIAGNOSTICS_H
#define TEASDK_COMPILER_DIAGNOSTICS_H

#include <string>
#include <vector>
#include <cstdint>

#include <boost/stacktrace/stacktrace.hpp>

namespace tcc89 {
    enum class Diagnostic_type {
        Integer_Constant_Overflow,
        Oversized_Character_Constant,
        Undersized_Character_Constant,
        Undersized_Character_Escape,
        Oversized_Character_Escape,
        Bad_Trigraph,
        Bad_Escape,
        Unrecognized_Character,

        //Internal Compiler Error
        I_ICE
    };

    struct Diagnostic {
        std::string file;
        std::uint64_t linecount;
        Diagnostic_type type;
        std::string message;
        std::string raised_by;
    };

    struct IICE_DIAGNOSTIC {
        Diagnostic diag;
        boost::stacktrace::stacktrace stacktrace;
    };

    class Compiler_Diagnostics {
    public:
        /**
         * @brief prints the collected diagnostics
         */
        void print_diagnostics();

        /**
         * @brief adds diagnostics to print
         *
         * @param diag the diagnostic to add to the list
         */
        void add_diagnostic(const Diagnostic& diag);

        void print_diagnostic(const Diagnostic& diag);
    private:
        std::vector<Diagnostic> diags;
    };

    extern Compiler_Diagnostics diagnostics;

    void ICE(const IICE_DIAGNOSTIC& diag);
}

#endif //TEASDK_COMPILER_DIAGNOSTICS_H
