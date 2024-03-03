#pragma once
#include "MetadataProcessor.h"
#include "MetadataHelpers.h"

#include <vector>
#include <unordered_map>

struct MetadataState {
	Il2CppGlobalMetadataHeader* header;
	IFile* il2cppBinary;
	Il2CppMetadataRegistration* metadataRegistration;
};

std::vector<FieldData> ParseFieldsForType(MetadataState& state, Il2CppTypeDefinition* typeDef) {
	std::vector<FieldData> fields = {};

	for (int fieldIndex = typeDef->fieldStart; fieldIndex < typeDef->fieldStart + typeDef->field_count; fieldIndex++) {
		FieldData fieldData;
		Il2CppFieldDefinition* field = GetFieldInfoFromIndex(state.header, fieldIndex);
		Il2CppType* fieldType = GetTypeFromIndex(state.il2cppBinary, state.metadataRegistration, field->typeIndex);

		if (fieldType->attrs & FIELD_ATTRIBUTE_LITERAL) fieldData._const = true;
		if (fieldType->attrs & FIELD_ATTRIBUTE_STATIC) fieldData._static = true;

		fieldData.name = ReplaceInvalidCharacters(GetStringFromIndex(state.header, field->nameIndex));
		fieldData.type = fieldType;
		fieldData.typeDef = GetUnderlyingTypeDefFromType(state.header, state.il2cppBinary, state.metadataRegistration, fieldType);

		Il2CppFieldDefaultValue* defaultVal = GetFieldDefaultValueStruct(state.header, fieldIndex);
		// somehow there's very large values in defaultVal->typeIndex sometimes, and negative ones too????
		if (defaultVal && defaultVal->typeIndex >= 0 && defaultVal->typeIndex < state.metadataRegistration->typesCount) {
			Il2CppType* defaultValueType = GetTypeFromIndex(state.il2cppBinary, state.metadataRegistration, defaultVal->typeIndex);
			fieldData.defaultValuePtr = reinterpret_cast<uintptr_t>(reinterpret_cast<BYTE*>((char*)state.header + state.header->fieldAndParameterDefaultValueDataOffset) + defaultVal->dataIndex);
			fieldData.defaultValueType = defaultValueType;
			fieldData.defaultValueTypeDef = GetUnderlyingTypeDefFromType(state.header, state.il2cppBinary, state.metadataRegistration, defaultValueType);
		}
		fields.push_back(fieldData);
	}

	return fields;
}

std::vector<MethodArgument> ParseMethodArguments(MetadataState& state, Il2CppMethodDefinition* methodDef) {
	std::vector<MethodArgument> args = {};

	for (int argIndex = methodDef->parameterStart; argIndex < methodDef->parameterStart + methodDef->parameterCount; argIndex++) {
		MethodArgument argData;
		Il2CppParameterDefinition* paramDef = GetParamInfoFromIndex(state.header, argIndex);
		argData.name = ReplaceInvalidCharacters(GetStringFromIndex(state.header, paramDef->nameIndex));

		Il2CppType* argType = GetTypeFromIndex(state.il2cppBinary, state.metadataRegistration, paramDef->typeIndex);
		argData.type = argType;
		argData.typeDef = GetUnderlyingTypeDefFromType(state.header, state.il2cppBinary, state.metadataRegistration, argType);
		argData.passByRef = argType->byref;

		Il2CppParameterDefaultValue* paramDefaultValue = GetParamDefaultValue(state.header, argIndex);
		if (paramDefaultValue) {
			Il2CppType* defaultValueType = GetTypeFromIndex(state.il2cppBinary, state.metadataRegistration, paramDefaultValue->typeIndex);
			argData.defaultValuePtr = reinterpret_cast<uintptr_t>(reinterpret_cast<BYTE*>((char*)state.header + state.header->fieldAndParameterDefaultValueDataOffset) + paramDefaultValue->dataIndex);
			argData.defaultValueType = defaultValueType;
			argData.defaultValueTypeDef = GetUnderlyingTypeDefFromType(state.header, state.il2cppBinary, state.metadataRegistration, defaultValueType);
		}

		args.push_back(argData);
	}

	return args;
}

std::vector<MethodData> ParseMethodsForClass(MetadataState& state, Il2CppTypeDefinition* typeDef) {
	std::vector<MethodData> methods = {};

	for (int methodIndex = typeDef->methodStart; methodIndex < typeDef->methodStart + typeDef->method_count; methodIndex++) {
		MethodData methodData;
		Il2CppMethodDefinition* methodDef = GetMethodInfoFromIndex(state.header, methodIndex);
		Il2CppType* methodReturnType = GetTypeFromIndex(state.il2cppBinary, state.metadataRegistration, methodDef->returnType);

		methodData.name = ReplaceInvalidCharacters(GetStringFromIndex(state.header, methodDef->nameIndex));
		methodData.returnType = methodReturnType;
		methodData.returnTypeDef = GetUnderlyingTypeDefFromType(state.header, state.il2cppBinary, state.metadataRegistration, methodReturnType);
		methodData.returnByRef = methodReturnType->byref;
		methodData.arguments = ParseMethodArguments(state, methodDef);

		methods.push_back(methodData);
	}

	return methods;
}

TypeData ParseType(MetadataState& state, Il2CppTypeDefinition* typeDef) {
	TypeData currType;
	currType._namespace = ReplaceInvalidCharacters(GetStringFromIndex(state.header, typeDef->namespaceIndex));
	currType.name = ReplaceInvalidCharacters(GetStringFromIndex(state.header, typeDef->nameIndex));
	currType.fields = ParseFieldsForType(state, typeDef);

	// handle type based on what it is
	if (typeDef->bitfield & 0b10) // 2nd bit is 1 = enum
		currType.type = ClassType::ENUM;
		// idk but enums shouldn't have methods
	else {
		currType.type = ClassType::CLASS;
		currType.methods = ParseMethodsForClass(state, typeDef);
	}

	return currType;
}

//
// Main metadata parsing function
//
std::vector<Il2cppImageData> ParseMetadata(std::vector<BYTE>& metadataBytes, IFile* il2cppBinary, Il2CppMetadataRegistration metadataRegistration) {
	Il2CppGlobalMetadataHeader* header = reinterpret_cast<Il2CppGlobalMetadataHeader*>(metadataBytes.data());

	MetadataState state = { header, il2cppBinary, &metadataRegistration };

	std::vector<Il2cppImageData> allImages;

	// parse all of the images so we can later split up classes into their own folder
	Il2CppImageDefinition* imageDefStart = reinterpret_cast<Il2CppImageDefinition*>((char*)header + header->imagesOffset);
	int imageCount = header->imagesCount / sizeof(Il2CppImageDefinition);
	for (int imgIdx = 0; imgIdx < imageCount; imgIdx++) {
		Il2CppImageDefinition* image = imageDefStart + imgIdx;

		Il2cppImageData imgData;
		imgData.name = ReplaceInvalidCharacters(GetStringFromIndex(header, image->nameIndex));

		// start parsing all the il2cpp types
		Il2CppTypeDefinition* typeDefinitionStart = reinterpret_cast<Il2CppTypeDefinition*>((char*)header + header->typeDefinitionsOffset);
		for (uint32_t typeNum = 0; typeNum < image->typeCount; typeNum++) {
			Il2CppTypeDefinition* typeDef = typeDefinitionStart + image->typeStart + typeNum;
			imgData.definedTypes.push_back(ParseType(state, typeDef));
		}

		allImages.push_back(imgData);
	}

	return allImages;
}