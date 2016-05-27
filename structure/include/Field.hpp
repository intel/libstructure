#pragma once

#include "GenericField.hpp"
#include "FieldValue.hpp"
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

    FieldValue<T> with(ValueBuilder builder) const
    {
        if (!builder.atom)
            throw ValueStructureMismatch(getName());

        return FieldValue<T>(*this, builder.atomicValue);
    }
    std::string getTypeName() const override { return typeName; }

private:
    std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const override
    {
        return std::make_unique<FieldValue<T>>(with(builder));
    }

    static const std::string typeName;
};
}
