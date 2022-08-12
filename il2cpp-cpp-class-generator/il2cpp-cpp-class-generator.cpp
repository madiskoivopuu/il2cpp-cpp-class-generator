// il2cpp-cpp-class-cenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <commdlg.h>

#include "metadata-processing/default/MetadataProcessor.h"
#include "metadata-processing/typeinfo.h"

int main()
{
	std::cout << "Choose your global-metadata.dat file..." << std::endl;

    char output[MAX_PATH] = { 0 };

	OPENFILENAME selectedFile = { 0 };
    selectedFile.lStructSize = sizeof(OPENFILENAME);
    selectedFile.hwndOwner = NULL;
    selectedFile.lpstrFilter = "Any File\0*.*\0";
    selectedFile.lpstrFile = output; // must be CHAR[]...
    selectedFile.nMaxFile = MAX_PATH;
    selectedFile.lpstrFileTitle = NULL;
    selectedFile.nMaxFileTitle = MAX_PATH;
    selectedFile.lpstrTitle = "Select an input File";
    selectedFile.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&selectedFile)) {
        void* metadataBytes = LoadMetadataFile(selectedFile.lpstrFile);
        if (!metadataBytes) return 0;

        std::vector<AssemblyData> il2cppImages = ParseMetadata(metadataBytes);


        free(metadataBytes);
    }
    else {
        std::cout << "You failed to select any file." << std::endl;
        std::cin.get();
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
