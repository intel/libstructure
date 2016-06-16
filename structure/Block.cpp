#include "type/Block.hpp"
#include "Visitor.hpp"
#include "Exception.hpp"

#include <algorithm>
#include <iterator>

namespace structure
{

void Block::accept(StructureVisitor &visitor) const
{
    visitor.visit(*this);
}

std::vector<Block::StructureRef> Block::getFields() const
{
    std::vector<StructureRef> result;
    std::transform(begin(mFields), end(mFields), std::back_inserter(result),
                   [](auto &&from) { return StructureRef(*from); });
    return result;
}

BlockValue Block::with(ValueBuilder builder) const
{

    if (builder.atom) {
        throw ValueStructureMismatch(getName(), "Expected a Block, got an atom");
    }

    if (builder.listValue.size() != mFields.size()) {
        throw ValueStructureMismatch(getName(), "Expected " + std::to_string(mFields.size()) +
                                                    " fields; got " +
                                                    std::to_string(builder.listValue.size()));
    }

    BlockValue b(*this);

    auto it = builder.listValue.begin();
    for (auto &f : mFields) {
        if (it == builder.listValue.end()) {
            // This can't happen since we checked the sizes above
            throw ValueStructureMismatch(getName(), "Not enough fields were provided");
        }

        auto v = f->with(*(it++));
        b.addValue(std::move(v));
    }

    return b;
}

std::unique_ptr<StructureValue> Block::genericWith(ValueBuilder builder) const
{
    return std::make_unique<BlockValue>(with(builder));
}

std::unique_ptr<StructureValue> Block::doWith(ValueImporter &importer, std::string path) const
{
    auto value = std::make_unique<BlockValue>(*this);

    importer.onEnterBlock(*this);
    for (const auto &field : getFields()) {
        value->addValue(field.get().with(importer, path));
    }
    importer.onExitBlock(*this);

    return std::move(value);
}
}
