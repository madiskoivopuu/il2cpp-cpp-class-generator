#pragma once
#include "../FileHelper.h"
class IFile {
public:
	FileInformation info;
	std::vector<BYTE> fileBytes;

	virtual uintptr_t MapVAToReal(uintptr_t virtualAddress) = 0;
};