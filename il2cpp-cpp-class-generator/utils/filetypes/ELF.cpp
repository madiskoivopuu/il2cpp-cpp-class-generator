#include "ELF.h"

ELF::ELF(std::vector<BYTE>& bytes, FileInformation info) : sections({}) {
	this->fileBytes = bytes;
	this->info = info;

	this->elfHeader = *reinterpret_cast<ELFHeader*>(this->fileBytes.data());

	ELFProgramHeader* programSectionList = reinterpret_cast<ELFProgramHeader*>(this->fileBytes.data() + this->elfHeader.e_phoff);
	for (int i = 0; i < this->elfHeader.e_phnum; i++) {
		ELFProgramHeader section = programSectionList[i];
		this->sections.push_back(section);
	}
}

uintptr_t ELF::MapVAToOffset(uintptr_t virtualAddress) {
	for (const ELFProgramHeader& section : this->sections) {
		if (section.p_vaddr <= virtualAddress && virtualAddress <= section.p_vaddr + section.p_memsz)
			return virtualAddress - section.p_vaddr + section.p_offset;
	}

	return 0;
}