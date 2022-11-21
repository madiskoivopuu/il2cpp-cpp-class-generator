#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

#include "../metadata-file/versions/metadata-v24-0.h"
#include "../../../il2cpp/il2cpp-binarystructs.h"

// 32 and 64 bit structs starting from metadata version 19
struct Il2CppMetadataRegistration_B32 // we will use this one so the alignment stays correct for 32 bit binaries
{
    int32_t genericClassesCount;
    int32_t pGenericClasses;
    int32_t genericInstsCount;
    int32_t pGenericInsts;
    int32_t genericMethodTableCount;
    int32_t pGenericMethodTable;
    int32_t typesCount;
    int32_t pTypes;
    int32_t methodSpecsCount;
    int32_t pMethodSpecs;
    int32_t fieldOffsetsCount;
    int32_t pFieldOffsets;
    int32_t typeDefinitionsSizesCount;
    int32_t pTypeDefinitionsSizes;
    int32_t metadataUsagesCount;
    int32_t pMetadataUsages;
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
    Il2CppType* types;
    int32_t methodSpecsCount;
    void* methodSpecs;
    int32_t fieldOffsetsCount;
    void** fieldOffsets;
    int32_t typeDefinitionsSizesCount;
    void* typeDefinitionsSizes;
    uint64_t metadataUsagesCount;
    void* metadataUsages;
};

Il2CppMetadataRegistration_B64 GetMedatataRegistration(std::vector<BYTE>& il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion);