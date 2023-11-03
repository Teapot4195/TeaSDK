//
// Created by teapot on 25/10/23.
//

#include "Tokenizer.h"
#include "Compiler_Diagnostics.h"

#include <liblog/logger.h>
#include <argparse/Argparse.h>

#include <utility>
#include <sstream>
#include <codecvt>
#include <locale>
#include <unordered_set>

namespace tcc89 {
    extern std::vector<argparse::Arg> args;

    std::unordered_set<std::string> keywords_set {
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while"
    };

    std::unordered_map<char, Token_Type> separator_map {
            {'(', Tok_Paren_Open},
            {')', Tok_Paren_Close},
            {'[', Tok_Square_Open},
            {']', Tok_Square_Close},
            {'{', Tok_Curly_Open},
            {'}', Tok_Curly_Close},
            {'<', Tok_Angle_Open},
            {'>', Tok_Angle_Close},
            {'=', Tok_EQ},
            {';', Tok_Semicolon},
            {'*', Tok_Star},
            {',', Tok_Comma},
            {'.', Tok_Period},
            {'/', Tok_Slash},
            {'%', Tok_Percent},
            {'+', Tok_Plus},
            {'-', Tok_Minus},
            {'&', Tok_Ampersand},
            {'^', Tok_Hat},
            {'|', Tok_Pipe},
            {'?', Tok_Question},
            {':', Tok_Colon},
            {'!', Tok_Excl},
            {'~', Tok_Tilde}
    };

    std::wstring s2ws(const std::string& str)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    std::string ws2s(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }

    Tokenizer::Tokenizer(preprocessor_symbols symbols, const std::string &file) :
        Tokenizer(std::move(symbols), std::ifstream(file), file) {}
    Tokenizer::Tokenizer(preprocessor_symbols symbols, std::ifstream file, std::string filepath) :
        table(std::move(symbols)), file_stream(std::move(file)), filepath(std::move(filepath)) {}

    void Tokenizer::load_file(const std::string &file) {
        file_stream.open(file);
    }

    void Tokenizer::load_file(std::ifstream file) {
        file_stream = std::move(file);
    }

