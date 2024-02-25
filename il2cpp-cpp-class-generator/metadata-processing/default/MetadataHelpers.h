#pragma once

#include "metadata.h"
#include "MetadataRegistration.h"
#include <vector>
#include <unordered_map>

// Our own helper functions
static std::unordered_map<char, char> invalidCharReplacements = {
	{'.', '_'},
	{'<', '_'},
	{'>', '_'},
	{'\n', '_'}
};
char* ReplaceInvalidCharacters(char* string) {
	for (int i = 0; i < strlen(string); i++) {
		if (invalidCharReplacements.count(string[i])) { // invalid char found
			string[i] = invalidCharReplacements[string[i]];
		}
	}
	return string;
}

// Function declarations for different metadata versions
char* GetStringFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	char* strings = ((char*)header + header->stringOffset) + index;
	return strings;
}

Il2CppImageDefinition* GetImageFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppImageDefinition*>((char*)header + header->imagesOffset) + index;
}

Il2CppMethodDefinition* GetMethodInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppMethodDefinition*>((char*)header + header->methodsOffset) + index;
}

Il2CppFieldDefinition* GetFieldInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppFieldDefinition*>((char*)header + header->fieldsOffset) + index;
}

Il2CppPropertyDefinition* GetPropInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppPropertyDefinition*>((char*)header + header->propertiesOffset) + index;
}

Il2CppParameterDefinition* GetParamInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppParameterDefinition*>((char*)header + header->parametersOffset) + index;
}

Il2CppFieldDefaultValue* GetFieldDefaultValueStruct(Il2CppGlobalMetadataHeader* header, int fieldIndex) {
	Il2CppFieldDefaultValue* first = reinterpret_cast<Il2CppFieldDefaultValue*>((char*)header + header->fieldDefaultValuesOffset);
	for (Il2CppFieldDefaultValue* value = first; value < first + header->fieldDefaultValuesCount; value++) { // TODO: cache these values beforehand so we dont have to loop every single time
		if (value->fieldIndex == fieldIndex) 
			return value;
	}
	return nullptr;
}

Il2CppParameterDefaultValue* GetParamDefaultValue(Il2CppGlobalMetadataHeader* header, int paramIndex) {
	for (int defValueIdx = 0; defValueIdx < header->parameterDefaultValuesCount; defValueIdx++) {
		Il2CppParameterDefaultValue* defValue = reinterpret_cast<Il2CppParameterDefaultValue*>((char*)header + header->parameterDefaultValuesOffset) + defValueIdx;
		if (defValue->parameterIndex == paramIndex)
			return defValue;
	}

	return nullptr;
}

Il2CppType* GetTypeFromIndex(IFile* il2cppBinary, Il2CppMetadataRegistration* metadataRegistration, int index) {
	uintptr_t typesOffsetFromFileStart = il2cppBinary->MapVAToOffset(reinterpret_cast<uintptr_t>(metadataRegistration->types));
	uintptr_t typeVirtualAddrPtr = *(reinterpret_cast<uintptr_t*>(il2cppBinary->fileBytes.data() + typesOffsetFromFileStart) + index);
	return reinterpret_cast<Il2CppType*>(il2cppBinary->fileBytes.data() + il2cppBinary->MapVAToOffset(typeVirtualAddrPtr));
}