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

std::unique_ptr<StructureValue> Block::with(ValueInitializer initializer) const
{
    return build(initializer);
}

std::unique_ptr<StructureValue> Block::doBuild(ValueImporter &importer,
                                               const std::string &path) const

{
    auto value = std::make_unique<BlockValue>(*this);

    importer.onEnterBlock(*this);
    for (const auto &field : getFields()) {
        value->addValue(field.get().build(importer, path));
    }
    importer.onExitBlock(*this);

    return std::move(value);
}
}
