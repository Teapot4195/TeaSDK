//
// Created by teapot on 10/10/23.
//

#include "logger.h"

const std::string default_logger_name = "GLOBAL";
std::ostream& default_logger_stream = std::cout;

namespace liblog {
    /// Global Logger instance
    console_logger global_logger_;

    /// Mutex for the global logger instance, here for multithreading
    std::mutex log_mutex_;

    logger::~logger() = default;

    console_logger::~console_logger() = default;

    console_logger::console_logger() :
        name(default_logger_name), level(log_level::INFO), stream(default_logger_stream) {}

    console_logger::console_logger(const std::string &name) :
        name(name), level(log_level::INFO), stream(default_logger_stream) {}

    console_logger::console_logger(enum log_level level) :
        name(default_logger_name), level(level), stream(default_logger_stream) {}

    console_logger::console_logger(const std::string &name, enum log_level level) :
        name(name), level(level), stream(default_logger_stream) {}

    console_logger::console_logger(const std::string &name, enum log_level level, std::ostream &stream) :
        name(name), level(level), stream(stream) {}

    void console_logger::trace(const std::string &str) {
        if (level > log_level::TRACE)
            return;

        stream << "TRACE: " << name << ": " << str << std::endl;
    }

    void console_logger::verbose(const std::string &str) {
        if (level > log_level::VERBOSE)
            return;

        stream << "VERBOSE: " << name << ": " << str << std::endl;
    }

    void console_logger::debug(const std::string &str) {
        if (level > log_level::DEBUG)
            return;

        stream << "DEBUG: " << name << ": " << str << std::endl;
    }

    void console_logger::info(const std::string &str) {
        if (level > log_level::INFO)
            return;

        stream << "INFO: " << name << ": " << str << std::endl;
    }

    void console_logger::warn(const std::string &str) {
        if (level > log_level::WARN)
            return;

        stream << "WARN: " << name << ": " << str << std::endl;
    }

    void console_logger::fatal(const std::string &str) {
        if (level > log_level::FATAL)
            return;

        stream << "FATAL: " << name << ": " << str << std::endl;
    }
}

void trace(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.trace(str);
}

void verbose(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.verbose(str);
}

void debug(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.debug(str);
}

void info(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.info(str);
}

void warn(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.warn(str);
}

void fatal(const std::string& str) {
std::lock_guard<std::mutex> log(liblog::log_mutex_);
liblog::global_logger_.fatal(str);
}
