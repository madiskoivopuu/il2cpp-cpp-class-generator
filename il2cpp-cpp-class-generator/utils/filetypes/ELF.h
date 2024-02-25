#pragma once
#include "IFile.h"

#define EI_NIDENT	16

// Source: https://github.com/torvalds/linux/blob/master/include/uapi/linux/elf.h
struct ELFHeader {
	unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	// NOTE: the next 3 are using uintptr_t since based on elf.h, that's the only difference between the elf32/64 headers
	// we only allow elf files that have the same num of bits as this executable so we can use uintptr_t
	uintptr_t e_entry;		/* Entry point virtual address */
	uintptr_t e_phoff;		/* Program header table file offset */
	uintptr_t e_shoff;		/* Section header table file offset */
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
};

#ifdef _WIN64
// Source: https://github.com/torvalds/linux/blob/master/include/uapi/linux/elf.h
struct ELFProgramHeader {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;		/* Segment file offset */
	uint64_t p_vaddr;		/* Segment virtual address */
	uint64_t p_paddr;		/* Segment physical address */
	uint64_t p_filesz;		/* Segment size in file */
	uint64_t p_memsz;		/* Segment size in memory */
	uint64_t p_align;		/* Segment alignment, file & memory */
};
#else
struct ELFProgramHeader {
	uint32_t	p_type;
	uint32_t	p_offset;
	uint32_t	p_vaddr;
	uint32_t	p_paddr;
	uint32_t	p_filesz;
	uint32_t	p_memsz;
	uint32_t	p_flags;
	uint32_t	p_align;
};
#endif

class ELF : public IFile
{
public:
	ELFHeader elfHeader;
	std::vector<ELFProgramHeader> sections;

	ELF(std::vector<BYTE>& bytes, FileInformation info);
	uintptr_t MapVAToOffset(uintptr_t virtualAddress);
};

