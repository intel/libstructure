#include "ValueInitializer.hpp"
#include "importer/AtomImporter.hpp"
#include "importer/ListImporter.hpp"
#include "importer/WrapImporter.hpp"

namespace structure
{
ValueInitializer::ValueInitializer(const char *atomicValue)
    : mImporter(new AtomImporter(atomicValue))
{
}
ValueInitializer::ValueInitializer(ValueImporter &importer) : mImporter(new WrapImporter(importer))
{
}
ValueInitializer::ValueInitializer(std::initializer_list<ValueInitializer> list)
    : mImporter(new ListImporter(list))
{
}

std::unique_ptr<GenericFieldValue> ValueInitializer::import(const GenericField &field,
                                                            const std::string &path)
{
    return mImporter->import(field, path);
}
void ValueInitializer::onEnterBlock(const Block &block)
{
    mImporter->onEnterBlock(block);
}
void ValueInitializer::onExitBlock(const Block &block)
{
    mImporter->onExitBlock(block);
}
} // namespace structure
