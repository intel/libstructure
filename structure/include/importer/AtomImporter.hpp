#pragma once

#include "ValueImporter.hpp"
#include "type/GenericField.hpp"
#include "type/Block.hpp"
#include "value/GenericFieldValue.hpp"

#include <string>

namespace structure
{
/** Wraps a string literal for the purpose of ValueInitializer
 */
class AtomImporter : public ValueImporter
{
public:
    AtomImporter(const char *value) : mValue(value) {}

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &) override
    {
        return field.with(mValue);
    }

    void onEnterBlock(const Block &) override { throw WrongType("Block", "Atom"); }

    void onExitBlock(const Block &) override { throw WrongType("Block", "Atom"); }

private:
    std::string mValue;
};
}
