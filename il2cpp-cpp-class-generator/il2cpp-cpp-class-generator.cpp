// il2cpp-cpp-class-cenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <commdlg.h>
#include <winver.h>
#pragma comment(lib,"Version.lib")

#include "metadata-processing/default/MetadataProcessor.h"
#include "metadata-processing/typeinfo.h"


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

struct UnityVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
};
bool GetUnityVersion(char* unityPath, UnityVersion& unityVer) {
    int verInfoSize = GetFileVersionInfoSize(unityPath, 0);
    if (verInfoSize == 0) return false;

    BYTE* data = new BYTE[verInfoSize];
    LPVOID queryPtr = 0;
    unsigned int size = 0;
    if (!GetFileVersionInfoA(unityPath, 0, verInfoSize, data)) return false;

    if (!VerQueryValue(data, "\\", &queryPtr, &size)) return false;
    if (!size) return false;

    VS_FIXEDFILEINFO* fileInfo = static_cast<VS_FIXEDFILEINFO*>(queryPtr);
    unityVer.major = HIWORD(fileInfo->dwFileVersionMS);
    unityVer.minor = LOWORD(fileInfo->dwFileVersionMS);
    unityVer.build = HIWORD(fileInfo->dwProductVersionLS);
}

int main()
{

    char metadataLoc[MAX_PATH+FILENAME_MAX] = { 0 };
    char unityLoc[MAX_PATH+FILENAME_MAX] = { 0 };

    std::cout << "Choose your global-metadata.dat file..." << std::endl;
    if (!GetUserSelectedFileLoc(metadataLoc)) {
        std::cout << "Selection failed, exiting..." << std::endl;
        std::cin.get();
        return 0;
    }
    std::cout << "Choose your UnityPlayer.dll/libunity.so file..." << std::endl;
    if (!GetUserSelectedFileLoc(unityLoc)) {
        std::cout << "Selection failed, exiting..." << std::endl;
        std::cin.get();
        return 0;
    }

    UnityVersion version = { 0 };
    if (!GetUnityVersion(unityLoc, version)) {
        std::cout << "Failed to get unity version from provided dll/so." << std::endl;
        std::cin.get();
        return 0;
    }

    std::cout << version.major << std::endl;
    std::cout << version.minor << std::endl;
    std::cout << version.build << std::endl;

     //void* metadataBytes = LoadMetadataFile(metadataLoc);
    // if (!metadataBytes) return 0;

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
