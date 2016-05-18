#pragma once

#include "GenericField.hpp"
#include "Visitor.hpp"
#include "ValueBuilder.hpp"
#include "Exception.hpp"

namespace structure
{

template <typename T>
class Field : public GenericField
{
public:
    Field(std::string name) : GenericField(name) {}

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<StructureValue> with(ValueBuilder builder) const override;
};
}

#include "FieldValue.hpp"

namespace structure
{

template <typename T>
std::unique_ptr<StructureValue> Field<T>::with(ValueBuilder builder) const
{
    if (!builder.atom)
        throw ValueStructureMismatch(getName());

    return std::make_unique<FieldValue<T>>(*this, builder.atomicValue);
}
}