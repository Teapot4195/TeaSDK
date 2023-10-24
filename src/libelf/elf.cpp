//
// Created by teapot on 10/10/23.
//

#include "elf.h"
#include "libelf_header.h"

#include <iterator>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>

namespace libelf {
    namespace header {
        std::unordered_map<OSABI, std::string> OSABI_to_String {
                {OSABI::System_V,             "UNIX - System V"},
                {OSABI::HP_UX,                "UNIX - Hewlett-Packard"},
                {OSABI::NETBSD,               "UNIX-Like - BSD - NetBSD"},
                {OSABI::LINUX,                "UNIX-Like - Linux"},
                {OSABI::GNU_HURD,             "UNIX-Like - GNU HURD"},
                {OSABI::SOLARIS,              "UNIX - Solaris"},
                {OSABI::AIX,                  "UNIX - AIX"},
                {OSABI::IRIX,                 "UNIX - IRIX"},
                {OSABI::FREEBSD,              "UNIX-Like - BSD - FreeBSD"},
                {OSABI::TRU64,                "UNIX - Tru64"},
                {OSABI::NovellModesto,        "NovellModesto"},
                {OSABI::OpenBSD,              "UNIX-Like - BSD - OpenBSD"},
                {OSABI::OpenVMS,              "OpenVMS"},
                {OSABI::Nonstop_Kernel,       "Nonstop_Kernel"},
                {OSABI::AROS,                 "AmigaOS-Like - AROS"},
                {OSABI::FENIXOS,              "UNIX-Like - Linux - FenixOS"},
                {OSABI::NUXI_CLOUDABI,        "Nuxi CloudABI"},
                {OSABI::STRATUS_TECH_OPENVOS, "Multics-Like - OpenVOS"}
        };

        std::unordered_map<TYPE, std::string> TYPE_to_String {
                {TYPE::ET_NONE, "ET_NONE - None"},
                {TYPE::ET_REL,  "ET_REL - Relocatable File"},
                {TYPE::ET_EXEC, "ET_EXEC - Executable File"},
                {TYPE::ET_DYN,  "ET_DYN - Shared Object File"},
                {TYPE::ET_CORE, "ET_CORE - Core File"}
        };

        std::unordered_map<MACHINE, std::string> MACHINE_to_String {
                {MACHINE::None, "No specific instruction set"},
                {MACHINE::ATNT_WE_32100, "AT&T WE 32100"},
                {MACHINE::SPARC, "SPARC"},
                {MACHINE::x86, "x86"},
                {MACHINE::Motorola_68000, "Motorola 68000 (M68k)"},
                {MACHINE::Motorola_88000, "Motorola 88000 (M88k)"},
                {MACHINE::Intel_MCU, "Intel MCU"},
                {MACHINE::Intel_80860, "Intel 80860"},
                {MACHINE::MIPS, "MIPS"},
                {MACHINE::IBM_SYS_370, "IBM System/370"},
                {MACHINE::MIPS_RS3000, "MIPS RS3000 Little-endian"},
                {MACHINE::HP_PA_RISC, "Hewlett-Packard PA-RISC"},
                {MACHINE::Intel_80960, "Intel 80960"},
                {MACHINE::PowerPC, "PowerPC"},
                {MACHINE::PowerPC_64, "PowerPC 64-bit"},
                {MACHINE::S390, "S390/S390x"},
                {MACHINE::IBM_SPU, "IBM SPU/SPC"},
                {MACHINE::NEC_V800, "NEC V800"},
                {MACHINE::Fujitsu_FR20, "Fujitsu FR20"},
                {MACHINE::TRW_RH_32, "TRW RH-32"},
                {MACHINE::Motorola_RCE, "Motorola RCE"},
                {MACHINE::Arm_Armv7_AArch32, "Arm (up to Armv7/AArch32)"},
                {MACHINE::Digital_alpha, "Digital Alpha"},
                {MACHINE::SuperH, "Super H"},
                {MACHINE::SPARC_V9, "SPARC V9"},
                {MACHINE::Siemens_Tricore_Embedded, "Siemens Tricore embedded processor"},
                {MACHINE::Argonaut_RISC_Core, "Argonaut RISC Core"},
                {MACHINE::Hitachi_H8_300, "Hitachi H8/300"},
                {MACHINE::Hitachi_H8_300H, "Hitachi H8/300H"},
                {MACHINE::Hitachi_H8S, "Hitachi H8S"},
                {MACHINE::Hitachi_H8_500, "Hitachi H8/500"},
                {MACHINE::IA_64, "IA-64"},
                {MACHINE::Stanford_MIPS_X, "Stanford MIPS-X"},
                {MACHINE::Motorola_Coldfire, "Motorola Coldfire"},
                {MACHINE::Motorola_M68HC12, "Motorola M68HC12"},
                {MACHINE::Fujitsu_MMA_Multimedia_Accelerator, "Fujitsu MMA Multimedia Accelerator"},


                {MACHINE::AMD_x86_64, "Advanced Micro Devices x86-64"},
        };
    }

