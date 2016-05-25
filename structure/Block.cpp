#include "Block.hpp"
#include "Visitor.hpp"
#include "BlockValue.hpp"
#include "Exception.hpp"

namespace structure
{

void Block::accept(StructureVisitor &visitor) const
{
    visitor.visit(*this);
}

const std::list<std::unique_ptr<Structure>> &Block::getFields() const
{
    return mFields;
}

std::unique_ptr<StructureValue> Block::with(ValueBuilder builder) const
{

    if (builder.atom)
        throw ValueStructureMismatch(getName());

    if (builder.listValue.size() != mFields.size())
        throw ValueStructureMismatch(getName());

    auto b = std::make_unique<BlockValue>(*this);

    auto it = builder.listValue.begin();
    for (auto &f : mFields) {
        if (it == builder.listValue.end())
            throw ValueStructureMismatch(getName());

        auto v = f->with(*(it++));
        b->addValue(std::move(v));
    }

    return std::move(b);
}
}
