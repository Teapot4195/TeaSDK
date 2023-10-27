//
// Created by teapot on 26/10/23.
//

#ifndef TEASDK_TARGET_H
#define TEASDK_TARGET_H

namespace libutils::target {
    /**
     * @brief gets the target architecture that this library is currently being built for
     *
     * @return target for this library
     */
    consteval const char *getTargetArch();

#if defined(__x86_64__) || defined(_M_X64)
#define x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define x86_32
#elif defined(__ARM_ARCH_2__)
#define ARMv2
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARMv3
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARMv4T
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARMv5
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARMv6T2
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define ARM_V6
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARMv7
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARMv7A
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARMv7R
#elif defined(__ARM_ARCH_7M__)
#define ARMv7M
#elif defined(__ARM_ARCH_7S__)
#define ARMv7S
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARM64
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define MIPS
#elif defined(__sh__)
#define SUPERH
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define POWERPC
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define POWERPC64
#elif defined(__sparc__) || defined(__sparc)
#define SPARC
#elif defined(__m68k__)
#define M68K
#endif
}

#endif //TEASDK_TARGET_H
