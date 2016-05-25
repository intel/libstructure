#pragma once

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

void apply(const Structure &structure, BlockFunction onEnterBlock, BlockFunction onExitBlock,
           FieldFunction onEnterField, bool recursive);

// Value

using BlockValueFunction = std::function<void(const BlockValue &)>;
const BlockValueFunction defaultBlockValueFunction = [](const BlockValue &) {};

using FieldValueFunction = std::function<void(const GenericFieldValue &)>;
const FieldValueFunction defaultFieldValueFunction = [](const GenericFieldValue &) {};

void apply(const StructureValue &structure, BlockValueFunction onEnterBlock,
           BlockValueFunction onExitBlock, FieldValueFunction onEnterField, bool recursive);

// Functions

void display(const Structure &structure);
void display(const StructureValue &value);
void display(const std::unique_ptr<StructureValue> &value);
void display(const std::unique_ptr<Block> &structure);
template <typename T>
void display(const std::unique_ptr<Field<T>> &structure)
{
    display(*structure);
}

std::string getValue(const StructureValue &value);
std::string getValue(const std::unique_ptr<StructureValue> &value);

Structure &getChild(const Structure &structure, std::string path);
StructureValue &getChild(const StructureValue &value, std::string path);
StructureValue &getChild(const std::unique_ptr<StructureValue> &value, std::string path);
Structure &getChild(const std::unique_ptr<Block> &structure, std::string path);
template <typename T>
Structure &getChild(const std::unique_ptr<Field<T>> &structure, std::string path)
{
    return getChild(*structure, path);
}

std::unique_ptr<StructureValue> with(const Structure &structure, ValueBuilder builder);
std::unique_ptr<StructureValue> with(const std::unique_ptr<Block> &structure, ValueBuilder builder);
template <typename T>
std::unique_ptr<StructureValue> with(const std::unique_ptr<Field<T>> &structure,
                                     ValueBuilder builder)
{
    return with(*structure, builder);
}
}
