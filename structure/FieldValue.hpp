#pragma once

#include "Visitor.hpp"
#include "GenericFieldValue.hpp"
#include "Field.hpp"

#include <initializer_list>
#include <string>

namespace structure
{

template <typename T>
class FieldValue : public GenericFieldValue
{
public:
    FieldValue(const Field<T> &structure, std::string value)
        : GenericFieldValue(structure), mValue(value)
    {
    }

    const std::string getValue() const override { return mValue; }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }

private:
    std::string mValue;
};
}
