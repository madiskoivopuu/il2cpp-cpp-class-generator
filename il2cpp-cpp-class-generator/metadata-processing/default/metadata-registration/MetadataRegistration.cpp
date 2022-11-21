#include <vector>
#include <Windows.h>
#include <iostream>

#include "MetadataRegistration.h"
#include "../../../utils/FileHelper.h"
#include "../metadata-file/versions/metadata-v24-0.h"
#include "../metadata-file/versions/metadata-v24-15.h"
#include "../metadata-file/versions/metadata-v24-5.h"
#include "../metadata-file/versions/metadata-v29-0.h"

// Pattern scans 3 specific int32_t's in memory that are contiguous. If found, will return the pointer to the metadata registration (in our own memory).
Il2CppMetadataRegistration_B64 GetMedatataRegistration(std::vector<BYTE>& il2cppBytes, Il2CppGlobalMetadataHeader_v24_0* header, float metadataVersion) {
	int64_t pMetadataReg = 0;
	int64_t pLocalIl2cppBytes = reinterpret_cast<uintptr_t>(il2cppBytes.data());
	Il2CppMetadataRegistration_B64 fixedMetadataReg = { -1 };
	

	FileInformation fileInfo = GetFileInfoFromFileBytes(il2cppBytes);
	if (fileInfo.arch == FileArch::UNKNOWN) return Il2CppMetadataRegistration_B64{};

	// pattern scan configuration
	int wordSize = 4;
	if (fileInfo.arch == FileArch::B64) { // struct is aligned differently on 32bit/64bit arch, for the pattern scan to work properly we have to keep track of this
		wordSize = 8;
	}

	// calculate the types count & method specs count
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
				pMetadataReg = reinterpret_cast<int64_t>(il2cppBytes.data() + i - 8 * wordSize);
				break;
			}
		}
		else if (typeDefsSizesCount == searchTypesCount && fieldOffsetsCount == searchTypesCount) {
			pMetadataReg = reinterpret_cast<int64_t>(il2cppBytes.data() + i - 8 * wordSize);
			break;
		}
	}

	// dereference the pointer based on wordSize to keep everything aligned properly
	// and convert each virtual address to an actual address our program can use
	if (pMetadataReg) {
		if (fileInfo.arch == FileArch::B32) {
			fixedMetadataReg.genericClassesCount = *reinterpret_cast<int32_t*>(pMetadataReg);
			fixedMetadataReg.genericClasses = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 1 * wordSize));
			fixedMetadataReg.genericInstsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 2 * wordSize);
			fixedMetadataReg.genericInsts = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 3 * wordSize));
			fixedMetadataReg.genericMethodTableCount = *reinterpret_cast<int32_t*>(pMetadataReg + 4 * wordSize);
			fixedMetadataReg.genericMethodTable = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 5 * wordSize));
			fixedMetadataReg.typesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 6 * wordSize);
			fixedMetadataReg.types = reinterpret_cast<Il2CppType*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 7 * wordSize));
			fixedMetadataReg.methodSpecsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 8 * wordSize);
			fixedMetadataReg.methodSpecs = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 9 * wordSize));
			fixedMetadataReg.fieldOffsetsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 10 * wordSize);
			fixedMetadataReg.fieldOffsets = reinterpret_cast<void**>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 11 * wordSize));
			fixedMetadataReg.typeDefinitionsSizesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 12 * wordSize);
			fixedMetadataReg.typeDefinitionsSizes = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 13 * wordSize));
			fixedMetadataReg.metadataUsagesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 14 * wordSize);
			fixedMetadataReg.metadataUsages = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int32_t*>(pMetadataReg + 15 * wordSize));
		}
		else {
			fixedMetadataReg.genericClassesCount = *reinterpret_cast<int32_t*>(pMetadataReg);
			fixedMetadataReg.genericClasses = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 1 * wordSize));
			fixedMetadataReg.genericInstsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 2 * wordSize);
			fixedMetadataReg.genericInsts = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 3 * wordSize));
			fixedMetadataReg.genericMethodTableCount = *reinterpret_cast<int32_t*>(pMetadataReg + 4 * wordSize);
			fixedMetadataReg.genericMethodTable = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 5 * wordSize));
			fixedMetadataReg.typesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 6 * wordSize);
			fixedMetadataReg.types = reinterpret_cast<Il2CppType*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 7 * wordSize));
			fixedMetadataReg.methodSpecsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 8 * wordSize);
			fixedMetadataReg.methodSpecs = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 9 * wordSize));
			fixedMetadataReg.fieldOffsetsCount = *reinterpret_cast<int32_t*>(pMetadataReg + 10 * wordSize);
			fixedMetadataReg.fieldOffsets = reinterpret_cast<void**>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 11 * wordSize));
			fixedMetadataReg.typeDefinitionsSizesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 12 * wordSize);
			fixedMetadataReg.typeDefinitionsSizes = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 13 * wordSize));
			fixedMetadataReg.metadataUsagesCount = *reinterpret_cast<int32_t*>(pMetadataReg + 14 * wordSize);
			fixedMetadataReg.metadataUsages = reinterpret_cast<void*>(pLocalIl2cppBytes + *reinterpret_cast<int64_t*>(pMetadataReg + 15 * wordSize));
		}
	}

	return fixedMetadataReg;
}