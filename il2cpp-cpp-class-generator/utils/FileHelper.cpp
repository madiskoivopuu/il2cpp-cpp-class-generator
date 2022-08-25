#include "FileHelper.h"

#include <fstream>
#include <Windows.h>

FileInformation GetFileInfoFromFileBytes(char* fileBytes) {
    FileInformation info = { FileType::UNKNOWN, FileArch::UNKNOWN };
    if (!fileBytes) return info;

    unsigned int fileMagicNr = static_cast<unsigned int>(*fileBytes);
    if (fileMagicNr == 0x4D5Au) { // PE
        info.format = FileType::PE; 
    }
    else if(fileMagicNr == 0x7F454C46u) {
        info.format = FileType::ELF;
        uint8_t is32or64bit = static_cast<uint8_t>(*(fileBytes+0x4));
        if (is32or64bit == 1) {
            info.arch = FileArch::B32;
        }
        else if (is32or64bit == 2) {
            info.arch = FileArch::B64;
        }
    }

    return info;
}

FileInformation GetFileInfo(char* filePath) {
    void* buf = LoadFileAsBinary(filePath);
    FileInformation info = GetFileInfoFromFileBytes(static_cast<char*>(buf));
    free(buf);

    return info;
}

void* LoadFileAsBinary(char* filePath) {
    std::ifstream file(filePath, std::ios_base::binary);

    // get the length of file to malloc a buffer
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    void* buffer = malloc(length);
    file.read(static_cast<char*>(buffer), length);

    if (!file) {
        free(buffer);
        throw std::exception("Failed to fully load file as binary");
    }

    return buffer;
}