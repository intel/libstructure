/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/structure_export.h"

#include "structure/type/Numerical.hpp"
#include "structure/type/detail/stock.fw.hpp"
#include "structure/type/Field.hpp"

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
};

/** Helper class for creating new floating point field types
 *
 * @tparam _Storage The type with which values will be stored (e.g. `float`)
 *
 * @ingroup StockTypes
 */
template <class _Storage>
class NewFloatingPoint : public detail::FieldCrtp<NewFloatingPoint<_Storage>, FloatingPoint,
                                                  _Storage, NumericalAttributes<_Storage>>
{
    static_assert(std::is_floating_point<_Storage>::value,
                  "The specified storage is not a floating point type.");

private:
    using This = NewFloatingPoint<_Storage>;
    using ThisValue = FieldValue<This>;
    using Base = detail::FieldCrtp<This, FloatingPoint, _Storage, NumericalAttributes<_Storage>>;

    /** Creates a value from the given float
     */
    std::unique_ptr<GenericFieldValue> withTyped(float value) const override
    {
        return this->withTypedTemplate(value);
    }
    /** Creates a value from the given double
     */
    std::unique_ptr<GenericFieldValue> withTyped(double value) const override
    {
        return this->withTypedTemplate(value);
    }

public:
    using Base::Base;

    bool isAllowed(const _Storage &value) const
    {
        auto &range = this->getAttributes().mRange;
        return range.min() <= value and value <= range.max();
    }

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
