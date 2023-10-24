//
// Created by teapot on 10/10/23.
//

#ifndef TEASDK_LOGGER_H
#define TEASDK_LOGGER_H

#include <mutex>
#include <iostream>

namespace liblog {
    class logger {
    public:
        void warn(std::string str);
    } global_logger_;

    std::mutex log_mutex_;
}

void warn(const std::string& str) {
    //TODO: actually call the correct logger function in the future
    std::lock_guard<std::mutex> log(liblog::log_mutex_);
    //liblog::global_logger_.warn(str);

    std::cout << "WARN: " <<  str << std::endl;
}

#endif //TEASDK_LOGGER_H
