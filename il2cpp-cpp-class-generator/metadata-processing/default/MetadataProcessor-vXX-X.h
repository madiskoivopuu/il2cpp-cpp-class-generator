#pragma once
#include "../../class-generator/typeinfo.h"

#include <vector>

// Function declarations for different metadata versions
std::vector<Il2cppImageData> ParseMetadata_v23_0(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_0(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_1(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_15(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_2(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_3(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_4(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v24_5(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v27_0(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v27_1(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v27_9(void* metadataBytes);
std::vector<Il2cppImageData> ParseMetadata_v29_0(void* metadataBytes);
