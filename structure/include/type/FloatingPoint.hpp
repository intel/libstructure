#pragma once

#include "structure_export.h"

#include "type/StockTypes.hpp"
#include "type/Field.hpp"

namespace structure
{
namespace detail
{
template <typename F>
struct FloatingTrait
{
    static std::string getName();
};

template <>
struct FloatingTrait<float>
{
    static std::string getName() { return "Float"; }
};
template <>
struct FloatingTrait<double>
{
    static std::string getName() { return "Double"; }
};
template <>
struct FloatingTrait<long double>
{
    static std::string getName() { return "LongDouble"; }
};
} // namespace detail

class STRUCTURE_EXPORT FloatingPoint : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }
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

    static std::string typeToString() { return detail::FloatingTrait<_Storage>::getName(); }
};

using Float = NewFloatingPoint<float>;
using Double = NewFloatingPoint<double>;
using LongDouble = NewFloatingPoint<long double>;
}
