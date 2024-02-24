#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

#include "../../utils/FileHelper.h"
#include "../../utils/filetypes/IFile.h"
#include "metadata.h"
#include "../../il2cpp/il2cpp-binarystructs.h"

struct Il2CppGlobalMetadataHeader;

struct Il2CppMetadataRegistration
{
    int32_t genericClassesCount;
    int32_t** genericClasses;
    int32_t genericInstsCount;
    int32_t** genericInsts;
    int32_t genericMethodTableCount;
    int32_t* genericMethodTable;
    int32_t typesCount;
    Il2CppType** types;
    int32_t methodSpecsCount;
    int32_t* methodSpecs;
    int32_t fieldOffsetsCount;
    int32_t** fieldOffsets;
    int32_t typeDefinitionsSizesCount;
    int32_t** typeDefinitionsSizes;
    int32_t metadataUsagesCount;
    int32_t** metadataUsages;
};

Il2CppMetadataRegistration PatternScanMetadataRegistration(IFile* il2cppBytes, Il2CppGlobalMetadataHeader* header);