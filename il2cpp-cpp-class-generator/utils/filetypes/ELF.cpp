#include "ELF.h"

ELF::ELF(std::vector<BYTE>& bytes, FileInformation info) : sections({}) {
	this->fileBytes = bytes;
	this->info = info;

	this->elfHeader = *reinterpret_cast<ELFHeader*>(this->fileBytes.data());

	ELFProgramHeader* sectionList = reinterpret_cast<ELFProgramHeader*>(this->fileBytes.data() + this->elfHeader.e_shoff);
	for (int i = 0; i < this->elfHeader.e_shnum; i++) {
		ELFProgramHeader section = sectionList[i];
		this->sections.push_back(section);
	}
}

uintptr_t ELF::MapVAToOffset(uintptr_t virtualAddress) {
	for (const ELFProgramHeader& section : this->sections) {
		if (section.p_vaddr <= virtualAddress && virtualAddress <= section.p_vaddr + section.p_filesz)
			return virtualAddress - (section.p_vaddr - section.p_paddr);
	}

	return 0;
}