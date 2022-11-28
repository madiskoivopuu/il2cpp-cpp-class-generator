#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

#include "../metadata-file/versions/metadata-v24-0.h"
#include "../../../il2cpp/il2cpp-binarystructs.h"

struct Il2CppMetadataRegistration
{
    int32_t genericClassesCount;
    int32_t* genericClasses;
    int32_t genericInstsCount;
    int32_t* genericInsts;
    int32_t genericMethodTableCount;
    int32_t* genericMethodTable;
    int32_t typesCount;
    Il2CppType* types;
    int32_t methodSpecsCount;
    int32_t* methodSpecs;
    int32_t fieldOffsetsCount;
    int32_t** fieldOffsets;
    int32_t typeDefinitionsSizesCount;
    int32_t* typeDefinitionsSizes;
    uint64_t metadataUsagesCount;
    int32_t* metadataUsages;
};

Il2CppMetadataRegistration GetMedatataRegistration(std::vector<BYTE>& il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion);