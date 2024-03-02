#pragma once
#include "typeconversion.h"

#include <vector>

struct FieldData {
	char* name;
	bool _static;
	bool _const;
	Il2CppType* type;
	uintptr_t defaultValuePtr; // ptr to the default value in metadata, we will parse it later
	Il2CppType* defaultValueType;

	FieldData() : name(NULL), _static(false), _const(false), type(NULL), defaultValuePtr(NULL), defaultValueType(NULL) {}
};

struct MethodArgument {
	char* name;
	Il2CppType* type;
	bool passByRef;
	uintptr_t defaultValuePtr;
	Il2CppType* defaultValueType;

	MethodArgument() : name(NULL), type(NULL), passByRef(false), defaultValuePtr(NULL), defaultValueType(NULL) {}
};

struct MethodData {
	char* name;
	Il2CppType* returnType;
	bool returnByRef;
	std::vector<MethodArgument> arguments;

	MethodData() : name(NULL), returnType(NULL), returnByRef(false), arguments({}) {}
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