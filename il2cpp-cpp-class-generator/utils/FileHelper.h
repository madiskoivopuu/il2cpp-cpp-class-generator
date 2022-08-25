#pragma once
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
	B32,
	B64
};

struct FileInformation {
	FileType format;
	FileArch arch;
};

FileInformation GetFileInfoFromFileBytes(char* fileBytes);
FileInformation GetFileInfo(char* filePath);
std::vector<BYTE> LoadFileAsBinary(char* filePath);

