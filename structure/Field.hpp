#pragma once

#include "GenericField.hpp"
#include "FieldValue.hpp"
#include "FieldTraits.hpp"
#include "Visitor.hpp"
#include "ValueBuilder.hpp"
#include "Exception.hpp"

#include "Field.fw.hpp"

namespace structure
{

template <typename T>
class Field : public GenericField
{
public:
    Field(std::string name) : GenericField(name) {}

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<StructureValue> with(ValueBuilder builder) const override
    {
        if (!builder.atom)
            throw ValueStructureMismatch(getName());

        return std::make_unique<FieldValue<T>>(*this, builder.atomicValue);
    }
    std::string getTypeName() const override { return FieldTraits<Field<T>>::name; }
};
}
