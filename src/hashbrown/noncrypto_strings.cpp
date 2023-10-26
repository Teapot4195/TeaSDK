//
// Created by teapot on 26/10/23.
//

#include "hashbrown.h"

namespace hashbrown::non_crypto::strings {
    std::uint64_t fnv_1_64_arb_size(const char* string, std::size_t size) {
        std::uint64_t hash = 0xcbf29ce484222325ull;

        for (std::size_t i = 0; i < size; i++) {
            hash *= 0x100000001b3;
            hash ^= string[i];
        }

        return hash;
    }
}
