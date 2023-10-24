//
// Created by teapot on 11/10/23.
//

#include <iostream>

#include "libelf/elf.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " elf_executable" << std::endl;
        return 1;
    }

    libelf::elf file = libelf::read(argv[1]);
    libelf::printelf(file);

    return 0;
}
