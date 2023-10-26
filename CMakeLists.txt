cmake_minimum_required(VERSION 3.26)
project(TeaSDK VERSION 0.0.1)

###################################################
# Preamble
###################################################

set(TARGETS_PREFIX teasdk)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)

###################################################
# Packages required
###################################################

find_package(Doxygen
        REQUIRED dot)

###################################################
# Common libraries used by many utilities
###################################################

add_library(libargparse OBJECT
        src/argparse/Argparse.cpp
        src/argparse/Argparse.h

)

add_library(libelf OBJECT
    src/libelf/elf.cpp
    src/libelf/elf.h
    src/libelf/libelf_header.h
)

add_library(liblog OBJECT
        src/liblog/logger.cpp
        src/liblog/logger.h
)

add_library(hashbrown OBJECT
        src/hashbrown/hashbrown.h
        src/hashbrown/pointer_hash.cpp
        src/hashbrown/noncrypto_strings.cpp
)

add_library(libutils OBJECT
        src/libutils/target.cpp
        src/libutils/target.h
)

###################################################
# Executables
###################################################

add_executable(${TARGETS_PREFIX}-tcc89
    src/c89/main.cpp
)
target_link_libraries(${TARGETS_PREFIX}-tcc89
        libelf
        liblog
        libargparse
        hashbrown
        libutils
)

add_executable(${TARGETS_PREFIX}-readelf
    src/binutils/readelf/main.cpp
)
target_link_libraries(${TARGETS_PREFIX}-readelf libelf)

###################################################
#Doxygen
###################################################

doxygen_add_docs(docs src ALL WARN_IF_UNDOCUMENTED)
