#include "UnityVersion.h"
#include <Windows.h>
#include <winver.h>
#include <fstream>
#pragma comment(lib,"Version.lib")

#include <iostream>

void ReadUnityVersionBytes(char* fileBytes, int readIndex, UnityVersion& unityVer) {
	char numBuffer[12] = {0};
	int currWriteIndex = 0;
	int step = 0;

	while (fileBytes[readIndex] != 0) {
		char* current = &fileBytes[readIndex];
		if (step > 2) break;

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
	char fileBytes[64] = {0};
	std::ifstream file(gameManPath, std::ios_base::binary);

	file.read(fileBytes, sizeof(fileBytes)-1);

	ReadUnityVersionBytes(fileBytes, 0x14, unityVer);
	if (unityVer.major < 2000 || unityVer.major > 2100) {
		ReadUnityVersionBytes(fileBytes, 0x30, unityVer);
	}

	if (unityVer.major < 2000 || unityVer.major > 2100) {
		throw std::exception("Failed to parse unity version from globalgamemanagers file");
		return false;
	}

	return true;
}