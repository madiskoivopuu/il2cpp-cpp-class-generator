#include "MetadataProcessor-vXX-X.h"
#include ".././../class-generator/typeinfo.h"

#include "versions/metadata-v24-1.h"
#include "MetadataProcessorImpl.h"

std::vector<Il2cppImageData> ParseMetadata_v24_1(void* metadataBytes) {
	return ParseMetadata(metadataBytes);
}