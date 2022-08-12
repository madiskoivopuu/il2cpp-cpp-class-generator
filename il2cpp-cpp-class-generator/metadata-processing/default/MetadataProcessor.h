#pragma once

#include <vector>
#include "../typeinfo.h"

std::vector<AssemblyData> ParseMetadata(void* metadataBytes);
void* LoadMetadataFile(char* filePath);