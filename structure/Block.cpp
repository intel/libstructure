#include "block.hpp"
#include "visitor.hpp"
#include <iostream>
#include <typeinfo>

namespace structure
{

Block::Block(std::string name) : Structure(name)
{
}

Block::~Block()
{
}

void Block::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

void Block::addField(std::unique_ptr<Structure> &child)
{
    mFields.push_back(std::move(child));
}

std::list<std::unique_ptr<Structure>> &Block::getFields()
{
    return mFields;
}
}