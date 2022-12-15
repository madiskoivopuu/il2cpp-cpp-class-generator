#pragma once
#include "../../../class-generator/typeinfo.h"
#include "../../../il2cpp/il2cpp-tabledefs.h"
#include "../../../il2cpp/il2cpp-binarystructs.h"
#include "../metadata-registration/MetadataRegistration.h"
#include "../../../utils/filetypes/IFile.h"

#include <vector>
#include <unordered_map>

// Template macros
#define IL2CPP_TEMPLATES_V23_0 Il2CppGlobalMetadataHeader_v24_0, Il2CppImageDefinition_v24_0, Il2CppTypeDefinition_v24_0, Il2CppFieldDefinition_v24_0
#define IL2CPP_TEMPLATES_V24_0 Il2CppGlobalMetadataHeader_v24_0, Il2CppImageDefinition_v24_0, Il2CppTypeDefinition_v24_0, Il2CppFieldDefinition_v24_0
#define IL2CPP_TEMPLATES_V24_1 Il2CppGlobalMetadataHeader_v24_1, Il2CppImageDefinition_v24_1, Il2CppTypeDefinition_v24_1, Il2CppFieldDefinition_v24_1
#define IL2CPP_TEMPLATES_V24_15 Il2CppGlobalMetadataHeader_v24_15, Il2CppImageDefinition_v24_15, Il2CppTypeDefinition_v24_15, Il2CppFieldDefinition_v24_15
#define IL2CPP_TEMPLATES_V24_2 Il2CppGlobalMetadataHeader_v24_2, Il2CppImageDefinition_v24_2, Il2CppTypeDefinition_v24_2, Il2CppFieldDefinition_v24_2
#define IL2CPP_TEMPLATES_V24_3 Il2CppGlobalMetadataHeader_v24_3, Il2CppImageDefinition_v24_3, Il2CppTypeDefinition_v24_3, Il2CppFieldDefinition_v24_3
#define IL2CPP_TEMPLATES_V24_4 Il2CppGlobalMetadataHeader_v24_4, Il2CppImageDefinition_v24_4, Il2CppTypeDefinition_v24_4, Il2CppFieldDefinition_v24_4
#define IL2CPP_TEMPLATES_V24_5 Il2CppGlobalMetadataHeader_v24_5, Il2CppImageDefinition_v24_5, Il2CppTypeDefinition_v24_5, Il2CppFieldDefinition_v24_5
#define IL2CPP_TEMPLATES_V27_0 Il2CppGlobalMetadataHeader_v27_0, Il2CppImageDefinition_v27_0, Il2CppTypeDefinition_v27_0, Il2CppFieldDefinition_v27_0
#define IL2CPP_TEMPLATES_V27_1 Il2CppGlobalMetadataHeader_v27_1, Il2CppImageDefinition_v27_1, Il2CppTypeDefinition_v27_1, Il2CppFieldDefinition_v27_1
#define IL2CPP_TEMPLATES_V27_9 Il2CppGlobalMetadataHeader_v27_9, Il2CppImageDefinition_v27_9, Il2CppTypeDefinition_v27_9, Il2CppFieldDefinition_v27_9
#define IL2CPP_TEMPLATES_V29_0 Il2CppGlobalMetadataHeader_v29_0, Il2CppImageDefinition_v29_0, Il2CppTypeDefinition_v29_0, Il2CppFieldDefinition_v29_0