    std::vector<Token> Tokenizer::tokenize() {
        std::vector<Token> tokens;
        std::size_t linecount = 1;
        int c, n; //c is current character, n is next
        bool SOL = true;
        std::stringstream fs;

        while (!file_stream.eof()) {
            c = file_stream.get();
            n = file_stream.peek();

            if (c == '\n')
                linecount++;

            if (c == '?' && n == '?') {
                file_stream.get();
                c = file_stream.get();
                switch (c) {
                    case '=':
                        c = '#';
                        break;
                    case '/':
                        c = '\\';
                        break;
                    case '\'':
                        c = '^';
                        break;
                    case '(':
                        c = '[';
                        break;
                    case ')':
                        c = ']';
                        break;
                    case '!':
                        c = '|';
                        break;
                    case '<':
                        c = '{';
                        break;
                    case '>':
                        c = '}';
                        break;
                    case '-':
                        c = '~';
                        break;
                    default:
                        diagnostics.add_diagnostic({
                            filepath,
                            linecount,
                            Diagnostic_type::Bad_Trigraph,
                            "Trigraph \'??" + std::string(1, (char)c) + '\'',
                            "Tokenize@Parse Trigraph"
                        });
                        break;
                }
            }

            fs.put((char)c);
        }

        fs.put(EOF);

        std::cout << fs.str() << std::endl;

        linecount = 1;
        fs.flush();

        while (true) {
            c = fs.get();
            n = fs.peek();

            if (c == EOF) {
                break;
            }

            /* After whitespace, it is still start of line */
            if (c == ' ' || c == '\t' || c == '\v') {
                // Eat the rest of the whitespace until another character is encountered
                while ((c = fs.peek()) && (c == ' ' || c == '\t' || c == '\v'))
                    fs.get();

                tokens.emplace_back(Tok_Whitespace);
                continue;
            }

            // Newlines
            if (c == '\n' || c == '\f' || c == '\r') {
                linecount++;
                tokens.emplace_back(Tok_Newline, false, "", linecount);
                SOL = true;
                continue;
            }

            // Preprocessor directives
            if (SOL && c == '#') {
                SOL = false;

                std::string preproc;

                while (true) {
                    c = fs.get();

                    if (c == '\\') {
                        n = fs.peek();

                        if (n == '\n' || n == '\f' || n == '\r') {
                            linecount++;
                            continue;
                        }
                    }

                    if (c == '\n' || c == '\f' || c == '\r') {
                        linecount++;
                        break;
                    }

                    preproc += (char)c;
                }

                tokens.emplace_back(Tok_Preprocessor, false, preproc);
                tokens.emplace_back(Tok_Newline, false, "", linecount);

                continue;
            }

            /* all the stuff that could need Start of line is done, and it is definitely no longer a start of line.
             */
            SOL = false;

            // Folded newlines
            if (c == '\\' && (n == '\n' || n == '\f' || n == '\r')) {
                linecount++;
                tokens.emplace_back(Tok_Newline_Fold, false, "", linecount);
                continue;
            }

            // Comments
            if (c == '/' && n == '*') {
                std::string comment;

                comment += '/';
                comment += (char)fs.get();

                while (true) {
                    c = fs.get();

                    if (c == '*') {
                        n = fs.peek();

                        if (n == '/') {
                            fs.get();
                            comment += "*/";
                            break;
                        }
                    } else if (c == '\n' || c == '\f' || c == '\r') {
                        linecount++;
                    }

                    comment += (char)c;
                }

                tokens.emplace_back(Tok_Comment, false, comment);
                continue;
            }

            // Integer literals
            // TODO: Implement floats
            if (c >= '0' && c <= '9') {
                // Because ASCII digits are in order and continuous this case will cover all digits 0-9

                Token entry {Tok_IntConstant};

                std::uint8_t buf;
                std::uint64_t value = 0;
                unsigned int count = 0;

                if (n == 'x' || n == 'X') { // It's an int constant in HEX format
                    fs.get();
                    while ((buf = fs.get())) {
                        if (buf >= '0' && buf <= '9') {
                            buf -= '0';
                        } else if (buf >= 'A' && buf <= 'F') {
                            buf -= 'A';
                            buf += 10;
                        } else if (buf >= 'a' && buf <= 'f') {
                            buf -= 'a';
                            buf += 10;
                        } else {
                            fs.unget();
                            break;
                        }

                        value <<= 4;
                        value &= 0xFFFFFFFFFFFFFFF0;
                        value |= buf;
                        count++;
                    }

                    if (count >= 16)
                        diagnostics.add_diagnostic({
                            filepath,
                            linecount,
                            Diagnostic_type::Integer_Constant_Overflow,
                            "Integer constant may have overflowed 64bit integer implementation limit",
                            "Tokenize@Parse IntConst as Hex, -f-warn-integer-overflow"
                        });
                } else if (c == '0' && n >= '0' && n <= '9') { //It's an int constant in octal format
                    while ((buf = fs.get())) {
                        if (buf >= '0' && buf <= '9') {
                            buf -= '0';
                        } else {
                            fs.unget();
                            break;
                        }

                        value <<= 3;
                        value &= 0xFFFFFFFFFFFFFFF8;
                        value |= buf;
                        count += 3;
                    }

                    if (count >= 64)
                        diagnostics.add_diagnostic({
                            filepath,
                            linecount,
                            Diagnostic_type::Integer_Constant_Overflow,
                            "Integer constant may have overflowed 64bit integer implementation limit",
                            "Tokenize@Parse IntConst as Octal, -f-warn-integer-overflow"
                        });
                } else { //It's an int in decimal format
                    while ((buf = fs.get())) {
                        if (buf >= '0' && buf <= '9') {
                            buf -= '0';
                        } else {
                            fs.unget();
                            break;
                        }

                        value *= 10;
                        value += buf;
                        count++;
                    }

                    if (count >= 20)
                        diagnostics.add_diagnostic({
                            filepath,
                            linecount,
                            Diagnostic_type::Integer_Constant_Overflow,
                            "Integer constant may have overflowed 64bit integer implementation limit",
                            "Tokenize@Parse IntConst as Decimal, -f-warn-integer-overflow"
                        });
                }

                for (int i = 0; i < 3; i++) {
                    c = fs.get();
                    if (c == 'u' || c == 'U') {
                        entry.UNSIGNED = true;
                    } else if (c == 'l' || c == 'L') {
                        entry.L = true;
                    } else {
                        fs.unget();
                        break;
                    }
                }

                if (entry.UNSIGNED) {
                    entry.int_data = value;
                } else {
                    entry.int_data = (std::int64_t) value;
                }

                tokens.push_back(entry);
                continue;
            }

            if (c == 'L') {
                if (n == '\'') {
                    fs.get();
                    std::wstring b;
                    unsigned int count {0}, d;
                    int v, y, z;

                    // Although the while loop appears useless, it is here so the lexing can be resilient
                    while ((v = fs.get()) && v != '\'') {
                        d = 0;
                        y = 0;

                        if (v == '\\') {
                            v = fs.get();
                            if (v == 'x') {
                                while ((v = fs.get()) && ((v >= '0' && v <= '9') || (v >= 'a' && v <= 'f') || (v >= 'A' && v <= 'F'))) {
                                    z = v;
                                    if (v >= '0' && v <= '9') {
                                        z -= '0';
                                    } else if (v >= 'a' && v <= 'f') {
                                        z -= 'a';
                                        z += 10;
                                    } else if (v >= 'A' && v <= 'F') {
                                        z -= 'A';
                                        z += 10;
                                    }

                                    y <<= 4;
                                    y &= (int)0xFFFFFFF0;
                                    y |= z;

                                    d++;
                                }
                                fs.unget();

                                if (d < 0) {
                                    ICE({{
                                        filepath,
                                        linecount,
                                        Diagnostic_type::I_ICE,
                                        "Unsigned Integer is less than zero",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    }, boost::stacktrace::stacktrace()});
                                } else if (d == 0) {
                                    diagnostics.add_diagnostic({
                                        filepath,
                                        linecount,
                                        Diagnostic_type::Undersized_Character_Escape,
                                        "Hex escape code has no value",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    });
                                } else if (d > 2) {
                                    diagnostics.add_diagnostic({
                                        filepath,
                                        linecount,
                                        Diagnostic_type::Oversized_Character_Escape,
                                        "Hex escape code has too many values",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    });
                                }

                                b += (char)y;
                            } else if ((v >= '0' && v < '8')) {
                                fs.unget();
                                while ((v = fs.get()) && d <= 3 && v >= '0' && v < '8') {
                                    v -= '0';
                                    y <<= 3;
                                    y &= (int)0xFFFFFFF8;
                                    y |= v;
                                    d++;
                                }
                                fs.unget();

                                if (d < 0) {
                                     ICE({{
                                         filepath,
                                         linecount,
                                         Diagnostic_type::I_ICE,
                                         "Unsigned Integer is less than zero",
                                         "Tokenize@Parse Character constant@Parse octal escape"
                                     }, boost::stacktrace::stacktrace()});
                                }

                                b += (char)y;
                            } else {
                                switch (v) {
                                    case 'n':
                                        b += '\n';
                                        break;
                                    case 't':
                                        b += '\t';
                                        break;
                                    case 'v':
                                        b += '\v';
                                        break;
                                    case 'r':
                                        b += '\r';
                                        break;
                                    case 'f':
                                        b += '\f';
                                        break;
                                    case 'a':
                                        b += '\a';
                                        break;
                                    case '\\':
                                        b += '\\';
                                        break;
                                    case '?':
                                        b += '?';
                                        break;
                                    case '\'':
                                        b += '\'';
                                        break;
                                    case '\"':
                                        b += '\"';
                                        break;
                                    default:
                                        b += (char)v;
                                        diagnostics.add_diagnostic({
                                           filepath,
                                           linecount,
                                           Diagnostic_type::Bad_Escape,
                                           "Escape sequence of \'\\" + std::string(1, (char)v) + '\'',
                                           "Tokenize@Parse Character Constant@Parse Escape"
                                        });
                                        break;
                                }
                            }
                        }

                        count++;
                    }

                    tokens.emplace_back(
                        Tok_CharacterConstant,
                        true,
                        "",
                        std::monostate{},
                        false,
                        false,
                        false,
                        b[0]
                    );
                    continue;
                } else if (n == '\"') {
                    fs.get();
                    std::wstring b;
                    unsigned int count {0}, d;
                    int v, y, z;

                    while ((v = fs.get()) && v != '\"') {
                        d = 0;
                        y = 0;

                        if (v == '\\') {
                            v = fs.get();
                            if (v == 'x') {
                                while ((v = fs.get()) && ((v >= '0' && v <= '9') || (v >= 'a' && v <= 'f') || (v >= 'A' && v <= 'F'))) {
                                    z = v;
                                    if (v >= '0' && v <= '9') {
                                        z -= '0';
                                    } else if (v >= 'a' && v <= 'f') {
                                        z -= 'a';
                                        z += 10;
                                    } else if (v >= 'A' && v <= 'F') {
                                        z -= 'A';
                                        z += 10;
                                    }

                                    y <<= 4;
                                    y &= (int)0xFFFFFFF0;
                                    y |= z;

                                    d++;
                                }
                                fs.unget();

                                if (d < 0) {
                                    ICE({{
                                        filepath,
                                        linecount,
                                        Diagnostic_type::I_ICE,
                                        "Unsigned Integer is less than zero",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    }, boost::stacktrace::stacktrace()});
                                } else if (d == 0) {
                                    diagnostics.add_diagnostic({
                                        filepath,
                                        linecount,
                                        Diagnostic_type::Undersized_Character_Escape,
                                        "Hex escape code has no value",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    });
                                } else if (d > 2) {
                                    diagnostics.add_diagnostic({
                                        filepath,
                                        linecount,
                                        Diagnostic_type::Oversized_Character_Escape,
                                        "Hex escape code has too many values",
                                        "Tokenizer@Parse Character constant@Parse hex escape code"
                                    });
                                }

                                b += (char)y;
                            } else if ((v >= '0' && v < '8')) {
                                fs.unget();
                                while ((v = fs.get()) && d <= 3 && v >= '0' && v < '8') {
                                    v -= '0';
                                    y <<= 3;
                                    y &= (int)0xFFFFFFF8;
                                    y |= v;
                                    d++;
                                }
                                fs.unget();

                                if (d < 0) {
                                     ICE({{
                                         filepath,
                                         linecount,
                                         Diagnostic_type::I_ICE,
                                         "Unsigned Integer is less than zero",
                                         "Tokenize@Parse Character constant@Parse octal escape"
                                     }, boost::stacktrace::stacktrace()});
                                }

                                b += (char)y;
                            } else {
                                switch (v) {
                                    case 'n':
                                        b += '\n';
                                        break;
                                    case 't':
                                        b += '\t';
                                        break;
                                    case 'v':
                                        b += '\v';
                                        break;
                                    case 'r':
                                        b += '\r';
                                        break;
                                    case 'f':
                                        b += '\f';
                                        break;
                                    case 'a':
                                        b += '\a';
                                        break;
                                    case '\\':
                                        b += '\\';
                                        break;
                                    case '?':
                                        b += '?';
                                        break;
                                    case '\'':
                                        b += '\'';
                                        break;
                                    case '\"':
                                        b += '\"';
                                        break;
                                    default:
                                        b += (char)v;
                                        diagnostics.add_diagnostic({
                                           filepath,
                                           linecount,
                                           Diagnostic_type::Bad_Escape,
                                           "Escape sequence of \'\\" + std::string(1, (char)v) + '\'',
                                           "Tokenize@Parse Character Constant@Parse Escape"
                                        });
                                        break;
                                }
                            }
                        }

                        count++;
                    }

                    tokens.emplace_back(Tok_String, true, b);

                    continue;
                }
            }

            // Character constants
            if (c == '\'') {
                std::string b;
                unsigned int count {0}, d;
                int v, y, z;

                // Although the while loop appears useless, we put it here for resilient parsing,
                while ((v = fs.get()) && v != '\'') {
                    d = 0;
                    y = 0;

                    if (v == '\\') {
                        v = fs.get();
                        if (v == 'x') {
                            while ((v = fs.get()) && ((v >= '0' && v <= '9') || (v >= 'a' && v <= 'f') || (v >= 'A' && v <= 'F'))) {
                                z = v;
                                if (v >= '0' && v <= '9') {
                                    z -= '0';
                                } else if (v >= 'a' && v <= 'f') {
                                    z -= 'a';
                                    z += 10;
                                } else if (v >= 'A' && v <= 'F') {
                                    z -= 'A';
                                    z += 10;
                                }

                                y <<= 4;
                                y &= (int)0xFFFFFFF0;
                                y |= z;

                                d++;
                            }
                            fs.unget();

                            if (d < 0) {
                                ICE({{
                                    filepath,
                                    linecount,
                                    Diagnostic_type::I_ICE,
                                    "Unsigned Integer is less than zero",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                }, boost::stacktrace::stacktrace()});
                            } else if (d == 0) {
                                diagnostics.add_diagnostic({
                                    filepath,
                                    linecount,
                                    Diagnostic_type::Undersized_Character_Escape,
                                    "Hex escape code has no value",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                });
                            } else if (d > 2) {
                                diagnostics.add_diagnostic({
                                    filepath,
                                    linecount,
                                    Diagnostic_type::Oversized_Character_Escape,
                                    "Hex escape code has too many values",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                });
                            }

                            b += (char)y;
                        } else if ((v >= '0' && v < '8')) {
                            fs.unget();
                            while ((v = fs.get()) && d <= 3 && v >= '0' && v < '8') {
                                v -= '0';
                                y <<= 3;
                                y &= (int)0xFFFFFFF8;
                                y |= v;
                                d++;
                            }
                            fs.unget();

                            if (d < 0) {
                                ICE({{
                                    filepath,
                                    linecount,
                                    Diagnostic_type::I_ICE,
                                    "Unsigned Integer is less than zero",
                                    "Tokenize@Parse Character constant@Parse octal escape"
                                }, boost::stacktrace::stacktrace()});
                            }

                            b += (char)y;
                        } else {
                            switch (v) {
                                case 'n':
                                    b += '\n';
                                    break;
                                case 't':
                                    b += '\t';
                                    break;
                                case 'v':
                                    b += '\v';
                                    break;
                                case 'r':
                                    b += '\r';
                                    break;
                                case 'f':
                                    b += '\f';
                                    break;
                                case 'a':
                                    b += '\a';
                                    break;
                                case '\\':
                                    b += '\\';
                                    break;
                                case '?':
                                    b += '?';
                                    break;
                                case '\'':
                                    b += '\'';
                                    break;
                                case '\"':
                                    b += '\"';
                                    break;
                                default:
                                    b += (char)v;
                                    diagnostics.add_diagnostic({
                                        filepath,
                                        linecount,
                                        Diagnostic_type::Bad_Escape,
                                        "Escape sequence of \'\\" + std::string(1, (char)v) + '\'',
                                        "Tokenize@Parse Character Constant@Parse Escape"
                                    });
                                    break;
                            }
                        }
                    }

                    count++;
                }

                if (count < 0) {
                    ICE({{
                        filepath,
                        linecount,
                        Diagnostic_type::I_ICE,
                        "Unsigned Integer is less than zero",
                        "Tokenize@Parse Character constant",
                    }, boost::stacktrace::stacktrace()});
                } else if (count == 0) {
                    diagnostics.add_diagnostic({
                        filepath,
                        linecount,
                        Diagnostic_type::Undersized_Character_Constant,
                        "Character constant has 0 characters",
                        "Tokenize@Parse Character constant"
                    });
                } else if (count > 1) {
                    diagnostics.add_diagnostic({
                        filepath,
                        linecount,
                        Diagnostic_type::Oversized_Character_Constant,
                        "Character constant has greater than one characters",
                        "Tokenize@Parse Character constat",
                    });
                }

                tokens.emplace_back(
                    Tok_CharacterConstant,
                    false,
                    "",
                    std::monostate{},
                    false,
                    false,
                    false,
                    b[0]
                );
                continue;
            }

            if (c == '\"') {
                fs.get();
                std::string b;
                unsigned int count {0}, d;
                int v, y, z;

                while ((v = fs.get()) && v != '\"') {
                    d = 0;
                    y = 0;

                    if (v == '\\') {
                        v = fs.get();
                        if (v == 'x') {
                            while ((v = fs.get()) && ((v >= '0' && v <= '9') || (v >= 'a' && v <= 'f') || (v >= 'A' && v <= 'F'))) {
                                z = v;
                                if (v >= '0' && v <= '9') {
                                    z -= '0';
                                } else if (v >= 'a' && v <= 'f') {
                                    z -= 'a';
                                    z += 10;
                                } else if (v >= 'A' && v <= 'F') {
                                    z -= 'A';
                                    z += 10;
                                }

                                y <<= 4;
                                y &= (int)0xFFFFFFF0;
                                y |= z;

                                d++;
                            }
                            fs.unget();

                            if (d < 0) {
                                ICE({{
                                    filepath,
                                    linecount,
                                    Diagnostic_type::I_ICE,
                                    "Unsigned Integer is less than zero",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                }, boost::stacktrace::stacktrace()});
                            } else if (d == 0) {
                                diagnostics.add_diagnostic({
                                    filepath,
                                    linecount,
                                    Diagnostic_type::Undersized_Character_Escape,
                                    "Hex escape code has no value",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                });
                            } else if (d > 2) {
                                diagnostics.add_diagnostic({
                                    filepath,
                                    linecount,
                                    Diagnostic_type::Oversized_Character_Escape,
                                    "Hex escape code has too many values",
                                    "Tokenizer@Parse Character constant@Parse hex escape code"
                                });
                            }

                            b += (char)y;
                        } else if ((v >= '0' && v < '8')) {
                            fs.unget();
                            while ((v = fs.get()) && d <= 3 && v >= '0' && v < '8') {
                                v -= '0';
                                y <<= 3;
                                y &= (int)0xFFFFFFF8;
                                y |= v;
                                d++;
                            }
                            fs.unget();

                            if (d < 0) {
                                 ICE({{
                                     filepath,
                                     linecount,
                                     Diagnostic_type::I_ICE,
                                     "Unsigned Integer is less than zero",
                                     "Tokenize@Parse Character constant@Parse octal escape"
                                 }, boost::stacktrace::stacktrace()});
                            }

                            b += (char)y;
                        } else {
                            switch (v) {
                                case 'n':
                                    b += '\n';
                                    break;
                                case 't':
                                    b += '\t';
                                    break;
                                case 'v':
                                    b += '\v';
                                    break;
                                case 'r':
                                    b += '\r';
                                    break;
                                case 'f':
                                    b += '\f';
                                    break;
                                case 'a':
                                    b += '\a';
                                    break;
                                case '\\':
                                    b += '\\';
                                    break;
                                case '?':
                                    b += '?';
                                    break;
                                case '\'':
                                    b += '\'';
                                    break;
                                case '\"':
                                    b += '\"';
                                    break;
                                default:
                                    b += (char)v;
                                    diagnostics.add_diagnostic({
                                       filepath,
                                       linecount,
                                       Diagnostic_type::Bad_Escape,
                                       "Escape sequence of \'\\" + std::string(1, (char)v) + '\'',
                                       "Tokenize@Parse Character Constant@Parse Escape"
                                    });
                                    break;
                            }
                        }
                    }

                    count++;
                }

                tokens.emplace_back(Tok_String, false, b);
                continue;
            }

            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                std::string ident;

                ident += (char)c;

                std::cout << std::to_string(c) << std::endl;

                while ((c = fs.peek()) && (c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                    ident += (char)c;
                    fs.ignore();
                }

                if (keywords_set.contains(ident)) {
                    tokens.emplace_back(Tok_Keyword, false, ident);
                } else {
                    tokens.emplace_back(Tok_Ident, false, ident);
                }
            } else if (separator_map.contains(c)) {
                /*
                 * Catch separators
                 *
                 * { } = ;
                 * ( ) [ ]
                 * * , . /
                 * % + - <
                 * > & ^ |
                 * ? : ! ~
                 *
                 */
                tokens.emplace_back(separator_map[c]);
            } else {
                /*
                 * This rule is here as a "catch all" for the parser rules, it looses a bit of performance but the
                 * tradeoff is worth it in terms of generating good diagnostics
                 */
                //TODO: look for why -1 (EOF on my system) is falling through to here
                diagnostics.add_diagnostic(Diagnostic{
                    filepath,
                    linecount,
                    Diagnostic_type::Unrecognized_Character,
                    "Unrecognized character \'" + std::string(1, (char) c) + "\', charcode " +
                    std::to_string((char) c) + " fell through all parser rules",
                    "Tokenize@Catch Fallthrough"
                });
            }
        }

        tokens.emplace_back(Tok_EOF);

        return tokens;
    }

