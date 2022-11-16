#include <vector>
#include <Windows.h>
#include <iostream>

#include "MetadataRegistration.h"
#include "../../../utils/FileHelper.h"
#include "../metadata-file/versions/metadata-v24-0.h"
#include "../metadata-file/versions/metadata-v24-15.h"
#include "../metadata-file/versions/metadata-v24-5.h"
#include "../metadata-file/versions/metadata-v29-0.h"

// Pattern scans 3 specific int32_t's in memory that are contiguous. If found, will return the pointer to the metadata registration.
Il2CppMetadataRegistration_B64* GetMedatataRegistrationPtr(std::vector<BYTE> il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion) {
	FileInformation fileInfo = GetFileInfoFromFileBytes(il2cppBytes);
	if (fileInfo.arch == FileArch::UNKNOWN) return nullptr;

	char* pMetadataRegistration = nullptr;

	// pattern scan configuration
	int wordSize = 4;
	if (fileInfo.arch == FileArch::B64) { // struct is aligned differently on 32bit/64bit arch, for the pattern scan to work properly we have to keep track of this
		wordSize = 8;
	}

	// calculate the types count & 
	int32_t searchTypesCount = 0;
	if (metadataVersion <= 24.0f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_0);
	else if (metadataVersion <= 24.15f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_15);
	else if (metadataVersion <= 24.5f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_5);
	else if (metadataVersion <= 29.0f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v29_0);

	int32_t searchMethodSpecsCount = header->metadataUsageListsCount / sizeof(Il2CppMetadataUsageList);

	for (int i = 0; i < il2cppBytes.size() - wordSize*8; i += wordSize) {		
		int32_t methodSpecsCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i]);
		int32_t fieldOffsetsCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i + 2*wordSize]);
		int32_t typeDefsSizesCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i + 4*wordSize]);

		// TODO: check if this method actually works for unity version under 24.5...
		if (metadataVersion < 24.5f) {
			if (typeDefsSizesCount == searchTypesCount && methodSpecsCount == searchMethodSpecsCount) {
				return reinterpret_cast<Il2CppMetadataRegistration_B64*>(i - 8 * wordSize);
			}
		}
		else if (typeDefsSizesCount == searchTypesCount && fieldOffsetsCount == searchTypesCount) {
			return reinterpret_cast<Il2CppMetadataRegistration_B64*>(i - 8 * wordSize);
		}
	}

	return nullptr;
}