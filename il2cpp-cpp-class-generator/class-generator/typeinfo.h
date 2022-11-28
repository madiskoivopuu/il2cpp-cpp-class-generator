#pragma once
#include "typeconversion.h"

#include <vector>

struct FieldData {
	char* name = nullptr;
	bool _static = false;
	Il2CppTypeEnum type = Il2CppTypeEnum::IL2CPP_TYPE_END;
	uintptr_t defaultValue = 0; // could be char*, uint8_t, int etc.
};


struct PropData {
	char* name = nullptr;
};


struct MethodArgument {
	char* name = nullptr;
	Il2CppTypeEnum type = Il2CppTypeEnum::IL2CPP_TYPE_END;
	bool passByRef = false;
};

struct MethodData {
	char* name = nullptr;
	Il2CppTypeEnum returnType = Il2CppTypeEnum::IL2CPP_TYPE_END;
	std::vector<MethodArgument> arguments;
};


enum class ClassType {
	UNKNOWN = 0,
	ENUM,
	STRUCT,
	CLASS
};
struct ClassData {
	char* _namespace = nullptr;
	char* name = nullptr;
	ClassType type;

	std::vector<FieldData> fields;
	std::vector<PropData> properties;
	std::vector<MethodData> methods;
};


struct Il2cppImageData {
	char* name;
	std::vector<ClassData> classes;
};