    std::unordered_map<Token_Type, std::string> token_to_string {
            {Tok_Preprocessor, "Preprocessor token"},
            {Tok_Whitespace, "Whitespace token"},
            {Tok_Newline, "Newline token"},
            {Tok_Newline_Fold, "Folded Newline token"},
            {Tok_Comment, "Comment token"},

            {Tok_Ident, "Identifier token"},
            {Tok_Keyword, "Keyword token"},
            {Tok_IntConstant, "Integer constant token"},
            {Tok_CharacterConstant, "Character constant token"},
            {Tok_FloatingConstant, "Float constant token"},
            {Tok_String, "String constant token"},
            {Tok_Paren_Open, "( Token"},
            {Tok_Paren_Close, ") Token"},
            {Tok_Square_Open, "[ Token"},
            {Tok_Square_Close, "] Token"},
            {Tok_Curly_Open, "{ Token"},
            {Tok_Curly_Close, "} Token"},
            {Tok_Angle_Open, "< Token"},
            {Tok_Angle_Close, "> Token"},
            {Tok_EQ, "= Token"},
            {Tok_Semicolon, "; Token"},
            {Tok_Star, "* Token"},
            {Tok_Comma, ", Token"},
            {Tok_Period, ". Token"},
            {Tok_Slash, "/ Token"},
            {Tok_Percent, "% Token"},
            {Tok_Plus, "+ Token"},
            {Tok_Minus, "- Token"},
            {Tok_Ampersand, "& Token"},
            {Tok_Hat, "^ Token"},
            {Tok_Pipe, "| Token"},
            {Tok_Question, "? Token"},
            {Tok_Colon, ": Token"},
            {Tok_Excl, "! Token"},
            {Tok_Tilde, "~ Token"},


            {Tok_EOF, "EOF token"}
    };