    namespace seg {
        std::unordered_map<program::type, std::string> SEG_TYPE_to_String {
                {program::type::PT_NULL,    "PT_NULL  "},
                {program::type::PT_LOAD,    "PT_LOAD  "},
                {program::type::PT_DYNAMIC, "PT_DYN   "},
                {program::type::PT_INTERP,  "PT_INTERP"},
                {program::type::PT_NOTE,    "PT_NOTE  "},
                {program::type::PT_SHLIB,   "PT_SHLIB "},
                {program::type::PT_PHDR,    "PT_PHDR  "},
                {program::type::PT_TLS,     "PT_TLS   "},
        };
    }

    namespace sec {
        std::unordered_map<section::type, std::string> SEC_TYPE_to_String {
                {section::type::SHT_NULL,          "SHT_NULL          "},
                {section::type::SHT_PROGBITS,      "SHT_PROGBITS      "},
                {section::type::SHT_SYMTAB,        "SHT_SYMTAB        "},
                {section::type::SHT_STRTAB,        "SHT_STRTAB        "},
                {section::type::SHT_RELA,          "SHT_RELA          "},
                {section::type::SHT_HASH,          "SHT_HASH          "},
                {section::type::SHT_DYNAMIC,       "SHT_DYNAMIC       "},
                {section::type::SHT_NOTE,          "SHT_NOTE          "},
                {section::type::SHT_NOBITS,        "SHT_NOBITS        "},
                {section::type::SHT_REL,           "SHT_REL           "},
                {section::type::SHT_SHLIB,         "SHT_SHLIB         "},
                {section::type::SHT_DYNSYM,        "SHT_DYNSYM        "},
                {section::type::SHT_INIT_ARRAY,    "SHT_INIT_ARRAY    "},
                {section::type::SHT_FINI_ARRAY,    "SHT_FINI_ARRAY    "},
                {section::type::SHT_PREINIT_ARRAY, "SHT_PREINIT_ARRAY "},
                {section::type::SHT_GROUP,         "SHT_GROUP         "},
                {section::type::SHT_SYMTAB_SHNDX,  "SHT_SYMTAB_SHNDX  "},
                {section::type::SHT_NUM,           "SHT_NUM           "},
        };
    }

    template< typename T >
    std::string int_to_hex( T i )
    {
        std::stringstream stream;
        stream << "0x"
               << std::setfill ('0') << std::setw(sizeof(T)*2)
               << std::hex << i;
        return stream.str();
    }

    elf read(const std::string& path) {
        auto file = std::ifstream(path);
        return read(file);
    }

