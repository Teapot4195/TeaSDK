//
// Created by teapot on 10/10/23.
//

#ifndef TEASDK_LOGGER_H
#define TEASDK_LOGGER_H

#include <mutex>
#include <iostream>

/**
 * @brief main namespace for liblog
 *
 * All of the logging library resides here, except for the public facing log functions
 */
namespace liblog {
    /**
     * @brief level of depth for the logs
     */
    enum log_level {
        TRACE = 0,   ///< Trace level, LITERALLY EVERYTHING
        VERBOSE = 1, ///< Verbose level, usually less stuff
        DEBUG = 2,   ///< Debug info, even less stuff
        INFO = 3,    ///< Status messages and more important only
        WARN = 4,    ///< Warning messages and more important only
        FATAL = 5,   ///< pretty much nothing, only fatal errors will be logged
        NONE = 6     ///< None level, LITERALLY ABSOLUTELY NOTHING
    };

    /**
     * @brief base logger class
     */
    class logger {
    public:
        virtual ~logger();

        /**
         * @brief logs a trace level message
         *
         * @param str message to log
         */
        virtual void trace(const std::string& str) = 0;

        /**
         * @brief logs a verbose level message
         *
         * @param str message to log
         */
        virtual void verbose(const std::string& str) = 0;

        /**
         * @brief logs a debug level message
         *
         * @param str message to log
         */
        virtual void debug(const std::string& str) = 0;

        /**
         * @brief logs a info level message
         *
         * @param str message to log
         */
        virtual void info(const std::string& str) = 0;

        /**
         * @brief logs a warning level message
         *
         * @param str warning to log
         */
        virtual void warn(const std::string& str) = 0;

        /**
         * @brief logs a fatal level message
         *
         * @param str message to log
         */
        virtual void fatal(const std::string& str) = 0;
    };

    /**
     * @brief console logger implementation
     */
    class console_logger : public logger {
    public:
        ~console_logger() override;

        /**
         * @brief builds a logger instance with the class name of "GLOBAL"
         */
        console_logger();

        /**
         * @brief builds a logger instance with the specified class name
         *
         * @param name class name to log under
         */
        explicit console_logger(const std::string& name);

        /**
         * @brief builds a logger instance with the specified log level
         *
         * @param level level to log at
         */
        explicit console_logger(log_level level);

        /**
         * @brief builds a logger instance with the specified name and log level
         *
         * @param name name of the class to log under
         * @param level level to log at
         */
        console_logger(const std::string& name, log_level level);

        /**
         * @brief builds a logger instance with the specified name, log level and output stream
         *
         * @param name name of the class to log under
         * @param level level to log at
         */
        console_logger(const std::string& name, log_level level, std::ostream& stream);

        /**
         * @brief logs a trace level message
         *
         * @param str message to log
         */
        void trace(const std::string& str) override;

        /**
         * @brief logs a verbose level message
         *
         * @param str message to log
         */
        void verbose(const std::string& str) override;

        /**
         * @brief logs a debug level message
         *
         * @param str message to log
         */
        void debug(const std::string& str) override;

        /**
         * @brief logs a info level message
         *
         * @param str message to log
         */
        void info(const std::string& str) override;

        /**
         * @brief logs a warning level message
         *
         * @param str warning to log
         */
        void warn(const std::string& str) override;

        /**
         * @brief logs a fatal level message
         *
         * @param str message to log
         */
        void fatal(const std::string& str) override;

    private:
        const std::string& name;
        std::ostream& stream;
        log_level level;
    };

    class color_console_logger : public console_logger {

    };
}

/**
 * @brief logs a trace message to the console
 *
 * @param str the trace message to log
 */
void trace(const std::string& str);

/**
 * @brief logs a verbose message to the console
 *
 * @param str the verbose message to log
 */
void verbose(const std::string& str);

/**
 * @brief logs a debug message to the console
 *
 * @param str the debug message to log
 */
void debug(const std::string& str);

/**
 * @brief logs a info message to the console
 *
 * @param str the info message to log
 */
void info(const std::string& str);

/**
 * @brief logs a warning to the console
 *
 * @param str the warning to log
 */
void warn(const std::string& str);

/**
 * @brief logs a fatal message to the console
 *
 * @param str the fatal message to log
 */
void fatal(const std::string& str);

#endif //TEASDK_LOGGER_H
