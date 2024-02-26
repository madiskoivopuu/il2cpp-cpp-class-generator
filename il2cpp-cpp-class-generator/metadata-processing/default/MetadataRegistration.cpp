#include <vector>
#include <Windows.h>
#include <iostream>
#include <format>

#include "MetadataRegistration.h"

uint32_t CountMethods(Il2CppGlobalMetadataHeader* header) {
	uint32_t methods = 0;

	// start parsing all the il2cpp types
	Il2CppTypeDefinition* typeDefinitionStart = reinterpret_cast<Il2CppTypeDefinition*>((char*)header + header->typeDefinitionsOffset);
	uint32_t typesCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition);
	for (uint32_t typeNum = 0; typeNum < typesCount; typeNum++) {
		Il2CppTypeDefinition* typeDef = typeDefinitionStart + typeNum;

		// type must be class
		if (typeDef->methodStart != -1 && typeDef->method_count > 0)
			methods += typeDef->method_count;
	}

	return methods;
}

uint32_t CountTypeDefs(Il2CppGlobalMetadataHeader* header) {

}

// Does a deeper check to whether a potential registration pointer contains plausible values for a real metadata registration
bool CheckMetadataRegValidity(IFile* il2cppBinary, Il2CppMetadataRegistration* potential) {
	if (potential->fieldOffsetsCount < 0
		|| potential->genericClassesCount < 0
		|| potential->genericInstsCount < 0
		|| potential->genericMethodTableCount < 0
		|| potential->metadataUsagesCount < 0
		|| potential->methodSpecsCount < 0
		|| potential->typeDefinitionsSizesCount < 0
		|| potential->typesCount < 0) return false;

	int maxFileBounds = il2cppBinary->fileBytes.size();
	uintptr_t* end = reinterpret_cast<uintptr_t*>(potential + 1);
	for (uintptr_t* regPointer = reinterpret_cast<uintptr_t*>(potential)+1; regPointer < end; regPointer += 2) { // +2 skips over count fields
		uintptr_t virtualAddy = *regPointer;
		// somehow we can have the count and pointer set to zero
		if (virtualAddy == 0 && *(regPointer - 1) == 0) continue;
		
		int virtualAddyAsOffset = il2cppBinary->MapVAToOffset(virtualAddy);
		if(virtualAddyAsOffset == 0 || virtualAddyAsOffset >= maxFileBounds)
			return false;
	}

	return true;
}

// Pattern scans 1 specific int in memory and tries to narrow down the search results to only one
// NB! Virtual addresses (pointers in the struct) still have to be converted to the appropriate address
Il2CppMetadataRegistration PatternScanMetadataRegistration(IFile* il2cppBinary, Il2CppGlobalMetadataHeader* header) {
	std::vector<Il2CppMetadataRegistration*> potentialMetadataRegs = { };

	uint32_t totalMethods = CountMethods(header);
	uint32_t typeDefinitionsCount = header->typeDefinitionsCount / sizeof(Il2CppTypeDefinition);

	// calculate the types count & method specs count
	BYTE* start = il2cppBinary->fileBytes.data();
	BYTE* end = start + (il2cppBinary->fileBytes.size() - sizeof(Il2CppMetadataRegistration));

	for (; start <= end; start += sizeof(uintptr_t)) {
		Il2CppMetadataRegistration* potential = reinterpret_cast<Il2CppMetadataRegistration*>(start);

		if (potential->typeDefinitionsSizesCount != typeDefinitionsCount || potential->genericMethodTableCount > totalMethods)
			continue;
		if (header->version >= 27 && (potential->metadataUsages != nullptr || potential->metadataUsagesCount != 0))
			continue;

		// a few preliminary checks for finding the correct registration
		if (potential->genericMethodTableCount <= totalMethods && potential->typeDefinitionsSizesCount != typeDefinitionsCount)
			potentialMetadataRegs.push_back(potential);
	}

	std::vector<Il2CppMetadataRegistration*> betterRegCandidates = { };
	for (Il2CppMetadataRegistration* registration : potentialMetadataRegs)
		if (CheckMetadataRegValidity(il2cppBinary, registration))
			betterRegCandidates.push_back(registration);

	if (betterRegCandidates.size() != 1)
		throw std::exception("No candidates or over two metadata registration candidates were found after filtering!");

	return *betterRegCandidates.at(0);
}