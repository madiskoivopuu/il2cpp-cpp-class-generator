#include "MetadataProcessor-vXX-X.h"
#include ".././../class-generator/typeinfo.h"

#include "versions/metadata-v23-0.h"
#include "MetadataProcessorImpl.h"

std::vector<Il2cppImageData> ParseMetadata_v23_0(void* metadataBytes) {
	return ParseMetadata(metadataBytes);
}