    elf read(std::ifstream& file) {
        /* Definitions for bounds checked read macro */
#define read4(err) if(4 != file.readsome(byte4buf, 4)) {std::cout << "WARN: EOF when reading " << err << std::endl; std::exit(1);}
#define read8(err) if(8 != file.readsome(byte8buf, 8)) {std::cout << "WARN: EOF when reading " << err << std::endl; std::exit(1);}
#define get4 (result.DATA == header::DATA::little) ? \
        (((unsigned long long)byte4buf[0] & 0xFF) | (((unsigned long long)byte4buf[1] << 8) & 0xFF00) | (((unsigned long long)byte4buf[2] << 16) & 0xFF0000) | (((unsigned long long)byte4buf[3] << 24)) & 0xFF000000) : \
        (((unsigned long long)byte4buf[3] & 0xFF) | (((unsigned long long)byte4buf[2] << 8) & 0xFF00) | (((unsigned long long)byte4buf[1] << 16) & 0xFF0000) | (((unsigned long long)byte4buf[0] << 24)) & 0xFF000000);
#define get8 (result.DATA == header::DATA::little) ?                                          \
        (((unsigned long long)byte8buf[0] & 0xFF) | (((unsigned long long)byte8buf[1] << 8) & (0xFF << 8)) |           \
         (((unsigned long long)byte8buf[2] << 16) & (0xFF << 16)) | (((unsigned long long)byte8buf[3] << 24) & (0xFF << 24)) |  \
         (((unsigned long long)byte8buf[4] << 32) & (0xFFull << 32)) | (((unsigned long long)byte8buf[5] << 40) & (0xFFull << 40)) |  \
         (((unsigned long long)byte8buf[6] << 48) & (0xFFull << 48)) | (((unsigned long long)byte8buf[7] << 56) & (0xFFull << 56))) : \
        (((unsigned long long)byte8buf[7] & 0xFF) | (((unsigned long long)byte8buf[6] << 8) & (0xFF << 8)) |           \
         (((unsigned long long)byte8buf[5] << 16) & (0xFF << 16)) | (((unsigned long long)byte8buf[4] << 24) & (0xFF << 24)) |  \
         (((unsigned long long)byte8buf[3] << 32) & (0xFFull << 32)) | (((unsigned long long)byte8buf[2] << 40) & (0xFFull << 40)) |  \
         (((unsigned long long)byte8buf[1] << 48) & (0xFFull << 48)) | (((unsigned long long)byte8buf[0] << 56) & (0xFFull << 56)));

        /* Buffers */
        char byte1buf[1];
        char byte2buf[2];
        char byte4buf[4];
        char byte8buf[8];
        char loadbuf[128];
        elf result {};

        /* Elf header magic */
        read4("elf header magic")
        if (std::memcmp(byte4buf, libelf::header::MAGIC, 4) != 0) {
            std::cout << "WARN: MAGIC of elf is not correct, got: " << std::endl;
            std::exit(1);
        }

        /* Elf Bit type */
        if (1 != file.readsome(byte1buf, 1)) {
            std::cout << "WARN: EOF when reading elf class" << std::endl;
            std::exit(1);
        } else if (!(byte1buf[0] == 1 || byte1buf[0] == 2)) {
            std::cout << "WARN: Elf bit type is not of a supported value, expected {1|2} got " << std::to_string(byte1buf[0]) << std::endl;
            std::exit(1);
        }

        result.CLASS = header::CLASS(byte1buf[0]);

        /* Elf Data type */
        if (1 != file.readsome(byte1buf, 1)) {
            std::cout << "WARN: EOF when reading elf data type" << std::endl;
            std::exit(1);
        } else if (!(byte1buf[0] == 1 || byte1buf[0] == 2)) {
            std::cout << "WARN: Elf endianness is not of a known value, expected {1|2} got " << std::to_string(byte1buf[0]) << std::endl;
            std::exit(1);
        }

        result.DATA = header::DATA(byte1buf[0]);

        /* Elf Version */
        if (1 != file.readsome(byte1buf, 1)) {
            std::cout << "WARN: EOF when reading elf version" << std::endl;
            std::exit(1);
        }

        result.VERSION = byte1buf[0];

        /* Elf OSABI */
        if (1 != file.readsome(byte1buf, 1)) {
            std::cout << "WARN: EOF when reading elf OSABI" << std::endl;
            std::exit(1);
        } else if (!header::OSABI_to_String.contains(header::OSABI(byte1buf[0]))) {
            std::cout << "WARN: Unknown OSABI Type: " << std::to_string(byte1buf[0]) << std::endl;
            std::exit(1);
        }

        result.OSABI = header::OSABI(byte1buf[0]);

        /* Elf ABI Ver */
        if (1 != file.readsome(byte1buf, 1)) {
            std::cout << "WARN: EOF when reading elf ABIVersion" << std::endl;
            std::exit(1);
        }

        result.ABIVERSION = byte1buf[0];

        /* Skip past 7 bytes padding */
        file.seekg(0x10, std::ios::beg);
        if (file.eof()) {
            std::cout << "WARN: EOF when seeking past 7 byte pad" << std::endl;
            std::exit(1);
        }

        /* Elf file Type */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading elf file type" << std::endl;
            std::exit(1);
        }
        result.TYPE = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                             (byte2buf[1] | (byte2buf[0] << 8)));

        /* Elf target ISA */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading elf target ISA" << std::endl;
            std::exit(1);
        }
        result.TargetISA = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                            (byte2buf[1] | (byte2buf[0] << 8)));

        /* Elf version 2x */
        read4("elf version 2x")

        if (result.CLASS == header::CLASS::bit32) {
            /* 32bit entry point addr */
            read4("32bit entry point")
            result.entrypoint = get4

            /* 32bit program header table location */
            read4("32bit program header table location")
            result.program_header_table = get4

            /* 32bit section header table location */
            read4("32bit section header table location")
            result.section_header_table = get4
        } else {
            /* 64bit entry point addr */
            read8("64bit entry point")
            result.entrypoint = get8

            /* 64bit program header table location */
            read8("64bit program header table location")
            result.program_header_table = get8

            /* 64bit section header table location */
            read8("64bit section header table location")
            result.section_header_table = get8
        }

        /* elf flags */
        read4("elf flags")
        result.flags = get4

        /* elf elhsize header size */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading elf header size" << std::endl;
            std::exit(1);
        }
        result.header_size = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                    (byte2buf[1] | (byte2buf[0] << 8)));

        /* elf phentsize program header table entry size */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading program header table entry size" << std::endl;
            std::exit(1);
        }
        result.program_header_table_entry_size = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                                        (byte2buf[1] | (byte2buf[0] << 8)));

        /* elf phnum program header table entry count */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading program header table entry count" << std::endl;
            std::exit(1);
        }
        result.program_header_table_entry_num = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                                       (byte2buf[1] | (byte2buf[0] << 8)));

        /* elf shentsize section header table entry size */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading section header table entry size" << std::endl;
            std::exit(1);
        }
        result.section_header_table_entry_size = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                                        (byte2buf[1] | (byte2buf[0] << 8)));

        /* elf shnum section header table entry count */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading section header table entry count" << std::endl;
            std::exit(1);
        }
        result.section_header_table_entry_num = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                                       (byte2buf[1] | (byte2buf[0] << 8)));

        /* elf shstrndx section header table entry names */
        if (2 != file.readsome(byte2buf, 2)) {
            std::cout << "WARN: EOF when reading section header table entry names index" << std::endl;
            std::exit(1);
        }
        result.section_header_table_entry_names_index = (result.DATA == header::DATA::little ? (byte2buf[0] | (byte2buf[1] << 8)) :
                                                                                         (byte2buf[1] | (byte2buf[0] << 8)));

        assert(file.tellg() == (result.CLASS == header::CLASS::bit32 ? 0x34 : 0x40));

        /* Read out the entire elf file */
        file.seekg(std::ios::beg);
        while (0 != file.readsome(loadbuf, 128)) {
            std::copy(loadbuf, loadbuf + file.gcount(), std::back_inserter(result.file));
        }

        /* Read the program header table */
        file.seekg((long long)result.program_header_table, std::ios::beg);
        for (std::size_t i = 0; i < result.program_header_table_entry_num; i++) {
            elf_prog_header entry {};
            if (result.CLASS == header::CLASS::bit32) {
                /* segment type */
                read4("segment type")
                entry.type = get4

                /* segment offset */
                read4("segment offset")
                entry.offset = get4

                /* segment virtual address */
                read4("segment virtual address")
                entry.v_addr = get4

                /* segment physical address (0 where not-applicable) */
                read4("segment physical address")
                entry.p_addr = get4

                /* segment size in file image (can be 0) */
                read4("segment size in file image")
                entry.file_size = get4

                /* segment size in memory (can be 0) */
                read4("segment size in memory")
                entry.mem_size = get4

                /* flags */
                read4("segment flags")

                if (result.DATA == header::DATA::little) {
                    entry.flags = elf_prog_header_flag {
                            (byte4buf[0] & 0x1) != 0,
                            (byte4buf[0] & 0x2) != 0,
                            (byte4buf[0] & 0x4) != 0
                    };
                } else {
                    entry.flags = elf_prog_header_flag {
                            (byte4buf[3] & 0x1) != 0,
                            (byte4buf[3] & 0x2) != 0,
                            (byte4buf[3] & 0x4) != 0
                    };
                }

                /* alignment (0,1 for no alignment)  */
                read4("alignment data")
                entry.align = get4
            } else {
                /* segment type */
                read4("segment type")
                entry.type = get4

                /* flags */
                read4("segment flags")

                if (result.DATA == header::DATA::little) {
                    entry.flags = elf_prog_header_flag {
                            (byte4buf[0] & 0x1) != 0,
                            (byte4buf[0] & 0x2) != 0,
                            (byte4buf[0] & 0x4) != 0
                    };
                } else {
                    entry.flags = elf_prog_header_flag {
                            (byte4buf[3] & 0x1) != 0,
                            (byte4buf[3] & 0x2) != 0,
                            (byte4buf[3] & 0x4) != 0
                    };
                }

                /* segment offset */
                read8("segment offset")
                entry.offset = get8

                /* segment virtual address */
                read8("segment virtual address")
                entry.v_addr = get8

                /* segment physical address */
                read8("segment physical address")
                entry.p_addr = get8

                /* segment size in file image */
                read8("segment size in file image")
                entry.file_size = get8

                /* segment size in memory */
                read8("segment size in memory")
                entry.mem_size = get8

                /* segment alignment */
                read8("segment alignment")
                entry.align = get8
            }

            result.program_header.push_back(entry);
        }

        /* Read the section header table */
        file.seekg((long long)result.section_header_table, std::ios::beg);
        for (std::size_t i = 0; i < result.section_header_table_entry_num; ++i) {
            elf_section_header entry {};

            /* .shstrtab offset for section name */
            read4("sh_name")
            entry.name = get4

            /* type of this section */
            read4("sh_type")
            entry.type = get4

            if (result.CLASS == libelf::header::CLASS::bit32) {
                /* flags */
                read4("sh_flags")
                entry.flags = get4

                /* addr */
                read4("sh_addr")
                entry.addr = get4

                /* offset */
                read4("sh_offset")
                entry.offset = get4

                /* size */
                read4("sh_size")
                entry.size = get4
            } else {
                /* flags */
                read8("sh_flags")
                entry.flags = get8

                /* addr */
                read8("sh_addr")
                entry.addr = get8

                /* offset */
                read8("sh_offset")
                entry.offset = get8

                /* size */
                read8("sh_size")
                entry.size = get8
            }

            /* section index of associated index */
            read4("sh_link")
            entry.link = get4

            /* Extra info about the section */
            read4("sh_info")
            entry.info = get4

            if (result.CLASS == libelf::header::CLASS::bit32) {
                /* alignment of section */
                read4("sh_addralign")
                entry.addralign = get4

                /* entry size */
                read4("s_entsize")
                entry.entsize = get4
            } else {
                /* alignment of section */
                read8("sh_addralign")
                entry.addralign = get8

                /* entry size */
                read8("sh_entsize")
                entry.entsize = get8
            }

            entry.begin = result.file.begin() + (long)entry.offset;
            entry.end = entry.begin + (long)entry.size + 1;

            result.section_header.push_back(entry);
        }

        auto section_table_offset = (long long)result.section_header[result.section_header_table_entry_names_index].offset;
        for (std::size_t i = 0; i < result.section_header_table_entry_num; i++) {
            elf_section_header& entry = result.section_header[i];

            long long head = section_table_offset + (long long)entry.name;

            if (head > result.file.size())
                continue;

            /* Glob until null is reached */
            std::string name;
            char c;
            while ((c = result.file[head]) != '\0') {
                name += c;
                ++head;
            }

            entry.str_name = name;
        }

        return result;
