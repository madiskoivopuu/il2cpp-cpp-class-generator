#pragma once
#include "../../class-generator/typeinfo.h"
#include "../../il2cpp/il2cpp-tabledefs.h"
#include "../../il2cpp/il2cpp-binarystructs.h"
#include "../../utils/filetypes/IFile.h"

#include "MetadataRegistration.h"

std::vector<Il2cppImageData> ParseMetadata(std::vector<BYTE>& metadataBytes, IFile* il2cppBinary, Il2CppMetadataRegistration metadataRegistration);