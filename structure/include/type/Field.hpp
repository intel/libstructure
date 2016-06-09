#pragma once

#include "type/GenericField.hpp"
#include "value/FieldValue.hpp"
#include "ValueBuilder.hpp"
#include "Exception.hpp"

#include "structure_export.h"

namespace structure
{
namespace detail
{
// CRTP = Curiously Reccuring Template Pattern
template <typename Derived, typename Base, typename _Storage>
class FieldCrtp : public Base
{
    // This prevents hard-to-understand compilation errors.
    static_assert(is_structure<Base>::value, "Base must be a Structure.");

private:
    using This = FieldCrtp<Derived, Base, _Storage>;
    using ThisValue = FieldValue<Derived>;

public:
    using Base::Base;

    using Storage = _Storage;

    ThisValue with(ValueBuilder builder) const
    {
        if (!builder.atom) {
            throw ValueStructureMismatch(this->getName(),
                                         "Expected a " + getTypeName() + ", got a Block.");
        }

        return {*static_cast<const Derived *>(this), builder.atomicValue};
    }

    static _Storage fromString(const std::string &input)
    {
        _Storage parsed;
        if (not convertTo(input, parsed)) {
            throw ParseError(input);
        }
        return parsed;
    }

    std::string getTypeName() const override { return Derived::typeToString(); }

private:
    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override
    {
        return std::make_unique<ThisValue>(with(builder));
    }
};
} // namespace detail
}
