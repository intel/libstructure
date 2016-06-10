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
/** An instantiated Field
 *
 * @tparam FieldType The type of the field with which this value is instantiated.
 */
template <typename FieldType>
class FieldValue : public GenericFieldValue
{
    // This prevents hard-to-understand compilation errors.
    static_assert(std::is_base_of<GenericField, FieldType>::value,
                  "The FieldType of a FieldValue must be a GenericField.");

public:
    /** Constructs a value from a Field and typed value */
    FieldValue(FieldType field, typename FieldType::Storage value)
        : mStructure(field), mValue(value)
    {
    }

    /** Constructs a value from a Field and string */
    FieldValue(FieldType field, const std::string &value)
        : mStructure(field), mValue(mStructure.fromString(value))
    {
    }

    std::string getValue() const override { return std::to_string(mValue); }
    void setValue(std::string value) { mValue = mStructure.fromString(value); }

    typename FieldType::Storage getTypedValue() const { return mValue; }
    void setTypedValue(typename FieldType::Storage value) { mValue = value; }

    /** @returns the Field type corresponding to this value
     *
     * Same as getStructure() but with a stronger type.
     */
    const FieldType &getField() const { return mStructure; }
    const Structure &getStructure() const override { return mStructure; }

    void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    void accept(StorageVisitor &visitor) const override { visitor.visitStorage(mValue); }

private:
    const FieldType mStructure;
    typename FieldType::Storage mValue;
};
}
