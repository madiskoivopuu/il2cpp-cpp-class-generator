#pragma once
#pragma once
#include "versions/metadata-basic.h"

// constant definitions that haven't changed for a while
struct Il2CppFieldDefaultValue
{
    FieldIndex fieldIndex;
    TypeIndex typeIndex;
    DefaultValueDataIndex dataIndex;
};

struct Il2CppFieldRef
{
    TypeIndex typeIndex;
    FieldIndex fieldIndex; // local offset into type fields
};

struct Il2CppMetadataUsageList
{
    uint32_t start;
    uint32_t count;
};

struct Il2CppMetadataUsagePair
{
    uint32_t destinationIndex;
    uint32_t encodedSourceIndex;
};

struct Il2CppParameterDefaultValue
{
    ParameterIndex parameterIndex;
    TypeIndex typeIndex;
    DefaultValueDataIndex dataIndex;
};

struct Il2CppStringLiteral
{
    uint32_t length;
    StringLiteralIndex dataIndex;
};

struct Il2CppInterfaceOffsetPair
{
    TypeIndex interfaceTypeIndex;
    int32_t offset;
};