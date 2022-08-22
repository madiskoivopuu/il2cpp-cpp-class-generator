#pragma once
#include "../../class-generator/typeinfo.h"

#include <vector>

// Function declarations for different metadata versions
template<typename THeader>
char* GetStringFromIndex(THeader* header, int index) {
	char* strings = ((char*)header + header->stringOffset) + index;
	return strings;
}

template<typename THeader, typename TImgDef>
TImgDef* GetImageFromIndex(THeader* header, int index) {
	return reinterpret_cast<TImgDef*>(header + header->imagesOffset) + index;
}


template<typename THeader, typename TFieldDef>
TFieldDef* GetFieldInfoFromIndex(THeader* header, int index) {
	return reinterpret_cast<TFieldDef*>(header + header->fieldsOffset) + index;
}

template<typename THeader, typename TPropDef>
TPropDef* GetPropInfoFromIndex(THeader* header, int index) {
	return reinterpret_cast<TPropDef*>(header + header->propertiesOffset) + index;
}

template<typename THeader, typename TImgDef>
std::vector<Il2cppImageData> ParseMetadata(void* metadataBytes) {
	THeader* header = static_cast<THeader*>(metadataBytes);

	std::vector<Il2cppImageData> allImages;

	// parse classes, their fields props methods etc from images
	TImgDef* imageDefStart = (TImgDef*)((const char*)header + header->imagesOffset);
	int imageCount = header->imagesCount / sizeof(TImgDef);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		TImgDef* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = GetStringFromIndex(header, image->nameIndex);
		std::cout << imgData.name << std::endl;

		allImages.push_back(imgData);
	}

	return allImages;
}