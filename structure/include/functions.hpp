#pragma once

#include "structure_export.h"

#include "Structure.hpp"
#include "Block.hpp"
#include "GenericField.hpp"
#include "Field.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>

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

STRUCTURE_EXPORT void display(const Structure &structure);
STRUCTURE_EXPORT void display(const StructureValue &value);
STRUCTURE_EXPORT void display(const std::unique_ptr<StructureValue> &value);
STRUCTURE_EXPORT void display(const std::unique_ptr<Block> &structure);
template <typename T>
void display(const std::unique_ptr<Field<T>> &structure)
{
    display(*structure);
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
}
