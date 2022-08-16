#pragma once
#include <cstdint>

struct UnityVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
};

bool GetUnityVersion(char* gameManPath, UnityVersion& unityVer);

