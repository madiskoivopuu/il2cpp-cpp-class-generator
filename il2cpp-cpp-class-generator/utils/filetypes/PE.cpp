#include "PE.h"

PE::PE(std::vector<BYTE>& bytes, FileInformation info) 
	: sections(), dosHeader({}), imageNTHeaders({})
{
	this->fileBytes = bytes;
	this->info = info;

	this->dosHeader = *reinterpret_cast<IMAGE_DOS_HEADER*>(this->fileBytes.data());
	this->imageNTHeaders = *reinterpret_cast<IMAGE_NT_HEADERS*>(this->fileBytes.data() + dosHeader.e_lfanew);

	// push all sections to the vector so we can convert addresses later on
	IMAGE_SECTION_HEADER* firstSection = reinterpret_cast<IMAGE_SECTION_HEADER*>(this->fileBytes.data() + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + this->imageNTHeaders.FileHeader.SizeOfOptionalHeader);
	for (int i = 0; i < this->imageNTHeaders.FileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER section = firstSection[i];
		this->sections.push_back(section);
	}
}

uintptr_t PE::MapVAToReal(uintptr_t virtualAddress) {
	if (virtualAddress < this->imageNTHeaders.OptionalHeader.ImageBase) return 0;

	// return raw address in this loop if we find the right section
	virtualAddress -= this->imageNTHeaders.OptionalHeader.ImageBase;
	for (const auto& section : this->sections) {
		if (section.VirtualAddress <= virtualAddress && virtualAddress <= section.VirtualAddress + section.Misc.VirtualSize) {
			return virtualAddress - section.VirtualAddress - section.PointerToRawData;
		}
	}

	return 0;
}