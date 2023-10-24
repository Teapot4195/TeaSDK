//
// Created by teapot on 10/10/23.
//

#ifndef TEASDK_LIBELF_HEADER_H
#define TEASDK_LIBELF_HEADER_H

#include <unordered_map>
#include <string>

/* TODO: Convert all these constants to enum one day */

namespace libelf::header {
    const char MAGIC[4] = {0x7f, 0x45, 0x4c, 0x46};

    /**
     * Defines the class of this executable, whether 32-bit or 64-bit.
     */
    enum class CLASS {
        bit32 = 0x1,
        bit64 = 0x2,
    };

    /**
     * Defines the byte ordering starting at offset 0x10
     */
    enum class DATA {
        little = 0x1,
        big = 0x2
    };

    /**
     * Defines the version of this ELF file.
     * There currently is no version other than 1.
     */
    #define ELFv_VERSION 0x1

    /**
     * Defines the target operating system ABI for this executable
     */
    enum class OSABI {
        System_V = 0x00,
        HP_UX = 0x01,
        NETBSD = 0x02,
        LINUX = 0x03,
        GNU_HURD = 0x04,
        SOLARIS = 0x06,
        AIX = 0x07,
        IRIX = 0x08,
        FREEBSD = 0x09,
        TRU64 = 0x0A,
        NovellModesto = 0x0B,
        OpenBSD = 0x0C,
        OpenVMS = 0x0D,
        Nonstop_Kernel = 0x0E,
        AROS = 0x0F,
        FENIXOS = 0x10,
        NUXI_CLOUDABI = 0x11,
        STRATUS_TECH_OPENVOS = 0x012,
    };

    /**
     * Defines the type of the object file, affected by endianness defined by ELF_HEAD_DATA_*.
     *
     * All definitions here are stored in ****BIG ENDIAN****
     */
    enum class TYPE {
        ET_NONE = 0x00,
        ET_REL = 0x01,
        ET_EXEC = 0x02,
        ET_DYN = 0x03,
        ET_CORE = 0x04,
        ET_LOOS = 0xFE00,
        ET_HIOS = 0xFEFF,
        ET_LOPROC = 0xFF00,
        ET_HIPROC = 0xFFFF,
    };

    /**
     * Defines the target instruction set architecture
     */
    enum class MACHINE {
        None = 0x00,
        ATNT_WE_32100 = 0x01,
        SPARC = 0x02,
        x86 = 0x03,
        Motorola_68000 = 0x04,
        Motorola_88000 = 0x05,
        Intel_MCU = 0x06,
        Intel_80860 = 0x07,
        MIPS = 0x08,
        IBM_SYS_370 = 0x09,
        MIPS_RS3000 = 0x0A,
        HP_PA_RISC = 0x0F,
        Intel_80960 = 0x13,
        PowerPC = 0x14,
        PowerPC_64 = 0x15,
        S390 = 0x16,
        IBM_SPU = 0x17,
        NEC_V800 = 0x24,
        Fujitsu_FR20 = 0x25,
        TRW_RH_32 = 0x26,
        Motorola_RCE = 0x27,
        Arm_Armv7_AArch32 = 0x28,
        Digital_alpha = 0x29,
        SuperH = 0x2A,
        SPARC_V9 = 0x2B,
        Siemens_Tricore_Embedded = 0x2C,
        Argonaut_RISC_Core = 0x2D,
        Hitachi_H8_300 = 0x2E,
        Hitachi_H8_300H = 0x2F,
        Hitachi_H8S = 0x30,
        Hitachi_H8_500 = 0x31,
        IA_64 = 0x32,
        Stanford_MIPS_X = 0x33,
        Motorola_Coldfire = 0x34,
        Motorola_M68HC12 = 0x35,
        Fujitsu_MMA_Multimedia_Accelerator = 0x36,
        Siemens_PCP = 0x37,
        Sony_nCPU_embedded_RISC_processor = 0x38,
        Denso_NDR1_microprocessor = 0x39,
        Motorola_StarCore_Processor = 0x3A,
        Toyota_ME16_Processor = 0x3B,
        STMicroelectronics_ST100_processor = 0x3C,
        Advanced_Logic_Group_TinyJ_Embedded_processor = 0x3D,
        AMD_x86_64 = 0x3E,
        Sony_DSP_processor = 0x3F,
        Digital_Equipment_Corp_PDP_10 = 0x40,
        Digital_Equipment_Corp_PDP_11 = 0x41,
        Siemens_FX66_Microcontroller = 0x42,
        STMicroelectronics_ST9_8_16_bit_microcontroller = 0x43,
        STMicroelectronics_ST7_8_bit_microcontroller = 0x44,
        Motorola_MC68HC16_microcontroller = 0x45,
        Motorola_MC68HC11_microcontroller = 0x46,
        Motorola_MC68HC08_microcontroller = 0x47,
        Motorola_MC68HC05_microcontroller = 0x48,
        Silicon_Graphics_SVx = 0x49,
        STMicroelectronics_ST19_8_bit_microcontroller = 0x4A,
        Digital_VAX = 0x4B,
        Axis_Communications_32bit_embedded_processor = 0x4C,
        Infineon_Technologies_32bit_embedded_processor = 0x4D,
        Element_14_64bit_DSP_processor = 0x4E,
        LSI_Logic_16bit_DSP_processor = 0x4F,
        TMS320C6000 = 0x8C,
        MCST_Elbrus_e32k = 0xAF,
        Arm_64bit_Armv8_AArch64 = 0xB7,
        Zilog_Z80 = 0xDC,
        RISC_V = 0xF3,
        Berkley_Packet_Filter = 0xF7,
        WDC_65C816 = 0x101,
    };

    /**
     * Version: always set to 1
     */
    #define ELF_HEAD_E_VERSION_ORIG 0x1
}

namespace libelf::program {
    /**
     * identifies the type of segment
     */
    enum class type {
        PT_NULL = 0x0,
        PT_LOAD = 0x1,
        PT_DYNAMIC = 0x2,
        PT_INTERP = 0x3,
        PT_NOTE = 0x4,
        PT_SHLIB = 0x5,
        PT_PHDR = 0x6,
        PT_TLS = 0x7,
    };
}

namespace libelf::section {
    /**
     * identifies the type of section
     */
     enum class type {
         SHT_NULL = 0x0,
         SHT_PROGBITS = 0x1,
         SHT_SYMTAB = 0x2,
         SHT_STRTAB = 0x3,
         SHT_RELA = 0x4,
         SHT_HASH = 0x5,
         SHT_DYNAMIC = 0x6,
         SHT_NOTE = 0x7,
         SHT_NOBITS = 0x8,
         SHT_REL = 0x9,
         SHT_SHLIB = 0xA,
         SHT_DYNSYM = 0xB,
         SHT_INIT_ARRAY = 0xE,
         SHT_FINI_ARRAY = 0xF,
         SHT_PREINIT_ARRAY = 0x10,
         SHT_GROUP = 0x11,
         SHT_SYMTAB_SHNDX = 0x12,
         SHT_NUM = 0x13,
     };
}

#endif //TEASDK_LIBELF_HEADER_H
