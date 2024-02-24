#pragma once
#include "../../class-generator/typeinfo.h"
#include "../../il2cpp/il2cpp-tabledefs.h"
#include "../../il2cpp/il2cpp-binarystructs.h"
#include "../../utils/filetypes/IFile.h"

#include "metadata.h"
#include "MetadataHelpers.h"
#include "MetadataRegistration.h"

#include <vector>
#include <unordered_map>
Il2CppFieldDefaultValue* GetFieldDefaultValueStruct(Il2CppGlobalMetadataHeader* header, int fieldIndex) {
	Il2CppFieldDefaultValue* first = reinterpret_cast<Il2CppFieldDefaultValue*>((char*)header + header->fieldDefaultValuesOffset);
	for (Il2CppFieldDefaultValue* value = first; value < first + header->fieldDefaultValuesCount; value++) { // TODO: cache these values beforehand so we dont have to loop every single time
		if (value->fieldIndex == fieldIndex) return value;
	}
	return nullptr;
}

Il2CppType* GetTypeFromIndex(IFile* il2cppBinary, Il2CppMetadataRegistration metadataRegistration, int index) {
	uintptr_t typesOffsetFromFileStart = il2cppBinary->MapVAToOffset(reinterpret_cast<uintptr_t>(metadataRegistration.types));
	uintptr_t typeVirtualAddrPtr = *(reinterpret_cast<uintptr_t*>(il2cppBinary->fileBytes.data() + typesOffsetFromFileStart) + index);
	return reinterpret_cast<Il2CppType*>(il2cppBinary->fileBytes.data() + il2cppBinary->MapVAToOffset(typeVirtualAddrPtr));
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
				for (int fieldIndex = typeDef->fieldStart; fieldIndex < typeDef->fieldStart + typeDef->field_count; fieldIndex++) {
					FieldData fieldData;
					TFieldDef* field = GetFieldInfoFromIndex<THeader, TFieldDef>(header, fieldIndex);
					Il2CppType* fieldType = GetTypeFromIndex(il2cppBinary, metadataRegistration, field->typeIndex);

					Il2CppFieldDefaultValue* defaultVal = GetFieldDefaultValueStruct<THeader>(header, fieldIndex);
					if (!(fieldType->attrs & FIELD_ATTRIBUTE_LITERAL && defaultVal->dataIndex != -1)) continue; // check if value is const

					fieldData.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, field->nameIndex));
					fieldData.type = fieldType->type;
					fieldData.defaultValue = reinterpret_cast<uintptr_t>(reinterpret_cast<BYTE*>((char*)header + header->fieldAndParameterDefaultValueDataOffset) + defaultVal->dataIndex);
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