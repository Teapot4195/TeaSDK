//
// Created by teapot on 27/10/23.
//

#include <iostream>

#include <hashbrown/hashbrown.h>

#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include "bench.h"

#include <vector>
#include <cstdlib> // for rand

PICOBENCH_SUITE("hashbrown_crc_1");

static void byte_crc_1(picobench::state& s) {
    bool f = true;
    for (auto a : s) {
        if (hashbrown::CRC::CRC_1::ubyte(a) != hashbrown::CRC::CRC_1::sbyte((std::int8_t)a))
            f = false;
    }
    if (!f)
        std::cout << "WARN: parity bit for signed int not equal unsigned int somewhere" << std::endl;
}
PICOBENCH(byte_crc_1);

static void short_crc_1(picobench::state& s) {
    bool f = true;
    for (auto a : s) {
        if (hashbrown::CRC::CRC_1::uint16(a) != hashbrown::CRC::CRC_1::int16(a))
            f = false;
    }
    if (!f)
        std::cout << "WARN: parity bit for signed int not equal unsigned int somewhere" << std::endl;
}
PICOBENCH(short_crc_1);

static void int_crc_1(picobench::state& s) {
    bool f = true;
    for (auto a : s) {
        if (hashbrown::CRC::CRC_1::uint32(a) != hashbrown::CRC::CRC_1::int32(a))
            f = false;
    }
    if (!f)
        std::cout << "WARN: parity bit for signed int not equal unsigned int somewhere" << std::endl;
}
PICOBENCH(int_crc_1);

static void long_crc_1(picobench::state& s) {
    bool f = true;
    for (auto a : s) {
        if (hashbrown::CRC::CRC_1::uint64(a) != hashbrown::CRC::CRC_1::int64(a))
            f = false;
    }
    if (!f)
        std::cout << "WARN: parity bit for signed int not equal unsigned int somewhere" << std::endl;
}
PICOBENCH(long_crc_1);

//// Benchmarking function written by the user:
//static void rand_vector(picobench::state& s)
//{
//    std::vector<int> v;
//    for (auto _ : s)
//    {
//        v.push_back(rand());
//    }
//}
//PICOBENCH(rand_vector); // Register the above function with picobench
//
//// Another benchmarking function:
//static void rand_vector_reserve(picobench::state& s)
//{
//    std::vector<int> v;
//    v.reserve(s.iterations());
//    for (auto _ : s)
//    {
//        v.push_back(rand());
//    }
//}
//PICOBENCH(rand_vector_reserve);
