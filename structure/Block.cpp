#include "type/Block.hpp"
#include "Visitor.hpp"
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

BlockValue Block::with(ValueBuilder builder) const
{

    if (builder.atom)
        throw ValueStructureMismatch(getName());

    if (builder.listValue.size() != mFields.size())
        throw ValueStructureMismatch(getName());

    BlockValue b(*this);

    auto it = builder.listValue.begin();
    for (auto &f : mFields) {
        if (it == builder.listValue.end())
            throw ValueStructureMismatch(getName());

        auto v = f->with(*(it++));
        b.addValue(std::move(v));
    }

    return b;
}

std::unique_ptr<StructureValue> Block::genericWith(ValueBuilder builder) const
{
    return std::make_unique<BlockValue>(with(builder));
}
}
