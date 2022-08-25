#pragma once
#include <cstdint>

struct UnityVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
};

#define UNITY_VERSION_GREATER_OR_EQUAL(unityVer, _major, _minor, _build) ((unityVer.major >= _major) && (unityVer.minor >= _minor) && (unityVer.build >= _build))

bool GetUnityVersion(char* gameManPath, UnityVersion& unityVer);
float MetadataVersionFromUnity(void* metadataBytes, UnityVersion unityVer);

