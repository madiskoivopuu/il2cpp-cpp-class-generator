#include "MetadataProcessor-vXX-X.h"
#include ".././../class-generator/typeinfo.h"

#include "versions/metadata-v24-5.h"
#include "MetadataProcessorImpl.h"

std::vector<Il2cppImageData> ParseMetadata_v24_5(void* metadataBytes) {
	return ParseMetadata(metadataBytes);
}