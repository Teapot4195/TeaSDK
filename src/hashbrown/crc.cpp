//
// Created by teapot on 27/10/23.
//

#include "hashbrown.h"

#include <cstring>

//#include <libutils/target.h>

/*
 * Teapot's note:
 * general CRC implementation:
 * Referenced from Hacker's Delight 2nd Ed. Chapter 14, Page 319: Cyclic Redundancy Check.
 *
 * CRC-1 implementation:
 * Referenced from Hacker's Delight 2nd Ed. Chapter 5, Section 5-1, Page 81: Counting bits.
 */

namespace hashbrown::CRC {

    namespace CRC_1 {
        bool string(const std::string& str) {
            return arb_cstr(str.c_str(), str.size());
        }

        bool cstr(const char* str) {
            return arb_cstr(str, std::strlen(str));
        }

        bool arb_cstr(const char* str, std::size_t size) {
            bool ret = false;
            int x;

            for (std::size_t i = 0; i < size; ++i) {
                x = (unsigned char)str[i];

                x = x - ((x >> 1) & 0x55);
                x = (x & 0x33) + ((x >> 2) & 0x33);
                x = (x + (x >> 4)) & 0x0F;
                x += ret;

                ret = x & 0x1;
            }

            return ret;
        }

        bool ubyte(std::uint8_t x) {
            x = x - ((x >> 1) & 0x55);
            x = (x & 0x33) + ((x >> 2) & 0x33);
            x = (x + (x >> 4)) & 0x0F;

            return x & 0x1;
        }

        bool sbyte(std::int8_t data) {
            return ubyte(data);
        }

        bool uint16(std::uint16_t x) {
            x = x - ((x >> 1) & 0x5555);
            x = (x & 0x3333) + ((x >> 2) & 0x3333);
            x = (x + (x >> 4)) & 0x0F0F;
            x = x + (x >> 8);

            return x & 0x1;
        }

        bool int16(std::int16_t data) {
            return uint16(data);
        }

        bool uint32(std::uint32_t x) {
            x = x - ((x >> 1) & 0x5555);
            x = (x & 0x3333) + ((x >> 2) & 0x3333);
            x = (x + (x >> 4)) & 0x0F0F;
            x = x + (x >> 8);
            x = x + (x >> 16);

            return x & 0x1;
        }

        bool int32(std::int32_t data) {
            return uint32(data);
        }

        bool uint64(std::uint64_t x) {
            x = x - ((x >> 1) & 0x5555);
            x = (x & 0x3333) + ((x >> 2) & 0x3333);
            x = (x + (x >> 4)) & 0x0F0F;
            x = x + (x >> 8);
            x = x + (x >> 16);
            x = x + (x >> 32);

            return x & 0x1;
        }

        bool int64(std::int64_t data) {
            return uint64(data);
        }

        bool uint128(std::uint64_t a, std::uint64_t b) {
            return uint64(a) ^ uint64(b);
        }

        bool int128(std::int64_t a, std::uint64_t b) {
            return uint128(a, b);
        }

        bool arb_ubyte(std::uint8_t* data, std::size_t count, std::size_t offset);
        bool arb_byte(std::int8_t* data, std::size_t count, std::size_t offset);
        bool vec_ubyte(std::vector<std::uint8_t> data);
        bool vec_byte(std::vector<std::int8_t> data);
        bool arb_ushort(std::uint16_t* data, std::size_t count, std::size_t offset);
        bool arb_short(std::int16_t* data, std::size_t count, std::size_t offset);
        bool vec_ushort(std::vector<std::uint16_t> data);
        bool vec_short(std::vector<std::int16_t> data);
        bool arb_uint(std::uint32_t* data, std::size_t count, std::size_t offset);
        bool arb_int(std::int32_t* data, std::size_t count, std::size_t offset);
        bool vec_uint(std::vector<std::uint32_t> data);
        bool vec_int(std::vector<std::int32_t> data);
        bool arb_ulong(std::uint64_t* data, std::size_t count, std::size_t offset);
        bool arb_long(std::int64_t* data, std::size_t count, std::size_t offset);
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
        std::uint8_t arb_ubyte(std::uint8_t* data, std::size_t count, std::size_t offset);
        std::uint8_t arb_byte(std::int8_t* data, std::size_t count, std::size_t offset);
        std::uint8_t vec_ubyte(std::vector<std::uint8_t> data);
        std::uint8_t vec_byte(std::vector<std::int8_t> data);
        std::uint8_t arb_ushort(std::uint16_t* data, std::size_t count, std::size_t offset);
        std::uint8_t arb_short(std::int16_t* data, std::size_t count, std::size_t offset);
        std::uint8_t vec_ushort(std::vector<std::uint16_t> data);
        std::uint8_t vec_short(std::vector<std::int16_t> data);
        std::uint8_t arb_uint(std::uint32_t* data, std::size_t count, std::size_t offset);
        std::uint8_t arb_int(std::int32_t* data, std::size_t count, std::size_t offset);
        std::uint8_t vec_uint(std::vector<std::uint32_t> data);
        std::uint8_t vec_int(std::vector<std::int32_t> data);
        std::uint8_t arb_ulong(std::uint64_t* data, std::size_t count, std::size_t offset);
        std::uint8_t arb_long(std::int64_t* data, std::size_t count, std::size_t offset);
        std::uint8_t vec_ulong(std::vector<std::uint64_t> data);
        std::uint8_t vec_long(std::vector<std::int64_t> data);
    }
}
