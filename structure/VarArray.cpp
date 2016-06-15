#include "type/VarArray.hpp"
#include "Exception.hpp"

namespace structure
{
BlockValue VarArray::build(ValueImporter &importer, const std::string &path) const
{
    BlockValue b(*this);
    importer.onEnterBlock(*this);

    try {
        while (true) {
            b.addValue(getFields()[0].get().build(importer, path));
        }
    } catch (NotEnoughValues & /*ex*/) {
        // VarArray is supposed to consume all values in Block
        // The only way to detect that all the values have been consumed is to catch
        // a NotEnoughValue exception
    }

    importer.onExitBlock(*this);
    return b;
}

std::unique_ptr<StructureValue> VarArray::doBuild(ValueImporter &importer,
                                                  const std::string &path) const
{
    return std::make_unique<BlockValue>(build(importer, path));
}

std::string VarArray::getTypeName() const
{
    return "VarArray";
}
};
