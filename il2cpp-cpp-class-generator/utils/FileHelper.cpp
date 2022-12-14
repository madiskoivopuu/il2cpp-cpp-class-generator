#include "FileHelper.h"

#include <cstdint>
#include <fstream>
#include <vector>
#include <Windows.h>


FileInformation GetFileInfoFromFileBytes(std::vector<BYTE> fileBuffer) {
    FileInformation info = { FileType::UNKNOWN, FileArch::UNKNOWN };
    if (!fileBuffer.size()) return info;

    BYTE* fileBytes = fileBuffer.data();
    // file magic number checks
    if (fileBytes[0] == 0x4D && fileBytes[1] == 0x5A) { // PE
        info.format = FileType::PE; 
        // PE files keep an offset to the signature at offset 0x3c
        // after the signature the first 2 bytes signify the machine type
        unsigned int offset = *reinterpret_cast<unsigned int*>(fileBytes + 0x3c);
        uint16_t machineType = *reinterpret_cast<uint16_t*>(fileBytes + offset + 0x4);
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
    else if(fileBytes[0] == 0x7F && fileBytes[1] == 0x45 && fileBytes[2] == 0x4C && fileBytes[3] == 0x46) { // ELF
        info.format = FileType::ELF;
        uint8_t is32or64bit = static_cast<uint8_t>(fileBytes[0x4]);
        if (is32or64bit == 1) {
            info.arch = FileArch::B32;
        }
        else if (is32or64bit == 2) {
            info.arch = FileArch::B64;
        }
        else throw std::exception("Could not correctly determine whether ELF is 32/64 bit binary.");
    }

    return info;
}

std::vector<BYTE> LoadFileAsBinary(char* filePath) {
    std::ifstream file(filePath, std::ios::binary);
    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve enough memory to read all bytes to vector
    std::vector<BYTE> fileBytes;
    fileBytes.resize(fileSize);

    file.read(reinterpret_cast<char*>(fileBytes.data()), fileSize);

    fileBytes.resize(file.gcount());
    return fileBytes;
}

FileInformation GetFileInfo(char* filePath) {
    std::vector<BYTE> fileBytes = LoadFileAsBinary(filePath);
    return GetFileInfoFromFileBytes(fileBytes);
}