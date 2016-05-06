#include "block.hpp"
#include "visitor.hpp"

namespace structure
{

Block::Block(std::string name, std::initializer_list<Structure *> fields) : Structure(name)
{
    mFields.insert(mFields.end(), fields.begin(), fields.end());
}

Block::~Block()
{
}

void Block::accept(Visitor *visitor)
{
    visitor->visit(this);
}

std::list<Structure *> Block::getFields()
{
    return mFields;
}
}