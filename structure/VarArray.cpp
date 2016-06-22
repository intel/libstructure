#include "type/VarArray.hpp"
#include "Exception.hpp"

namespace structure
{

std::unique_ptr<StructureValue> VarArray::doBuild(ValueImporter &importer,
                                                  const std::string &path) const
{
    auto b = std::make_unique<BlockValue>(*this);
    importer.onEnterBlock(path);

    try {
        while (true) {
            b->addValue(getFields()[0].get().build(importer, path));
        }
    } catch (NotEnoughValues & /*ex*/) {
        // VarArray is supposed to consume all values in Block
        // The only way to detect that all the values have been consumed is to catch
        // a NotEnoughValue exception
    }

    importer.onExitBlock(path);
    return std::move(b);
}

std::string VarArray::getTypeName() const
{
    return "VarArray";
}
};
