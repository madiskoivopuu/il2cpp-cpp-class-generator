#pragma once
#include "metadata-const.h"

#define METADATA_COMPILED_VERSION "27.0"

struct Il2CppImage;
struct Il2CppType;
struct Il2CppTypeDefinitionMetadata;

struct Il2CppTypeDefinition
{
    StringIndex nameIndex;
    StringIndex namespaceIndex;
    TypeIndex byvalTypeIndex;

    TypeIndex declaringTypeIndex;
    TypeIndex parentIndex;
    TypeIndex elementTypeIndex; // we can probably remove this one. Only used for enums

    GenericContainerIndex genericContainerIndex;

    uint32_t flags;

    FieldIndex fieldStart;
    MethodIndex methodStart;
    EventIndex eventStart;
    PropertyIndex propertyStart;
    NestedTypeIndex nestedTypesStart;
    InterfacesIndex interfacesStart;
    VTableIndex vtableStart;
    InterfacesIndex interfaceOffsetsStart;

    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;

    // bitfield to portably encode boolean values as single bits
    // 01 - valuetype;
    // 02 - enumtype;
    // 03 - has_finalize;
    // 04 - has_cctor;
    // 05 - is_blittable;
    // 06 - is_import_or_windows_runtime;
    // 07-10 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128)
    uint32_t bitfield;
    uint32_t token;
};

struct Il2CppFieldDefinition
{
    StringIndex nameIndex;
    TypeIndex typeIndex;
    uint32_t token;
};

struct Il2CppParameterDefinition
{
    StringIndex nameIndex;
    uint32_t token;
    TypeIndex typeIndex;
};

struct Il2CppMethodDefinition
{
    StringIndex nameIndex;
    TypeDefinitionIndex declaringType;
    TypeIndex returnType;
    ParameterIndex parameterStart;
    GenericContainerIndex genericContainerIndex;
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint16_t parameterCount;
};

struct Il2CppEventDefinition
{
    StringIndex nameIndex;
    TypeIndex typeIndex;
    MethodIndex add;
    MethodIndex remove;
    MethodIndex raise;
    uint32_t token;
};

struct Il2CppPropertyDefinition
{
    StringIndex nameIndex;
    MethodIndex get;
    MethodIndex set;
    uint32_t attrs;
    uint32_t token;
};

struct Il2CppAssemblyNameDefinition
{
    StringIndex nameIndex;
    StringIndex cultureIndex;
    StringIndex publicKeyIndex;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[PUBLIC_KEY_BYTE_LENGTH];
};

struct Il2CppImageDefinition
{
    StringIndex nameIndex;
    AssemblyIndex assemblyIndex;

    TypeDefinitionIndex typeStart;
    uint32_t typeCount;

    TypeDefinitionIndex exportedTypeStart;
    uint32_t exportedTypeCount;

    MethodIndex entryPointIndex;
    uint32_t token;

    int32_t customAttributeStart;
    uint32_t customAttributeCount;
};

struct Il2CppAssemblyDefinition
{
    ImageIndex imageIndex;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyNameDefinition aname;
};

struct Il2CppCustomAttributeTypeRange
{
    uint32_t token;
    int32_t start;
    int32_t count;
};

struct Il2CppGlobalMetadataHeader
{
    int32_t sanity;
    int32_t version;
    int32_t stringLiteralOffset; // string data for managed code
    int32_t stringLiteralCount;
    int32_t stringLiteralDataOffset;
    int32_t stringLiteralDataCount;
    int32_t stringOffset; // string data for metadata
    int32_t stringCount;
    int32_t eventsOffset; // Il2CppEventDefinition
    int32_t eventsCount;
    int32_t propertiesOffset; // Il2CppPropertyDefinition
    int32_t propertiesCount;
    int32_t methodsOffset; // Il2CppMethodDefinition
    int32_t methodsCount;
    int32_t parameterDefaultValuesOffset; // Il2CppParameterDefaultValue
    int32_t parameterDefaultValuesCount;
    int32_t fieldDefaultValuesOffset; // Il2CppFieldDefaultValue
    int32_t fieldDefaultValuesCount;
    int32_t fieldAndParameterDefaultValueDataOffset; // uint8_t
    int32_t fieldAndParameterDefaultValueDataCount;
    int32_t fieldMarshaledSizesOffset; // Il2CppFieldMarshaledSize
    int32_t fieldMarshaledSizesCount;
    int32_t parametersOffset; // Il2CppParameterDefinition
    int32_t parametersCount;
    int32_t fieldsOffset; // Il2CppFieldDefinition
    int32_t fieldsCount;
    int32_t genericParametersOffset; // Il2CppGenericParameter
    int32_t genericParametersCount;
    int32_t genericParameterConstraintsOffset; // TypeIndex
    int32_t genericParameterConstraintsCount;
    int32_t genericContainersOffset; // Il2CppGenericContainer
    int32_t genericContainersCount;
    int32_t nestedTypesOffset; // TypeDefinitionIndex
    int32_t nestedTypesCount;
    int32_t interfacesOffset; // TypeIndex
    int32_t interfacesCount;
    int32_t vtableMethodsOffset; // EncodedMethodIndex
    int32_t vtableMethodsCount;
    int32_t interfaceOffsetsOffset; // Il2CppInterfaceOffsetPair
    int32_t interfaceOffsetsCount;
    int32_t typeDefinitionsOffset; // Il2CppTypeDefinition
    int32_t typeDefinitionsCount;
    int32_t imagesOffset; // Il2CppImageDefinition
    int32_t imagesCount;
    int32_t assembliesOffset; // Il2CppAssemblyDefinition
    int32_t assembliesCount;
    int32_t fieldRefsOffset; // Il2CppFieldRef
    int32_t fieldRefsCount;
    int32_t referencedAssembliesOffset; // int32_t
    int32_t referencedAssembliesCount;
    int32_t attributesInfoOffset; // Il2CppCustomAttributeTypeRange
    int32_t attributesInfoCount;
    int32_t attributeTypesOffset; // TypeIndex
    int32_t attributeTypesCount;
    int32_t unresolvedVirtualCallParameterTypesOffset; // TypeIndex
    int32_t unresolvedVirtualCallParameterTypesCount;
    int32_t unresolvedVirtualCallParameterRangesOffset; // Il2CppRange
    int32_t unresolvedVirtualCallParameterRangesCount;
    int32_t windowsRuntimeTypeNamesOffset; // Il2CppWindowsRuntimeTypeNamePair
    int32_t windowsRuntimeTypeNamesSize;
    const char* windowsRuntimeStringsOffset;
    int32_t windowsRuntimeStringsSize;
    int32_t exportedTypeDefinitionsOffset; // TypeDefinitionIndex
    int32_t exportedTypeDefinitionsCount;
};