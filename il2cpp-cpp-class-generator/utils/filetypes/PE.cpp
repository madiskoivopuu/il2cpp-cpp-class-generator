#include "PE.h"

PE::PE(std::vector<BYTE>& bytes, FileInformation info) 
	: sections(), dosHeader({}), imageNTHeaders({})
{
	this->fileBytes = bytes;
	this->info = info;

	this->dosHeader = *reinterpret_cast<IMAGE_DOS_HEADER*>(this->fileBytes.data());
	this->imageNTHeaders = *reinterpret_cast<IMAGE_NT_HEADERS*>(this->fileBytes.data() + dosHeader.e_lfanew);

	// push all sections to the vector so we can convert addresses later on
	
	IMAGE_SECTION_HEADER* firstSection = IMAGE_FIRST_SECTION(reinterpret_cast<IMAGE_NT_HEADERS*>(this->fileBytes.data() + dosHeader.e_lfanew));
	for (int i = 0; i < this->imageNTHeaders.FileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER section = firstSection[i];
		this->sections.push_back(section);
	}
}

uintptr_t PE::MapVAToOffset(uintptr_t address) {
	if (address < this->imageNTHeaders.OptionalHeader.ImageBase) return 0;

	// return offset from the start of file if we find the right section
	address -= this->imageNTHeaders.OptionalHeader.ImageBase;
	for (const auto& section : this->sections) {
		if (section.VirtualAddress <= address && address <= section.VirtualAddress + section.Misc.VirtualSize) {
			return address - section.VirtualAddress + section.PointerToRawData;
		}
	}

	return 0;
}