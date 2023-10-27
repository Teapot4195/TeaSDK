//
// Created by teapot on 26/10/23.
//

#ifndef TEASDK_HASHBROWN_H
#define TEASDK_HASHBROWN_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief hashbrown library namespace
 *
 * Contains all of the public functions and classes for the hashbrown library,
 * CRC support functions are in the CRC namespace.
 * non_cryptographic hash functions are split by their input type.
 *
 * The following input parameter types are accepted:
 * 1. (string) std::string
 * 2. (cstr) const char*
 * 3. (arb_cstr) const char* with specific size
 * 4. (byte) byte
 * 5. (int16) 16-bit integer
 * 6. (int32) 32-bit integer
 * 7. (int64) 64-bit integer
 * 8. (int128) 128-bit integer split into two 64-bit integers
 * 9. (arb_byte) arbitrary size byte array
 * 10. (vec_byte) arbitrary size byte vector with specific size
 * 11. (arb_short) arbitrary size 16-bit array
 * 12. (vec_short) arbitrary size 16-bit array with specific size
 * 13. (arb_int) arbitrary size 32-bit array
 * 14. (vec_int) arbitrary size 32-bit array with specific size
 * 15. (arb_long) arbitrary size 64-bit array
 * 16. (vec_long) arbitrary size 64-bit array with specific size
 */
namespace hashbrown {
    /**
     * @brief CRC support library.
     *
     * Not all functions may be available on all platforms, may perform better or worse on some platforms.
     */
    namespace CRC {
        /**
         * @brief by default the implementations provided in this namespace may be optimized versions by intrinsic
         */
        namespace CRC_1 {
            /**
             * @brief provides native versions of the method, these are pure C++ only
             *
             * It is up to the compiler to optimize this with SIMD and the like.
             */
            namespace native {

            }

            /**
             * @brief computes the crc1 remainder (parity bit) for the specified string (does count parity bit)
             *
             * @param str string to compute parity bit for
             *
             * @return remainder as bool
             */
            bool string(const std::string& str);
            bool cstr(const char* str);
            bool arb_cstr(const char* str, std::size_t size);
            bool ubyte(std::uint8_t data);
            bool sbyte(std::int8_t data);
            bool uint16(std::uint16_t data);
            bool int16(std::int16_t data);
            bool uint32(std::uint32_t data);
            bool int32(std::int32_t data);
            bool uint64(std::uint64_t data);
            bool int64(std::int64_t data);
            bool uint128(std::uint64_t a, std::uint64_t b);
            bool int128(std::int64_t a, std::uint64_t b);
            bool arb_ubyte(std::uint8_t* data, std::size_t count, std::size_t offset = 0);
            bool arb_byte(std::int8_t* data, std::size_t count, std::size_t offset = 0);
            bool vec_ubyte(std::vector<std::uint8_t> data);
            bool vec_byte(std::vector<std::int8_t> data);
            bool arb_ushort(std::uint16_t* data, std::size_t count, std::size_t offset = 0);
            bool arb_short(std::int16_t* data, std::size_t count, std::size_t offset = 0);
            bool vec_ushort(std::vector<std::uint16_t> data);
            bool vec_short(std::vector<std::int16_t> data);
            bool arb_uint(std::uint32_t* data, std::size_t count, std::size_t offset = 0);
            bool arb_int(std::int32_t* data, std::size_t count, std::size_t offset = 0);
            bool vec_uint(std::vector<std::uint32_t> data);
            bool vec_int(std::vector<std::int32_t> data);
            bool arb_ulong(std::uint64_t* data, std::size_t count, std::size_t offset = 0);
            bool arb_long(std::int64_t* data, std::size_t count, std::size_t offset = 0);
            bool vec_ulong(std::vector<std::uint64_t> data);
            bool vec_long(std::vector<std::int64_t> data);
        }

        namespace CRC_3_GSM {
            std::uint8_t string(const std::string& str);
            std::uint8_t cstr(const char* str);
            std::uint8_t arb_cstr(const char* str, std::size_t size);
            std::uint8_t ubyte(std::uint8_t data);
            std::uint8_t sbyte(std::int8_t data);
            std::uint8_t uint16(std::uint16_t data);
            std::uint8_t int16(std::int16_t data);
            std::uint8_t uint32(std::uint32_t data);
            std::uint8_t int32(std::int32_t data);
            std::uint8_t uint64(std::uint64_t data);
            std::uint8_t int64(std::int64_t data);
            std::uint8_t uint128(std::uint64_t a, std::uint64_t b);
            std::uint8_t int128(std::int64_t a, std::uint64_t b);
            std::uint8_t arb_ubyte(std::uint8_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t arb_byte(std::int8_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t vec_ubyte(std::vector<std::uint8_t> data);
            std::uint8_t vec_byte(std::vector<std::int8_t> data);
            std::uint8_t arb_ushort(std::uint16_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t arb_short(std::int16_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t vec_ushort(std::vector<std::uint16_t> data);
            std::uint8_t vec_short(std::vector<std::int16_t> data);
            std::uint8_t arb_uint(std::uint32_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t arb_int(std::int32_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t vec_uint(std::vector<std::uint32_t> data);
            std::uint8_t vec_int(std::vector<std::int32_t> data);
            std::uint8_t arb_ulong(std::uint64_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t arb_long(std::int64_t* data, std::size_t count, std::size_t offset = 0);
            std::uint8_t vec_ulong(std::vector<std::uint64_t> data);
            std::uint8_t vec_long(std::vector<std::int64_t> data);
        }
    }

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
