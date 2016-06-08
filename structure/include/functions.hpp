#pragma once

#include "structure_export.h"

#include "type/Structure.hpp"
#include "type/Block.hpp"
#include "type/GenericField.hpp"
#include "type/Field.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>
#include <ostream>

namespace structure
{

// Structure

using BlockFunction = std::function<void(const Block &)>;
const BlockFunction defaultBlockFunction = [](const Block &) {};

using FieldFunction = std::function<void(const GenericField &)>;
const FieldFunction defaultFieldFunction = [](const GenericField &) {};

STRUCTURE_EXPORT void apply(const Structure &structure, BlockFunction onEnterBlock,
                            BlockFunction onExitBlock, FieldFunction onEnterField, bool recursive);

// Value

using BlockValueFunction = std::function<void(const BlockValue &)>;
const BlockValueFunction defaultBlockValueFunction = [](const BlockValue &) {};

using FieldValueFunction = std::function<void(const GenericFieldValue &)>;
const FieldValueFunction defaultFieldValueFunction = [](const GenericFieldValue &) {};

STRUCTURE_EXPORT void apply(const StructureValue &structure, BlockValueFunction onEnterBlock,
                            BlockValueFunction onExitBlock, FieldValueFunction onEnterField,
                            bool recursive);

// Functions

STRUCTURE_EXPORT void print(std::ostream &outStream, const Structure &structure);
STRUCTURE_EXPORT void print(std::ostream &outStream, const StructureValue &value);
STRUCTURE_EXPORT void print(std::ostream &outStream, const std::unique_ptr<StructureValue> &value);
STRUCTURE_EXPORT void print(std::ostream &outStream, const std::unique_ptr<Block> &structure);
template <typename T>
void print(std::ostream &outStream, const std::unique_ptr<Field<T>> &structure)
{
    print(*structure, outStream);
}

STRUCTURE_EXPORT std::string getValue(const StructureValue &value);
STRUCTURE_EXPORT std::string getValue(const std::unique_ptr<StructureValue> &value);

STRUCTURE_EXPORT Structure &getChild(const Structure &structure, std::string path);
STRUCTURE_EXPORT StructureValue &getChild(const StructureValue &value, std::string path);
STRUCTURE_EXPORT StructureValue &getChild(const std::unique_ptr<StructureValue> &value,
                                          std::string path);
STRUCTURE_EXPORT Structure &getChild(const std::unique_ptr<Block> &structure, std::string path);
template <typename T>
Structure &getChild(const std::unique_ptr<Field<T>> &structure, std::string path)
{
    return getChild(*structure, path);
}

STRUCTURE_EXPORT BlockValue with(const Block &block, ValueBuilder builder);
template <typename T>
FieldValue<T> with(const Field<T> &field, ValueBuilder builder)
{
    return field.with(builder);
}

template <class FieldType>
FieldValue<FieldType> with(FieldType fieldType, typename FieldType::Storage value)
{
    return {fieldType, value};
}

template <class FieldType>
FieldValue<FieldType> with(FieldType fieldType, const std::string &value)
{
    return {fieldType, value};
}
}
