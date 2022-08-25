#pragma once
#include "../../../class-generator/typeinfo.h"
#include "il2cpp-tabledefs.h"

#include <vector>
#include <unordered_map>

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

std::unordered_map<char, char> invalidCharReplacements = {
	{'.', '_'},
	{'<', ' '},
	{'>', ' '}
};
char* ReplaceInvalidCharacters(char* string) {
	for (int i = 0; i < strlen(string); i++) {
		if (invalidCharReplacements.count(string[i])) { // invalid char found
			string[i] = invalidCharReplacements[string[i]];
		}
	}
	return string;
}

template<typename THeader, typename TImgDef, typename TTypeDef>
std::vector<Il2cppImageData> ParseMetadata(void* metadataBytes) {
	THeader* header = static_cast<THeader*>(metadataBytes);

	std::vector<Il2cppImageData> allImages;

	// parse all of the images so we can later split up classes into their own folder
	TImgDef* imageDefStart = reinterpret_cast<TImgDef*>((char*)header + header->imagesOffset);
	int imageCount = header->imagesCount / sizeof(TImgDef);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		TImgDef* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, image->nameIndex));

		// start parsing classes
		TTypeDef* typeDefinitionStart = reinterpret_cast<TTypeDef*>((char*)header + header->typeDefinitionsOffset);
		for (uint32_t typeNum = 0; typeNum < image->typeCount; typeNum++) {
			TTypeDef* type = typeDefinitionStart + image->typeStart + typeNum;
			ClassData currClass;

			// don't store interfaces

			currClass._namespace = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, type->namespaceIndex));
			currClass.name = ReplaceInvalidCharacters(GetStringFromIndex<THeader>(header, type->nameIndex));
		}


		allImages.push_back(imgData);
	}

	return allImages;
}