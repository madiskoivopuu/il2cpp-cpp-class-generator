#pragma once

#include <vector>

struct FieldData {
	char* name;

};
struct PropData {
	char* name;
};

struct MethodData {
	char* name;
};

struct ClassData {
	char* _namespace;
	char* name;

	std::vector<FieldData> fields;
	std::vector<PropData> properties;
	std::vector<MethodData> methods;
};

struct Il2cppImageData {
	char* name;
	std::vector<ClassData> classes;
};