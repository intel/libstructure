#pragma once

#include "GenericField.hpp"
#include "FieldValue.hpp"
#include "Visitor.hpp"
#include "ValueBuilder.hpp"
#include "Exception.hpp"

#include "Field.fw.hpp"
#include "StockTypes.hpp"

#include "structure_export.h"

namespace structure
{
namespace detail
{
// CRTP = Curiously Reccuring Template Pattern
template <typename Derived, typename Base, typename _Storage>
class FieldCrtp : public Base
{
    // This prevents hard-to-understand compilation errors. The base class must be a GenericField
    // because it need the getName() method.
    static_assert(std::is_convertible<Base, const GenericField &>::value,
                  "Base must be a GenericField.");

private:
    using This = FieldCrtp<Derived, Base, _Storage>;
    using ThisValue = FieldValue<Derived>;

public:
    using Base::Base;

    using Storage = _Storage;

    ThisValue with(ValueBuilder builder) const
    {
        if (!builder.atom)
            throw ValueStructureMismatch(static_cast<const Base &>(*this).getName());

        return {*static_cast<const Derived *>(this), builder.atomicValue};
    }

private:
    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override
    {
        return std::make_unique<ThisValue>(with(builder));
    }
};
} // namespace detail

template <typename T>
class Field : public GenericField
{
private:
    using This = Field<T>;

public:
    using Storage = T;

    Field(std::string name) : GenericField(name) {}

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }
    std::string getTypeName() const override { return typeName; }

    FieldValue<This> with(ValueBuilder builder) const
    {
        if (!builder.atom)
            throw ValueStructureMismatch(getName());

        return FieldValue<This>(*this, builder.atomicValue);
    }

private:
    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override
    {
        return std::make_unique<FieldValue<This>>(with(builder));
    }

    STRUCTURE_EXPORT static const std::string typeName;
};
}
