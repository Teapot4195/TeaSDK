//
// Created by teapot on 10/10/23.
//

#ifndef TEASDK_ELF_H
#define TEASDK_ELF_H

#include <string>
#include <fstream>
#include <vector>

#include "libelf_header.h"

/**
 * @brief main namespace of libelf.
 *
 * Libelf resides in this namespace, all structures and the public library functions provided are here.
 */
namespace libelf {
    struct elf_prog_header_flag {
        bool PF_X; ///< If this segment is executable
        bool PF_W; ///< If this segment is writable
        bool PF_R; ///< If this segment is readable
    };

    /**
     * @brief represents the information used by the target os to create a process image.
     */
    struct elf_prog_header {
        unsigned long type;           ///< type of this segment.
        elf_prog_header_flag flags;   ///< flags for this segment (RWX)
        unsigned long long offset;    ///< offset of this data in the file, from start
        unsigned long long v_addr;    ///< virtual address of this segment
        unsigned long long p_addr;    ///< physical address of this segment (if relevant)
        unsigned long long file_size; ///< size of this segment in the file
        unsigned long long mem_size;  ///< size of this segment in memory
        unsigned long long align;     ///< alignemnt of this segment in memory
    };

    /**
     * @brief represents the information used by the target os to lpad the elf into memory
     */
    struct elf_section_header {
        unsigned long      name;           ///< sh_name in elf section header
        unsigned long      type;           ///< sh_type in elf section header
        unsigned long long flags;          ///< sh_flags in elf section header
        unsigned long long addr;           ///< sh_addr in elf section header
        unsigned long long offset;         ///< sh_offset in elf section header
        unsigned long long size;           ///< sh_size in elf section header
        unsigned long      link;           ///< sh_link in elf section header
        unsigned long      info;           ///< sh_info in elf section header
        unsigned long long addralign;      ///< sh_addr in elf section header
        unsigned long long entsize;        ///< sh_entsize in elf section header
        std::string        str_name;       ///< name extracted from .shstrtab
        std::vector<char>::iterator begin; ///< begin of the section in the file vec. (at first as usual)
        std::vector<char>::iterator end;   ///< end of the section in the file vec. (after end as usual)
    };

    /**
     * @brief stores the information contained in an elf file.
     */
    struct elf {
        // the entire file
        std::vector<char> file;                                ///< packed data of this elf file, stored in a vector

        // elf file header
        header::CLASS CLASS;                                   ///< bitiness targeted by this elf file
        header::DATA DATA;                                     ///< Endianness of this elf file
        unsigned char VERSION;                                 ///< Version of this elf file
        header::OSABI OSABI;                                   ///< OS ABI Targeted by this elf file
        unsigned char ABIVERSION;                              ///< ABI Version targeted by this elf file
        unsigned short TYPE;                                   ///< Type of this elf file
        unsigned short TargetISA;                              ///< ISA Targeted by this elf file.
        unsigned long long entrypoint;                         ///< entry point location (offset in file)
        unsigned long long program_header_table;               ///< location of the program header table (file offset)
        unsigned long long section_header_table;               ///< location of the section header table (file offset)
        unsigned long flags;                                   ///< flags
        unsigned short header_size;                            ///< size of this header
        unsigned short program_header_table_entry_size;        ///< size of the entries in the program header table
        unsigned short program_header_table_entry_num;         ///< number of entries in the program header table
        unsigned short section_header_table_entry_size;        ///< size of the entries section header table
        unsigned short section_header_table_entry_num;         ///< number of entries in the section header table
        unsigned short section_header_table_entry_names_index; ///< index in section header where .shstrtab resides

        // elf program header
        std::vector<elf_prog_header> program_header;           ///< unpacked entries from the program header table

        // elf section header
        std::vector<elf_section_header> section_header;        ///< unpacked entries from the section header table
    };

    /**
     * @brief reads the data contained in the elf file and processes it into an elf struct
     *
     * @see libelf::elf
     *
     * @param path path of the elf file to read
     *
     * @return the elf file representation
     */
    elf read(const std::string& path);

    /**
     * @brief reads the data contained in the elf file and processes it into an elf struct
     *
     * @see libelf::elf
     * @see libelf::read(const std::string& path)
     *
     * @param file ifstream of the elf file to read
     *
     * @return the elf file representation
     */
    elf read(std::ifstream& file);

    /**
     * @brief writes elf data to the output file at the specified location
     *
     * @see libelf::elf
     *
     * @param path path of the file to write to
     * @param data data to write to the file
     */
    void write(const std::string& path, elf& data);

    /**
     * @brief writes elf data to the output file at the specified location
     *
     * @see libelf::elf
     * @see libelf::write(const std::string& path, elf& data)
     *
     * @param file file in ofstream to write to
     * @param data data to write to the file
     */
    void write(std::ofstream& file, elf& data);

    /**
     * @brief pretty prints the data contained in the elf data
     *
     * @param data the data to pretty print.
     */
    void printelf(elf& data);
}

#endif //TEASDK_ELF_H
