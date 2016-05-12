#pragma once

#include "Structure.hpp"
#include "Block.hpp"
#include "GenericField.hpp"
#include "Visitor.hpp"
#include "Field.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>

namespace structure
{

using BlockFunction = std::function<void(const Block &)>;
const BlockFunction DefaultBlockFunction = [](const Block &) {};

using FieldFunction = std::function<void(const GenericField &)>;
const FieldFunction DefaultFieldFunction = [](const GenericField &) {};

void apply(const Structure &structure, BlockFunction onEnterBlock, BlockFunction onExitBlock,
           FieldFunction onEnterField, bool recursive);

void display(const Structure &structure);

Structure &getChild(const Structure &structure, std::string path);

// Builders

std::unique_ptr<Field<float>> makeFloat(std::string name);
std::unique_ptr<Field<int>> makeInteger(std::string name);

void addFields(Block &parent);

template <typename T, typename... Fields>
void addFields(Block &parent, T first, Fields... fields)
{
    parent.addField(std::move(first));
    addFields(parent, std::move(fields)...);
}

template <typename... Fields>
std::unique_ptr<Block> makeBlock(std::string name, Fields... fields)
{
    auto b = std::make_unique<Block>(name);
    addFields(*b, std::move(fields)...);
    return b;
}
}
