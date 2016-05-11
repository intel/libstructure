#pragma once

#include "Structure.hpp"
#include "Block.hpp"
#include "GenericField.hpp"
#include "Visitor.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>

namespace structure
{

using BlockFunction = std::function<void(Block &)>;
const BlockFunction DefaultBlockFunction = [](Block &) {};

using FieldFunction = std::function<void(GenericField &)>;
const FieldFunction DefaultFieldFunction = [](GenericField &) {};

void apply(std::unique_ptr<Structure> &structure, BlockFunction onEnterBlock,
           BlockFunction onExitBlock, FieldFunction onEnterField, bool recursive);

void display(std::unique_ptr<Structure> &structure);

const std::unique_ptr<Structure> &getChild(std::unique_ptr<Structure> &structure, std::string path);

void addField(std::unique_ptr<Structure> &parent, std::unique_ptr<Structure> child);

// Builders

std::unique_ptr<Structure> makeBlock(std::string name);
std::unique_ptr<Structure> makeFloat(std::string name);
std::unique_ptr<Structure> makeInteger(std::string name);

std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1);
std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2);
std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2, std::unique_ptr<Structure> f3);
std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2, std::unique_ptr<Structure> f3,
                                     std::unique_ptr<Structure> f4);
}