#undef read4
#undef read8
#undef get4
#undef get8
    }

    void write(const std::string& path, elf& data) {
        std::ofstream file = std::ofstream(path);
        write(file, data);
    }

    void write(std::ofstream& file, elf& data) {

    }

    void printelf(elf& data) {
        std::cout << "ELF Header:" << std::endl;
        std::cout << "    Class:                                  " <<
            (data.CLASS == header::CLASS::bit32 ?
                "ELF32" :
                "ELF64"
        ) << std::endl;
        std::cout << "    Data:                                   " <<
            (data.DATA == header::DATA::big ? "2's complement, big endian" : "2's complement, little endian"
        ) << std::endl;
        std::cout << "    Version:                                " << std::to_string(data.VERSION) << std::endl;
        std::cout << "    Target OS:                              " << header::OSABI_to_String[data.OSABI] << std::endl;
        std::cout << "    Target OS ABI version:                  " << std::to_string(data.ABIVERSION) << std::endl;
        std::cout << "    Object File Type:                       " <<
            (data.TYPE >= 0xFE00 ?
                (data.TYPE <= 0xFEFF ? "Operating Specific, Reserved" : "Processor Specific, Reserved") :
                header::TYPE_to_String[header::TYPE(data.TYPE)]) << std::endl;
        std::cout << "    Target ISA:                             " << header::MACHINE_to_String[header::MACHINE(data.TargetISA)] << std::endl;
        std::cout << "    Program Entrypoint:                     0x" << std::hex << data.entrypoint << std::dec << std::endl;
        std::cout << "    Program Header:                         " << data.program_header_table << std::endl;
        std::cout << "    Section Header:                         " << data.section_header_table << std::endl;
        std::cout << "    Flags:                                  0x" << std::hex << data.flags << std::dec << std::endl;
        std::cout << "    Elf header size:                        " << data.header_size << std::endl;
        std::cout << "    Program header table entry size:        " << data.program_header_table_entry_size << std::endl;
        std::cout << "    Program header table entry count:       " << data.program_header_table_entry_num << std::endl;
        std::cout << "    Section header table entry size:        " << data.section_header_table_entry_size << std::endl;
        std::cout << "    Section header table entry count:       " << data.section_header_table_entry_num << std::endl;
        std::cout << "    Section header table entry names index: " << data.section_header_table_entry_names_index << std::endl;
        std::cout << "Program Header:" << std::endl;
        std::cout << "    SEG TYPE  RWX             OFFSET             V_ADDR             P_ADDR          FILE_SIZE           MEM_SIZE              ALIGN" << std::endl;
#define set << std::setw(16) << std::setfill('0') << std::hex
        for (const elf_prog_header& entry: data.program_header) {
            std::cout << "    " <<
                ((entry.type >= 0x60000000 && entry.type <= 0x7FFFFFFF) ?
                 ((entry.type <= 0x6FFFFFFF) ? "OS-spec  " :
                                               "PROC-spec") :
                 ((seg::SEG_TYPE_to_String.contains(program::type(entry.type))) ? seg::SEG_TYPE_to_String[program::type(entry.type)] : "---------")) << " ";
            std::cout << (entry.flags.PF_R ? "R" : "-") << (entry.flags.PF_W ? "W" : "-") << (entry.flags.PF_X ? "X" : "-");
            std::cout << " 0x" set << entry.offset;
            std::cout << " 0x" set << entry.v_addr;
            std::cout << " 0x" set << entry.p_addr;
            std::cout << " 0x" set << entry.file_size;
            std::cout << " 0x" set << entry.mem_size;
            std::cout << " 0x" set << entry.align;
            std::cout << std::endl;
        }
        std::cout << "Section Header:" << std::endl;
        std::cout << "    Flags descritpion:" << std::endl;
        std::cout << "    W => Write" << std::endl;
        std::cout << "    A => Alloc" << std::endl;
        std::cout << "    X => Executable" << std::endl;
        std::cout << "    M => Mergable" << std::endl;
        std::cout << "    S => Strings" << std::endl;
        std::cout << "    I => sh_info contains SHT index" << std::endl;
        std::cout << "    L => Preserve order after combining" << std::endl;
        std::cout << "    N => OS Nonconforming" << std::endl;
        std::cout << "    G => Section is group member" << std::endl;
        std::cout << "    T => Thread local data" << std::endl;
        std::cout << "    O => OS-specific" << std::endl;
        std::cout << "    P => Processor-specific" << std::endl;
        std::cout << "    R => Special Ordering requirement (Solaris)" << std::endl;
        std::cout << "    E => Section is excluded unless referenced or allocated (Solaris)" << std::endl;
        std::cout << "            entry type WAXMSILNGTOPRE               ADDR             OFFSET               SIZE               LINK               INFO          ADDRALIGN            ENTSIZE name" << std::endl;
        for (const elf_section_header& entry: data.section_header) {
            std::cout << "    " <<
                                (sec::SEC_TYPE_to_String.contains(libelf::section::type(entry.type)) ?
                                    (sec::SEC_TYPE_to_String[libelf::section::type(entry.type)]) :
                                    (int_to_hex(entry.type)));
            std::cout << " " << ((entry.flags & 0x00000001) != 0 ? "W" : "-") <<
                                ((entry.flags & 0x00000002) != 0 ? "A" : "-") <<
                                ((entry.flags & 0x00000004) != 0 ? "X" : "-") <<
                                ((entry.flags & 0x00000010) != 0 ? "M" : "-") <<
                                ((entry.flags & 0x00000020) != 0 ? "S" : "-") <<
                                ((entry.flags & 0x00000040) != 0 ? "I" : "-") <<
                                ((entry.flags & 0x00000080) != 0 ? "L" : "-") <<
                                ((entry.flags & 0x00000100) != 0 ? "N" : "-") <<
                                ((entry.flags & 0x00000200) != 0 ? "G" : "-") <<
                                ((entry.flags & 0x00000400) != 0 ? "T" : "-") <<
                                ((entry.flags & 0x0FF00000) != 0 ? "O" : "-") <<
                                ((entry.flags & 0xF0000000) != 0 ? "P" : "-") <<
                                ((entry.flags & 0x04000000) != 0 ? "R" : "-") <<
                                ((entry.flags & 0x08000000) != 0 ? "E" : "-");
            std::cout << " 0x" set << entry.addr;
            std::cout << " 0x" set << entry.offset;
            std::cout << " 0x" set << entry.size;
            std::cout << " 0x" set << entry.link;
            std::cout << " 0x" set << entry.info;
            std::cout << " 0x" set << entry.addralign;
            std::cout << " 0x" set << entry.entsize;
            std::cout << " 0x" set << entry.name;
            std::cout << " " << entry.str_name;
            std::cout << std::endl;
        }
#undef set
    }
}
