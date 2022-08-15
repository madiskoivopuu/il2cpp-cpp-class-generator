#pragma once

#include <vector>
#include "../typeinfo.h"

std::vector<Il2cppImageData> ParseMetadata(void* metadataBytes);
void* LoadMetadataFile(char* filePath);