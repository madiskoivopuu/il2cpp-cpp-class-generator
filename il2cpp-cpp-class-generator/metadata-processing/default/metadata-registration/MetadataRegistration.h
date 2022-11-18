#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

#include "../metadata-file/versions/metadata-v24-0.h"

// 32 and 64 bit structs starting from metadata version 19
struct Il2CppMetadataRegistration_B32 // we will use this one so the alignment stays correct
{
    int32_t genericClassesCount;
    int32_t* genericClasses;
    int32_t genericInstsCount;
    int32_t* genericInsts;
    int32_t genericMethodTableCount;
    int32_t* genericMethodTable;
    int32_t typesCount;
    int32_t* types;
    int32_t methodSpecsCount;
    int32_t* methodSpecs;
    int32_t fieldOffsetsCount;
    int32_t** fieldOffsets;
    int32_t typeDefinitionsSizesCount;
    int32_t* typeDefinitionsSizes;
    int32_t metadataUsagesCount;
    int32_t* metadataUsages;
};

struct Il2CppMetadataRegistration_B64
{
    int32_t genericClassesCount;
    void* genericClasses;
    int32_t genericInstsCount;
    void* genericInsts;
    int32_t genericMethodTableCount;
    void* genericMethodTable;
    int32_t typesCount;
    void* types;
    int32_t methodSpecsCount;
    void* methodSpecs;
    int32_t fieldOffsetsCount;
    const int32_t** fieldOffsets;
    int32_t typeDefinitionsSizesCount;
    void* typeDefinitionsSizes;
    uint64_t metadataUsagesCount;
    void* metadataUsages;
};

Il2CppMetadataRegistration_B64* GetMedatataRegistrationPtr(std::vector<BYTE>& il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion);