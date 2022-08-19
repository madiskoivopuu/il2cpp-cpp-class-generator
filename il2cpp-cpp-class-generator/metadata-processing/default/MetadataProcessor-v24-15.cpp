#include "MetadataProcessor-vXX-X.h"
#include ".././../class-generator/typeinfo.h"

#include "versions/metadata-v24-15.h"
#include "MetadataProcessorImpl.h"

// v24_15 => 24.1 < 24.15 < 24.2
std::vector<Il2cppImageData> ParseMetadata_v24_15(void* metadataBytes) {
	return ParseMetadata(metadataBytes);
}