cmake_minimum_required(VERSION 3.26)
project(TeaSDK)

set(TARGETS_PREFIX teasdk)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)

find_package(Doxygen
        REQUIRED dot)

add_library(libelf OBJECT
    src/libelf/elf.cpp
    src/libelf/elf.h
        src/libelf/libelf_header.h
)

add_library(liblog OBJECT
        src/liblog/logger.cpp
        src/liblog/logger.h
)

add_executable(${TARGETS_PREFIX}-tcc89
    src/c89/main.cpp
)
target_link_libraries(${TARGETS_PREFIX}-tcc89 libelf liblog)

add_executable(${TARGETS_PREFIX}-readelf
    src/binutils/readelf/main.cpp
)
target_link_libraries(${TARGETS_PREFIX}-readelf libelf)

doxygen_add_docs(docs src ALL WARN_IF_UNDOCUMENTED)
