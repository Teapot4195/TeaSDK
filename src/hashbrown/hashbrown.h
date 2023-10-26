//
// Created by teapot on 26/10/23.
//

#ifndef TEASDK_HASHBROWN_H
#define TEASDK_HASHBROWN_H

#include <cstdint>

namespace hashbrown {
    namespace non_crypto {
        namespace strings {
            template<std::size_t N>
            constexpr std::uint64_t fnv_1_64(const char string[N]) {
                std::uint64_t hash = 0xcbf29ce484222325ull;

                // N-1 because the string is null terminated
                for (std::size_t i = 0; i < N - 1; i++) {
                    hash *= 0x100000001b3;
                    hash ^= string[i];
                }

                return hash;
            }

            std::uint64_t fnv_1_64_arb_size(const char* string, std::size_t size);
        }
    }
}

#endif //TEASDK_HASHBROWN_H
