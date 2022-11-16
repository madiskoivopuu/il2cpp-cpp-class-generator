#include "UnityVersion.h"
#include <Windows.h>
#include <winver.h>
#include <fstream>
#pragma comment(lib,"Version.lib")
#include <iostream>
#include <vector>

#include "../metadata-processing/default/metadata-file/versions/metadata-v24-0.h"

void ReadUnityVersionBytes(char* fileBytes, int readIndex, UnityVersion& unityVer) {
	char numBuffer[16] = {0};
	int currWriteIndex = 0;
	int step = 0;

	while (fileBytes[readIndex] != 0) {
		if (step > 2) break;

		char* current = &fileBytes[readIndex];
		if (*current == '.' || *current == 'f') {
			switch (step) {
			case 0:
				unityVer.major = atoi(numBuffer);
				break;
			case 1:
				unityVer.minor = atoi(numBuffer);
				break;
			case 2:
				unityVer.build = atoi(numBuffer);
				break;
			}
			// reset buffer to avoid only partially overwriting prev number
			currWriteIndex = 0;
			memset(numBuffer, 0x0, sizeof(numBuffer));

			step++;
		}
		else {
			if (currWriteIndex == sizeof(numBuffer)) throw std::exception("Unity version string too long to read completely");

			numBuffer[currWriteIndex] = *current;
			currWriteIndex++;
		}
		readIndex++;
	}
}

bool GetUnityVersion(char* gameManPath, UnityVersion& unityVer) {
    // only read 63 bytes cuz the version info should be at the beginning
	char fileBytes[64] = {0};
	std::ifstream file(gameManPath, std::ios_base::binary);
	file.read(fileBytes, sizeof(fileBytes)-1);

	ReadUnityVersionBytes(fileBytes, 0x14, unityVer);
	if (unityVer.major < 2000 || unityVer.major > 2100) { // this check will break next century, but by then this project will be dead
		ReadUnityVersionBytes(fileBytes, 0x30, unityVer);
	}

	if (unityVer.major < 2000 || unityVer.major > 2100) {
		throw std::exception("Failed to parse unity version from globalgamemanagers file");
		return false;
	}

	return true;
}

float MetadataVersionFromUnity(std::vector<BYTE> metadataBytes, UnityVersion unityVer) {
    Il2CppGlobalMetadataHeader_v24_0* header = reinterpret_cast<Il2CppGlobalMetadataHeader_v24_0*>(metadataBytes.data());
    switch (header->version) {
    case 29:
        if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2022, 1, 1)) { // v29.1 was more specifically added in a beta version 
            return 29.1f;
        }
        return 29.0f;

    case 27:
        if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2022, 2, 4)) {
            return 27.1f;
        }
        return 27.0f;
    case 24:
        if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2020, 1, 11)) {
            return 24.4f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2020, 0, 0)) {
            return 24.3f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2019, 4, 21)) {
            return 24.5f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2019, 4, 15)) {
            return 24.4f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2019, 3, 7)) {
            return 24.3f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2019, 0, 0)) {
            return 24.2f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2018, 4, 34)) {
            return 24.15f;
        }
        else if (UNITY_VERSION_GREATER_OR_EQUAL(unityVer, 2018, 3, 0)) {
            return 24.1f;
        }
        else {
            return 24.0f;
        }
        break;
    default:
        return static_cast<float>(header->version);
    }
}