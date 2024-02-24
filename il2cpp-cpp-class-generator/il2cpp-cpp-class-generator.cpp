// il2cpp-cpp-class-cenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <commdlg.h>

#include "dev.h"

#include "metadata-processing/default/metadata-file/metadata.h"

#include "metadata-processing/default/metadata-registration/MetadataRegistration.h"

#include "class-generator/typeinfo.h"
#include "utils/UnityVersion.h"
#include "utils/FileHelper.h"
#include "utils/filetypes/PE.h"

#ifdef _WIN64
#define CURRENT_ARCH FileArch::B64
#else
#define CURRENT_ARCH FileArch::B32
#endif

bool GetUserSelectedFileLoc(char* location) {
    OPENFILENAME selectedFile = { 0 };
    selectedFile.lStructSize = sizeof(OPENFILENAME);
    selectedFile.hwndOwner = NULL;
    selectedFile.lpstrFilter = "Any File\0*.*\0";
    selectedFile.lpstrFile = location; // must be CHAR[]...
    selectedFile.nMaxFile = MAX_PATH;
    selectedFile.lpstrFileTitle = NULL;
    selectedFile.nMaxFileTitle = MAX_PATH;
    selectedFile.lpstrTitle = "Select an input File";
    selectedFile.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    return GetOpenFileName(&selectedFile);
}

int main()
{
    char metadataLoc[MAX_PATH+FILENAME_MAX] = { 0 };
    char il2cppLoc[MAX_PATH+FILENAME_MAX] = { 0 };
    char gameManLoc[MAX_PATH+FILENAME_MAX] = { 0 };

    std::cout << "Choose your global-metadata.dat file..." << std::endl;
    if (!GetUserSelectedFileLoc(metadataLoc)) {
        std::cout << "Selection failed, exiting..." << std::endl;
        std::cin.get();
        return 0;
    }
    std::cout << "Choose your GameAssembly.dll/libil2cpp.so file..." << std::endl;
    if (!GetUserSelectedFileLoc(il2cppLoc)) {
        std::cout << "Selection failed, exiting..." << std::endl;
        std::cin.get();
        return 0;
    }
    
    // TODO: maybe remove in the future
    std::cout << "Choose your globalgamemanagers/unity default resources file..." << std::endl;
    if (!GetUserSelectedFileLoc(gameManLoc)) {
        std::cout << "Selection failed, exiting..." << std::endl;
        std::cin.get();
        return 0;
    }


    // Load il2cpp dll/so and check if the binary is built for the same architecture (32/64 bit) as this executable
    std::vector<BYTE> il2cppBytes = LoadFileAsBinary(il2cppLoc);
    if (!il2cppBytes.size()) {
        std::cout << "Failed to load GameAssembly/il2cpp file." << std::endl;
        std::cin.get();
        return 0;
    }

    FileInformation fileInfo = GetFileInfoFromFileBytes(il2cppBytes);
    IFile* il2cppBinary = nullptr;
    switch(fileInfo.format) {
    case FileType::PE:
        il2cppBinary = new PE(il2cppBytes, fileInfo);
        break;
    default:
        std::cout << "Cannot reverse the il2cpp binary since support hasn't been released for this file format yet." << std::endl;
        return 0;
    }

    if (fileInfo.arch != CURRENT_ARCH) {
        std::cout << "Cannot reverse the il2cpp binary since it isn't built for the same architecture (32/64 bits) as this executable. Please use the appropriate executable." << std::endl;
        std::cout << "Il2cpp binary: " << fileInfo.arch << " bits" << std::endl;
        std::cout << "Current executable: " << CURRENT_ARCH << " bits" << std::endl;
        std::cin.get();
        return 0;
    }

    // Load the metadata bytes into memory to get the main version
    std::vector<BYTE> metadataBytes = LoadFileAsBinary(metadataLoc);
    if (!metadataBytes.size()) {
        std::cout << "Failed to load metadata file." << std::endl;
        std::cin.get();
        return 0;
    }

    Il2CppGlobalMetadataHeader* header = reinterpret_cast<Il2CppGlobalMetadataHeader*>(metadataBytes.data());
    if (header->sanity != 0xFAB11BAF) {
        std::cout << "Invalid sanity number for metadata file." << std::endl;
        std::cin.get();
        return 0;
    }

    UnityVersion ver = { };
    GetUnityVersion(gameManLoc, ver);
    std::string metadataVer = MetadataVersionFromUnity(metadataBytes, ver);

   Il2CppMetadataRegistration metadataRegistration = PatternScanMetadataRegistration(il2cppBinary, header);
    if(metadataRegistration.genericClassesCount == -1) {
        std::cout << "Couldn't find the pointer to metadata registration inside GameAssembly.dll/libil2cpp.so." << std::endl;
        std::cin.get();
        return 0;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
