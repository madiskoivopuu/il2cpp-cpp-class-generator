#pragma once
#include <vector>
#include <Windows.h>

enum class FileType : unsigned int
{
	UNKNOWN = 0,
	PE,
	ELF,
	MACHO
};
enum FileArch : unsigned int
{
	UNKNOWN = 0,
	B32 = 32,
	B64 = 64
};
struct FileInformation {
	FileType format;
	FileArch arch;
};

class IFileHelper {
public:
	FileInformation info;
	std::vector<BYTE> fileBytes;
	bool fileLoaded;

	 IFileHelper(char* filePath) : info(), fileLoaded(false)
	{
		this->GetFileInfo(filePath);
		this->LoadSectionInfo();
	}

	virtual void LoadSectionInfo() = 0;
	virtual void MapVAToReal(uintptr_t virtualAddress) = 0;

private:
	virtual void GetFileInfoFromFileBytes();
	virtual std::vector<BYTE> LoadFileAsBinary(char* filePath);
	virtual void GetFileInfo(char* filePath);
};

