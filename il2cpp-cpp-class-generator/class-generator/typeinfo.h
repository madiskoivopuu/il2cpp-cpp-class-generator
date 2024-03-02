#pragma once
#include "typeconversion.h"

#include <vector>

struct Il2CppTypeDefinition; // forward decl

struct FieldData {
	char* name;
	bool _static;
	bool _const;
	Il2CppType* type;
	Il2CppTypeDefinition* typeDef;
	uintptr_t defaultValuePtr; // ptr to the default value in metadata, we will parse it later
	Il2CppType* defaultValueType;
	Il2CppTypeDefinition* defaultValueTypeDef;

	FieldData() : name(NULL), _static(false), _const(false), type(NULL), typeDef(NULL), defaultValuePtr(NULL), defaultValueType(NULL), defaultValueTypeDef(NULL) {}
};

struct MethodArgument {
	char* name;
	Il2CppType* type;
	Il2CppTypeDefinition* typeDef;
	bool passByRef;
	uintptr_t defaultValuePtr;
	Il2CppType* defaultValueType;
	Il2CppTypeDefinition* defaultValueTypeDef;

	MethodArgument() : name(NULL), type(NULL), typeDef(NULL), passByRef(false), defaultValuePtr(NULL), defaultValueType(NULL), defaultValueTypeDef(NULL) {}
};

struct MethodData {
	char* name;
	Il2CppType* returnType;
	Il2CppTypeDefinition* returnTypeDef;
	bool returnByRef;
	std::vector<MethodArgument> arguments;

	MethodData() : name(NULL), returnType(NULL), returnTypeDef(NULL), returnByRef(false), arguments({}) {}
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