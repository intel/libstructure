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
        : GenericFieldValue(structure), mValue(fromString(value))
    {
    }

    const std::string getValue() const override { return toString(mValue); }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }

private:
    static T fromString(std::string value);
    static std::string toString(T value);

    T mValue;
};

// Integer
template <>
inline int FieldValue<int>::fromString(std::string value)
{
    return std::atoi(value.c_str());
}
template <>
inline std::string FieldValue<int>::toString(int value)
{
    return std::to_string(value);
}

// Float
template <>
inline float FieldValue<float>::fromString(std::string value)
{
    return (float)std::atof(value.c_str());
}
template <>
inline std::string FieldValue<float>::toString(float value)
{
    return std::to_string(value);
}

// String
template <>
inline std::string FieldValue<std::string>::fromString(std::string value)
{
    return value;
}
template <>
inline std::string FieldValue<std::string>::toString(std::string value)
{
    return value;
}
}
