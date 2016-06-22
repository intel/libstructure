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

/** A type of field containing a floating point value. */
class STRUCTURE_EXPORT FloatingPoint : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }
};

/** Helper class for creating new floating point field types
 *
 * @tparam _Storage The type with which values will be stored (e.g. `float`)
 *
 * @ingroup StockTypes
 */
template <class _Storage>
class NewFloatingPoint
    : public detail::FieldCrtp<NewFloatingPoint<_Storage>, FloatingPoint, _Storage>
{
    static_assert(std::is_floating_point<_Storage>::value,
                  "The specified storage is not a floating point type.");

private:
    using This = NewFloatingPoint<_Storage>;
    using ThisValue = FieldValue<This>;
    using Base = detail::FieldCrtp<This, FloatingPoint, _Storage>;

public:
    using Base::Base;

    /** @returns the human-readable name of the field type */
    static std::string typeToString() { return detail::FloatingTrait<_Storage>::getName(); }
};

/** @addtogroup StockTypes
 * @{
 */
using Float = NewFloatingPoint<float>;
using Double = NewFloatingPoint<double>;
using LongDouble = NewFloatingPoint<long double>;
/** @} */
} // namespace structure
