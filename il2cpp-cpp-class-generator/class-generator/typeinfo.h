#pragma once
#include "typeconversion.h"

#include <vector>

struct FieldData {
	char* name;
	bool _static;
	bool _const;
	Il2CppTypeEnum type;
	uintptr_t defaultValuePtr; // ptr to the default value in metadata, we will parse it later
	Il2CppTypeEnum defaultValueType;

	FieldData() : name(NULL), _static(false), _const(false), type(IL2CPP_TYPE_END), defaultValuePtr(NULL), defaultValueType(IL2CPP_TYPE_END) {}
};

struct MethodArgument {
	char* name;
	Il2CppTypeEnum type;
	bool passByRef;
	uintptr_t defaultValuePtr;
	Il2CppTypeEnum defaultValueType;

	MethodArgument() : name(NULL), type(IL2CPP_TYPE_END), passByRef(false), defaultValuePtr(NULL), defaultValueType(IL2CPP_TYPE_END) {}
};

struct MethodData {
	char* name;
	Il2CppTypeEnum returnType;
	bool returnByRef;
	std::vector<MethodArgument> arguments;

	MethodData() : name(NULL), returnType(IL2CPP_TYPE_END), returnByRef(false), arguments({}) {}
};


enum class ClassType {
	UNKNOWN = 0,
	ENUM,
	STRUCT,
	CLASS
};
struct TypeData {
	char* _namespace;
	char* name;
	ClassType type;

	std::vector<FieldData> fields;
	std::vector<MethodData> methods;

	TypeData() : _namespace(NULL), name(NULL), type(ClassType::UNKNOWN), fields({}), methods({}) {}
};


struct Il2cppImageData {
	char* name;
	std::vector<TypeData> definedTypes;

	Il2cppImageData() : definedTypes({}), name(NULL) {}
};