// Our own helper functions
std::unordered_map<char, char> invalidCharReplacements = {
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
template<typename THeader>
char* GetStringFromIndex(THeader* header, int index) {
	char* strings = ((char*)header + header->stringOffset) + index;
	return strings;
}

template<typename THeader, typename TImgDef>
TImgDef* GetImageFromIndex(THeader* header, int index) {
	return reinterpret_cast<TImgDef*>((char*)header + header->imagesOffset) + index;
}


template<typename THeader, typename TFieldDef>
TFieldDef* GetFieldInfoFromIndex(THeader* header, int index) {
	return reinterpret_cast<TFieldDef*>((char*)header + header->fieldsOffset) + index;
}

template<typename THeader, typename TPropDef>
TPropDef* GetPropInfoFromIndex(THeader* header, int index) {
	return reinterpret_cast<TPropDef*>((char*)header + header->propertiesOffset) + index;
}

template<typename THeader>
Il2CppFieldDefaultValue* GetFieldDefaultValueStruct(THeader* header, int index) {
	return reinterpret_cast<Il2CppFieldDefaultValue*>((char*)header + header->fieldDefaultValuesOffset) + index;
}

Il2CppType* GetTypeFromIndex(IFile* il2cppBinary, Il2CppMetadataRegistration metadataRegistration, int index) {
	uintptr_t actualTypesOffset = il2cppBinary->MapVAToOffset(reinterpret_cast<uintptr_t>(metadataRegistration.types));
	Il2CppType* typeLoc = reinterpret_cast<Il2CppType*>(il2cppBinary->fileBytes.data() + actualTypesOffset) + index;
	return typeLoc;
}

//
// Main metadata parsing function
//
template<typename THeader, typename TImgDef, typename TTypeDef, typename TFieldDef>
std::vector<Il2cppImageData> ParseMetadata(std::vector<BYTE>& metadataBytes, IFile* il2cppBinary, Il2CppMetadataRegistration metadataRegistration) {
	THeader* header = reinterpret_cast<THeader*>(metadataBytes.data());

	std::vector<Il2cppImageData> allImages;

	// parse all of the images so we can later split up classes into their own folder
	TImgDef* imageDefStart = reinterpret_cast<TImgDef*>((char*)header + header->imagesOffset);
	int imageCount = header->imagesCount / sizeof(TImgDef);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		TImgDef* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, image->nameIndex));

		// start parsing all the il2cpp types
		TTypeDef* typeDefinitionStart = reinterpret_cast<TTypeDef*>((char*)header + header->typeDefinitionsOffset);
		for (uint32_t typeNum = 0; typeNum < image->typeCount; typeNum++) {
			TTypeDef* typeDef = typeDefinitionStart + image->typeStart + typeNum;
			ClassData currClass;
			currClass._namespace = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, typeDef->namespaceIndex));
			currClass.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, typeDef->nameIndex));

			std::cout << currClass._namespace << " " << currClass.name << std::endl;

			// handle type based on what it is
			if ((typeDef->bitfield >> 1) & 0x1) { // 2nd bit is 1 = enum
				currClass.type = ClassType::ENUM;
				for (int i = typeDef->fieldStart; i < typeDef->fieldStart + typeDef->field_count; i++) {
					FieldData fieldData;
					TFieldDef* field = GetFieldInfoFromIndex<THeader, TFieldDef>(header, i);
					Il2CppType* fieldType = GetTypeFromIndex(il2cppBinary, metadataRegistration, field->typeIndex);

					fieldData.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, field->nameIndex));
					fieldData.type = fieldType->type;

					Il2CppFieldDefaultValue* defaultVal = GetFieldDefaultValueStruct<THeader>(header, i);
					if (fieldType->attrs & FIELD_ATTRIBUTE_LITERAL && defaultVal->dataIndex != -1) { // literal = const value
						fieldData.defaultValue = reinterpret_cast<uintptr_t>( reinterpret_cast<BYTE*>((char*)header + header->fieldAndParameterDefaultValueDataOffset) + defaultVal->dataIndex);
					}

					currClass.fields.push_back(fieldData);
				}
			}
			else if (typeDef->bitfield & 0x1) { // 1nd bit is 1 = struct
				currClass.type = ClassType::STRUCT;
			}
			else {
				currClass.type = ClassType::CLASS;
			}
		}


		allImages.push_back(imgData);
	}

	return allImages;
}