    void Tokenizer::print(const std::vector<Token>& toks) {
        for (const auto& tok: toks) {
            std::string name = token_to_string[tok.type];
            switch (tok.type) {
                case Tok_Preprocessor:
                case Tok_Comment:
                case Tok_Ident:
                case Tok_Keyword:
                    info(name + " \"" + std::get<std::string>(tok.str) + "\"");
                    break;
                case Tok_Newline:
                case Tok_Newline_Fold:
                    info(name + "@" + std::to_string(std::get<std::size_t>(tok.int_data)));
                    break;
                case Tok_IntConstant:
                    if (tok.UNSIGNED)
                        info(name + " " + std::to_string(std::get<std::uint64_t>(tok.int_data)));
                    else
                        info(name + " " + std::to_string(std::get<std::int64_t>(tok.int_data)));
                    break;
                case Tok_CharacterConstant:
                    if (tok.WIDE)
                        info((name + " ") + (char)std::get<wchar_t>(tok.char_data));
                    else
                        info((name + " ") + std::get<char>(tok.char_data));
                    break;
                case Tok_FloatingConstant:
                    info(name + " " + std::to_string(std::get<double>(tok.int_data)));
                    break;
                case Tok_String:
                    if (tok.WIDE)
                        try {
                            info(name + " " + ws2s(std::get<std::wstring>(tok.str)));
                        } catch(const std::range_error& err) {
                            warn("while attempting to print wstring, encountered bad character");
                            info(name);
                        }
                    else
                        info(name + " " + std::get<std::string>(tok.str));
                    break;
                case Tok_Paren_Open:
                case Tok_Paren_Close:
                case Tok_Square_Open:
                case Tok_Square_Close:
                case Tok_Curly_Open:
                case Tok_Curly_Close:
                case Tok_Angle_Open:
                case Tok_Angle_Close:
                case Tok_EQ:
                case Tok_Semicolon:
                case Tok_Star:
                case Tok_Comma:
                case Tok_Period:
                case Tok_Slash:
                case Tok_Percent:
                case Tok_Plus:
                case Tok_Minus:
                case Tok_Ampersand:
                case Tok_Hat:
                case Tok_Pipe:
                case Tok_Question:
                case Tok_Colon:
                case Tok_Excl:
                case Tok_Tilde:
                case Tok_EOF:
                case Tok_Whitespace:
                    info(name);
                    break;
            }
        }
    }
}
