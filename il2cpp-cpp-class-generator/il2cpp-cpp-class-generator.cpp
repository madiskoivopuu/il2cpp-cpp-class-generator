// il2cpp-cpp-class-cenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <commdlg.h>
#include <fstream>

#include "class-generator/typeinfo.h"
#include "UnityVersion.h"

#include "metadata-processing/default/versions/metadata-v24-0.h"
#include "metadata-processing/default/MetadataProcessor-vXX-X.h"
#include "metadata-processing/default/MetadataProcessorImpl.h"

#define UNITY_VERSION_GREATER_OR_EQUAL(unityVer, _major, _minor, _build) ((unityVer.major >= _major) && (unityVer.minor >= _minor) && (unityVer.build >= _build))

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
    char gameManLoc[MAX_PATH+FILENAME_MAX] = { 0 };

    std::cout << "Choose your global-metadata.dat file..." << std::endl;
    if (!GetUserSelectedFileLoc(metadataLoc)) {
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

    UnityVersion version = { 0 };
    if (!GetUnityVersion(gameManLoc, version)) {
        std::cout << "Failed to get unity version from provided globalgamemanagers." << std::endl;
        std::cin.get();
        return 0;
    }

    // Load the metadata bytes into memory to get the main version
    void* metadataBytes = LoadMetadataFile(metadataLoc);
    if (!metadataBytes) {
        std::cout << "Failed to load metadata file." << std::endl;
        std::cin.get();
        return 0;
    }

    Il2CppGlobalMetadataHeader* header = static_cast<Il2CppGlobalMetadataHeader*>(metadataBytes);
    if (header->sanity != 0xFAB11BAF) {
        std::cout << "Invalid sanity number for metadata file." << std::endl;
        std::cin.get();
        return 0;
    }

    // now we parse the metadata based on the version
    switch (header->version) {
    case 29:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2022, 1, 1)) { // v29.1 was more specifically added in a beta version 
            std::cout << "Metadata v29.1 parsing not implemented yet" << std::endl;
            return 0;
        }
        ParseMetadata_v29_0(metadataBytes);
        break;
    
    case 28:
        ParseMetadata_v27_9(metadataBytes);
        break;
    case 27:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2022, 2, 4)) {
            ParseMetadata_v27_1(metadataBytes);
        }
        else {
            ParseMetadata_v27_0(metadataBytes);
        }
        break;
        // v27.9??
    case 26: 
    case 25:
        ParseMetadata_v24_5(metadataBytes);
        break;
    case 24:
        if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2020, 1, 11)) {
            ParseMetadata_v24_4(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2020, 0, 0)) {
            ParseMetadata_v24_3(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 4, 21)) {
            ParseMetadata_v24_5(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 4, 15)) {
            ParseMetadata_v24_4(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 3, 7)) {
            ParseMetadata_v24_3(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2019, 0, 0)) {
            ParseMetadata_v24_2(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2018, 4, 34)) {
            ParseMetadata_v24_15(metadataBytes);
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(version, 2018, 3, 0)) {
            ParseMetadata_v24_1(metadataBytes);
        }
        else {
            ParseMetadata_v24_0(metadataBytes);
        }
        break;
    }

    //std::vector<Il2cppImageData> il2cppImages = ParseMetadata(metadataBytes);


    //free(metadataBytes);

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
