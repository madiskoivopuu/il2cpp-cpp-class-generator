#pragma once
#include "IFile.h"

class PE : public IFile {
public:
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS imageNTHeaders;
	std::vector<IMAGE_SECTION_HEADER> sections;

	PE(std::vector<BYTE>& bytes, FileInformation info);
	uintptr_t MapVAToReal(uintptr_t virtualAddress);
};