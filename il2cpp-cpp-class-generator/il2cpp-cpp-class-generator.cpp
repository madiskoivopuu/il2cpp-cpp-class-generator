// il2cpp-cpp-class-cenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <commdlg.h>

#include "metadata-processing/default/metadata-file/versions/metadata-v23-0.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-0.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-1.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-15.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-2.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-3.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-4.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v24-5.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v27-0.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v27-1.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v27-9.h"
#include "metadata-processing/default/metadata-file/versions/metadata-v29-0.h"
#include "metadata-processing/default/metadata-file/MetadataProcessorImpl.h"

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
    std::cout << "Choose your globalgamemanagers file..." << std::endl;
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

    Il2CppGlobalMetadataHeader_v24_0* header = reinterpret_cast<Il2CppGlobalMetadataHeader_v24_0*>(metadataBytes.data());
    if (header->sanity != 0xFAB11BAF) {
        std::cout << "Invalid sanity number for metadata file." << std::endl;
        std::cin.get();
        return 0;
    }

    // unity version detection
    UnityVersion version = { 0 };
    if (!GetUnityVersion(gameManLoc, version)) {
        std::cout << "Failed to get unity version from provided globalgamemanagers." << std::endl;
        std::cin.get();
        return 0;
    }

   Il2CppMetadataRegistration metadataRegistration = GetMedatataRegistration(il2cppBinary, header, MetadataVersionFromUnity(metadataBytes, version));
    if(metadataRegistration.genericClassesCount == -1) {
        std::cout << "Couldn't find the pointer to metadata registration inside GameAssembly.dll/libil2cpp.so." << std::endl;
        std::cin.get();
        return 0;
    }

    // now we parse the metadata based on the version
    std::vector<Il2cppImageData> classesData;
    switch (header->version) {
    case 29:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2022, 1, 1)) { // v29.1 was more specifically added in a beta version 
            std::cout << "Metadata v29.1 parsing not implemented yet" << std::endl;
            return 0;
        }
        classesData = ParseMetadata<IL2CPP_TEMPLATES_V29_0>(metadataBytes, il2cppBinary, metadataRegistration);
        break;
    
    case 28:
        classesData = ParseMetadata<IL2CPP_TEMPLATES_V27_9>(metadataBytes, il2cppBinary, metadataRegistration);
        break;
    case 27:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2022, 2, 4)) { // v27.1
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V27_1>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else { // v27.0
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V27_0>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        break;
        // v27.9??
    case 26: 
    case 25:
        classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_5>(metadataBytes, il2cppBinary, metadataRegistration);
        break;
    case 24:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2020, 1, 11)) { // v24.4
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_5>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2020, 0, 0)) { // v24.3
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_3>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 4, 21)) { // v24.5
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_5>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 4, 15)) { // v24.4
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_4>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 3, 7)) { // v24.3
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_3>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 0, 0)) { // v24.2
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_2>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2018, 4, 34)) { // v24.15
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_15>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2018, 3, 0)) { // v24.1
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_1>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        else { // v24
            classesData = ParseMetadata<IL2CPP_TEMPLATES_V24_0>(metadataBytes, il2cppBinary, metadataRegistration);
        }
        break;
    case 23:
        classesData = ParseMetadata<IL2CPP_TEMPLATES_V23_0>(metadataBytes, il2cppBinary, metadataRegistration);
        break;
    default:
        std::cout << "Unsupported metadata version " << header->version << std::endl;
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
