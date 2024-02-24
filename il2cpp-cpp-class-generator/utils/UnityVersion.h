#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <Windows.h>

struct UnityVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
};

#define UNITY_VERSION_GREATER_OR_EQUAL(unityVer, _major, _minor, _build) ((unityVer.major >= _major) && (unityVer.minor >= _minor) && (unityVer.build >= _build))

bool GetUnityVersion(char* gameManPath, UnityVersion& unityVer);
std::string MetadataVersionFromUnity(std::vector<BYTE> metadataBytes, UnityVersion unityVer);

