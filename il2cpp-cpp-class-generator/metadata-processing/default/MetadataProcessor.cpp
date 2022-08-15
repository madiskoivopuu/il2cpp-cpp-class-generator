#include "MetadataProcessor.h"
#include "il2cpp-metadata.h"

#include <fstream>
#include <assert.h>
#include <iostream>

void* LoadMetadataFile(char* filePath) {
	std::ifstream file(filePath, std::ios_base::binary);

	// get the length of file to malloc a buffer
	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);

	void* buffer = malloc(length);
	file.read(static_cast<char*>(buffer), length);

	if (!file) {
		std::cout << "error: only " << file.gcount() << " could be read";
		free(buffer);
		return 0;
	}

	return buffer;
}

char* GetStringFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	char* strings = ((char*)header + header->stringOffset) + index;
	return strings;
}

Il2CppImageDefinition* GetImageFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppImageDefinition*>(header + header->imagesOffset) + index;
}

Il2CppFieldDefinition* GetFieldInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppFieldDefinition*>(header + header->fieldsOffset) + index;
}

Il2CppPropertyDefinition* GetPropInfoFromIndex(Il2CppGlobalMetadataHeader* header, int index) {
	return reinterpret_cast<Il2CppPropertyDefinition*>(header + header->propertiesOffset) + index;
}

std::vector<Il2cppImageData> ParseMetadata(void* metadataBytes) {
	Il2CppGlobalMetadataHeader* header = static_cast<Il2CppGlobalMetadataHeader*>(metadataBytes);
	assert(header->sanity == 0xFAB11BAF);
	
	std::vector<Il2cppImageData> allAssemblies;

	// parse classes, their fields props methods etc from images
	Il2CppImageDefinition* imageDefStart = (Il2CppImageDefinition*)((const char*)header + header->assembliesOffset);
	int imageCount = header->imagesCount / sizeof(Il2CppImageDefinition);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		Il2CppImageDefinition* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = GetStringFromIndex(header, image->nameIndex);
		std::cout << imgData.name << std::endl;

		allAssemblies.push_back(imgData);
	}

	return allAssemblies;
}