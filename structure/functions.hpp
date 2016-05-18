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

Structure &getChild(const Structure &structure, std::string path);

std::unique_ptr<StructureValue> with(Structure &structure, ValueBuilder builder);

// Builders

std::unique_ptr<Field<float>> makeFloat(std::string name);
std::unique_ptr<Field<int>> makeInteger(std::string name);

template <typename... Fields>
std::unique_ptr<Block> makeBlock(std::string name, Fields... fields)
{
    auto b = std::make_unique<Block>(name, std::move(fields)...);
    return b;
}
}
