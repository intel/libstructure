#pragma once

#include "ValueImporter.hpp"
#include "type/GenericField.hpp"
#include "type/Block.hpp"
#include "value/GenericFieldValue.hpp"

#include <string>

namespace structure
{
/** Wraps a value of any type for the purpose of ValueInitializer
 *
 * @tparam T : The type of the atom value
 */
template <class T>
class AtomImporter : public ValueImporter
{
public:
    AtomImporter(T value) : mValue(value) {}

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &) override
    {
        return field.with(mValue);
    }

    void onEnterBlock(const std::string &) override { throw WrongType("Block", "Atom"); }

    void onExitBlock(const std::string &) override { throw WrongType("Block", "Atom"); }

private:
    T mValue;
};
}
