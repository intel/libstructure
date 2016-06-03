#pragma once

#include "structure_export.h"

#include "type/StockTypes.hpp"
#include "type/Field.hpp"

namespace structure
{
class STRUCTURE_EXPORT FloatingPoint : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    std::string getTypeName() const override { return "FloatingPoint"; }
};

template <class _Storage>
class NewFloatingPoint
    : public detail::FieldCrtp<NewFloatingPoint<_Storage>, FloatingPoint, _Storage>
{
    static_assert(std::is_floating_point<_Storage>::value,
                  "The specified storage is not a floating point type.");

private:
    using Base = detail::FieldCrtp<NewFloatingPoint<_Storage>, FloatingPoint, _Storage>;

public:
    using Base::Base;
};

using Float = NewFloatingPoint<float>;
using Double = NewFloatingPoint<double>;
using LongDouble = NewFloatingPoint<long double>;
}
