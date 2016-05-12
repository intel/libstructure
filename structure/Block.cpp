#include "Block.hpp"
#include "Visitor.hpp"

namespace structure
{

Block::Block(std::string name) : Structure(name)
{
}

void Block::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void Block::addField(std::unique_ptr<Structure> child)
{
    mFields.push_back(std::move(child));
}

const std::list<std::unique_ptr<Structure>> &Block::getFields() const
{
    return mFields;
}
}
