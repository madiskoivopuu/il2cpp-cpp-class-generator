#include <vector>
#include <Windows.h>
#include <iostream>

#include "MetadataRegistration.h"
#include "../../../utils/FileHelper.h"
#include "../metadata-file/versions/metadata-v24-0.h"
#include "../metadata-file/versions/metadata-v24-15.h"
#include "../metadata-file/versions/metadata-v24-5.h"
#include "../metadata-file/versions/metadata-v29-0.h"

#ifdef _WIN64 
#define WORD_SIZE 8
#else
#define WORD_SIZE 4
#endif

// Pattern scans 3 specific int32_t's in memory that are contiguous. If found, will return the the metadata registration struct (in our own memory).
// NB! Virtual addresses (pointers in the struct) still have to be converted to the appropriate address
Il2CppMetadataRegistration GetMedatataRegistration(std::vector<BYTE>& il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion) {
	uintptr_t pLocalIl2cppBytes = reinterpret_cast<uintptr_t>(il2cppBytes.data());
	Il2CppMetadataRegistration metadataReg = { -1 };
	

	// pattern scan configuration
	// struct is aligned differently on 32bit/64bit arch, for the pattern scan to work properly we have to keep track of this
	uintptr_t wordSize = WORD_SIZE;

	// calculate the types count & method specs count
	int32_t searchTypesCount = 0;
	if (metadataVersion <= 24.0f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_0);
	else if (metadataVersion <= 24.15f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_15);
	else if (metadataVersion <= 24.5f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v24_5);
	else if (metadataVersion <= 29.0f) searchTypesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition_v29_0);
	int32_t searchMethodSpecsCount = header->metadataUsageListsCount / sizeof(Il2CppMetadataUsageList);

	for (uintptr_t i = 0; i < il2cppBytes.size() - wordSize*8; i += wordSize) {
		int32_t methodSpecsCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i]);
		int32_t fieldOffsetsCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i + 2*wordSize]);
		int32_t typeDefsSizesCount = *reinterpret_cast<int32_t*>(&il2cppBytes[i + 4*wordSize]);

		// TODO: check if this method actually works for unity version under 24.5...
		if (metadataVersion < 24.5f) {
			if (typeDefsSizesCount == searchTypesCount && methodSpecsCount == searchMethodSpecsCount) {
				metadataReg = *reinterpret_cast<Il2CppMetadataRegistration*>(il2cppBytes.data() + i - 8 * wordSize);
				break;
			}
		}
		else if (typeDefsSizesCount == searchTypesCount && fieldOffsetsCount == searchTypesCount) {
			metadataReg = *reinterpret_cast<Il2CppMetadataRegistration*>(il2cppBytes.data() + i - 8 * wordSize);
			break;
		}
	}

	return metadataReg;
}