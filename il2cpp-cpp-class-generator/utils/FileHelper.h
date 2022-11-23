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

FileInformation GetFileInfoFromFileBytes(std::vector<BYTE> fileBytes);
FileInformation GetFileInfo(char* filePath);
std::vector<BYTE> LoadFileAsBinary(char* filePath);

