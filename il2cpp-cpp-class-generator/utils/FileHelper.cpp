#include "FileHelper.h"

#include <cstdint>
#include <fstream>
#include <vector>
#include <Windows.h>

FileInformation GetFileInfoFromFileBytes(std::vector<BYTE> fileBuffer) {
    FileInformation info = { FileType::UNKNOWN, FileArch::UNKNOWN };
    if (!fileBuffer.size()) return info;

    BYTE* fileBytes = fileBuffer.data();
    unsigned int fileMagicNr = static_cast<unsigned int>(*fileBytes);
    if (fileMagicNr == 0x4D5Au) { // PE
        info.format = FileType::PE; 
        // PE files keep an offset to the signature at offset 0x3c
        // after the signature the first 2 bytes signify the machine type
        unsigned int offset = static_cast<unsigned int>(*(fileBytes + 0x3c));
        uint16_t machineType = static_cast<uint16_t>(*(fileBytes + offset + 0x4));
        switch (machineType) {
        case IMAGE_FILE_MACHINE_AMD64:
        case IMAGE_FILE_MACHINE_ARM64:
        case IMAGE_FILE_MACHINE_IA64:
            info.arch = FileArch::B64;
            break;

        default:
            info.arch = FileArch::B32;
            break;
        }
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
    std::vector<BYTE> buf = LoadFileAsBinary(filePath);
    FileInformation info = GetFileInfoFromFileBytes(buf);

    return info;
}

std::vector<BYTE> LoadFileAsBinary(char* filePath) {
    std::basic_ifstream<BYTE> file(filePath, std::ios::binary);

    return std::vector<BYTE>((std::istreambuf_iterator<BYTE>(file)),
        std::istreambuf_iterator<BYTE>());
}