#pragma once

// This is a different way from using templates to make the ParseMetadata function compatible with different metadata versions
// Basically, you first include the metadata header for the specific version and after that you include this implementation header

#include <vector>
#include <fstream>
#include <iostream>

inline char* GetStringFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	char* strings = ((char*)header + header->stringOffset) + index;
	return strings;
}

inline Il2CppImageDefinition* GetImageFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppImageDefinition*>(header + header->imagesOffset) + index;
}

inline Il2CppFieldDefinition* GetFieldInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppFieldDefinition*>(header + header->fieldsOffset) + index;
}

inline Il2CppPropertyDefinition* GetPropInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppPropertyDefinition*>(header + header->propertiesOffset) + index;
}

inline std::vector<Il2cppImageData> ParseMetadata(void* metadataBytes) {
	Il2CppGlobalMetadataHeader* header = static_cast<Il2CppGlobalMetadataHeader*>(metadataBytes);

	std::vector<Il2cppImageData> allImages;

	// parse classes, their fields props methods etc from images
	Il2CppImageDefinition* imageDefStart = (Il2CppImageDefinition*)((const char*)header + header->assembliesOffset);
	int imageCount = header->imagesCount / sizeof(Il2CppImageDefinition);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		Il2CppImageDefinition* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = GetStringFromIndex(header, image->nameIndex);
		std::cout << imgData.name << std::endl;

		allImages.push_back(imgData);
	}

	return allImages;
}