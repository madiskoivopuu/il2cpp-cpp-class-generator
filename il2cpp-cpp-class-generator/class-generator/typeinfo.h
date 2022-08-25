#pragma once
#include "typeconversion.h"

#include <vector>

struct FieldData {
	char* name;
	bool _static;
};


struct PropData {
	char* name;
};


struct MethodArgument {
	char* name;
	Il2CppTypeEnum argType;
	bool passByRef;
};

struct MethodData {
	char* name;
	Il2CppTypeEnum returnType;
	std::vector<MethodArgument> arguments;
};


enum class ClassType {
	ENUM = 0,
	STRUCT,
	CLASS
};
struct ClassData {
	char* _namespace;
	char* name;
	ClassType type;

	std::vector<FieldData> fields;
	std::vector<PropData> properties;
	std::vector<MethodData> methods;
};


struct Il2cppImageData {
	char* name;
	std::vector<ClassData> classes;
};