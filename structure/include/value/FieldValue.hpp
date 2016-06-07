#pragma once

#include "value/GenericFieldValue.hpp"
#include "Visitor.hpp"

#include "convert.hpp"

#include <initializer_list>
#include <string>
#include <iostream>
#include <type_traits>
#include <stdexcept>

namespace structure
{

template <typename FieldType>
class FieldValue : public GenericFieldValue
{
    // This prevents hard-to-understand compilation errors.
    static_assert(std::is_convertible<FieldType, const GenericField &>::value,
                  "FieldType must be a GenericField.");

public:
    FieldValue(FieldType field, typename FieldType::Storage value)
        : mStructure(field), mValue(value)
    {
    }

    FieldValue(FieldType field, const std::string &value)
        : mStructure(field), mValue(mStructure.fromString(value))
    {
    }

    std::string getValue() const override { return std::to_string(mValue); }
    void setValue(std::string value) { mValue = mStructure.fromString(value); }

    typename FieldType::Storage getTypedValue() const { return mValue; }
    void setTypedValue(typename FieldType::Storage value) { mValue = value; }

    const FieldType &getField() const { return mStructure; }
    const Structure &getStructure() const override { return mStructure; }

    void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    void accept(StorageVisitor &visitor) const override { visitor.visitStorage(mValue); }

private:
    const FieldType mStructure;
    typename FieldType::Storage mValue;
};
}
