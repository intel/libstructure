#pragma once

#include "ValueImporter.hpp"
#include "value/GenericFieldValue.hpp"

namespace structure
{
/** Wraps an importer for the purpose of ValueInitializer
 *
 * A ValueInitializer can contain an importer thanks to this class. If the ValueInitializer directly
 * contained the unwrapped importer, the latter would be destroyed upon destruction of the
 * ValueInitializer, which is not intended.
 */
class WrapImporter : public ValueImporter
{
public:
    WrapImporter(ValueImporter &importer) : mImporter(importer) {}

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override
    {
        return mImporter.import(field, path);
    }

    void onEnterBlock(const Block &block) override { mImporter.onEnterBlock(block); }
    void onExitBlock(const Block &block) override { mImporter.onExitBlock(block); }

private:
    ValueImporter &mImporter;
};
}
