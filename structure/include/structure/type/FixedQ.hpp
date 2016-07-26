/*
 * Copyright (c) 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Intel Corporation nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/structure_export.h"
#include "structure/type/detail/stock.fw.hpp"
#include "structure/type/Field.hpp"

#include "structure/detail/safe_cast.hpp"
#include <cstdint>
#include <type_traits>

namespace structure
{
/** A type of field containing a Q-number (Fixed Point) */
class STRUCTURE_EXPORT FixedQ : public GenericField
{
public:
    using GenericField::GenericField;

    virtual size_t getSize() const = 0;
    virtual size_t getFractional() const = 0;
    virtual size_t getIntegral() const = 0;
    virtual bool getSignedness() const = 0;
};

/** Helper class for creating new Q-number field types
 *
 * Q numbers are a kind of fixed-point numbers. They have 3 characteristic:
 * - the size of their "integral" part (noted "i");
 * - the size of the "fractional" part (noter "f");
 * - their signedness.
 *
 * Another possible, equivalent characterisation is:
 * - their size (noted "s");
 * - the size of the fractional part;
 * - their signedness.
 *
 * In that second notation, the size of the integral part is implicit (size - fractional - (sign
 * bit if any)). A Q number format can be noted:  Qs.f (or UQs.f for an unsigned number); this is
 * the representation chosen here.
 *
 * They have a fixed precision over their range of values. The precision is 1/2^f; their max
 * value is:
 * - 2^i (for an unsigned Q);
 * - 2^i - (1/2^f) (for a signed Q);
 * their min value is:
 * - 0 (for an unsigned Q)
 * - -(2^i) (for a signed Q).
 *
 * Their in-memory representation is N in the following equation: value = N/2^f
 *
 * @tparam size The size (typically 8, 16, 32 or 64).
 * @tparam fractional The size of the "fractional part".
 * @tparam isSigned The signedness.
 * @tparam _Storage The type with which values will be stored (e.g. `uint32_t`); consistency with
 *                  other template parameters is checked at compile time.
 *
 * @ingroup StockTypes
 */
template <size_t size, size_t fractional, bool isSigned, class _Storage>
class NewFixedQ
    : public detail::FieldCrtp<NewFixedQ<size, fractional, isSigned, _Storage>, FixedQ, _Storage>
{
    // We could write a clever class template to deduce the ideal storage type but for now, it's
    // easier to just pass it as argument and statically check it.
    static_assert(std::is_integral<_Storage>::value,
                  "The specified storage must be an integral type.");
    static_assert(size <= sizeof(_Storage) * 8,
                  "The specified storage is smaller than the specified size.");
    static_assert(fractional + (isSigned ? 1 : 0) <= size,
                  "The fractional part plus the optional sign are larger than the size.");
    static_assert(isSigned == std::is_signed<_Storage>::value,
                  "The specified has a different signedness than the specified one.");

private:
    using This = NewFixedQ<size, fractional, isSigned, _Storage>;
    using ThisValue = FieldValue<This>;
    using Base = detail::FieldCrtp<This, FixedQ, _Storage>;

    static constexpr double mDenominator{1UL << fractional};

    /** Creates a value from the given double representing a decimal number
     */
    std::unique_ptr<GenericFieldValue> withTyped(double value) const override
    {
        return std::make_unique<ThisValue>(*this,
                                           static_cast<_Storage>(round(value * mDenominator)));
    }

public:
    using Base::Base;
    size_t getSize() const override { return size; }
    size_t getFractional() const override { return fractional; }
    size_t getIntegral() const override { return size - fractional - (isSigned ? 1 : 0); }
    bool getSignedness() const override { return isSigned; }

    /** Parse a decimal number into a FixedQ
     *
     * @param[in] input A string representing a decimal number (e.g. "1.125").
     * @returns the memory representation for the Q number.
     */
    static _Storage fromString(const std::string &input)
    {
        /** @todo We assume that the user inputs a decimal value... what is he wants to directly
         * enter the numerator?
         */
        /** @todo choose the intermediate type according to the storage type. */
        double parsed = safe_cast<double>(input);
        /** @todo range checking. */

        // For Qn.m number, multiply by 2^n and round to the nearest integer
        // Q numbers are a ratio, of which the denominator is 2^fractional
        return static_cast<_Storage>(round(parsed * mDenominator));
    }

    /** @returns the human-readable name of the field type (e.g. "Q16.4") */
    static std::string typeToString()
    {
        return std::string(isSigned ? "Q" : "UQ") + std::to_string(size) + "f" +
               std::to_string(fractional);
    }
};

/** This macro defines a new Q number type.
 *
 * It also checks that its parameters are coherent. Calling sizeof() in the type is a trick that
 * forces the compiler to instantiate the type. If it wasn't, the static assertions contained in
 * NewFixedQ wouln't be triggered.
 *
 * @ingroup StockTypes
 */
#define MK_NEW_QTYPE(name, size, fractional, isSigned, storage)                                    \
    using name = NewFixedQ<size, fractional, isSigned, storage>;                                   \
    static_assert(sizeof(name), "This message shouldn't be displayed.")

MK_NEW_QTYPE(Q16f15, 16, 15, true, int16_t);
MK_NEW_QTYPE(UQ16f16, 16, 16, false, uint16_t);
MK_NEW_QTYPE(Q32f31, 32, 31, true, int32_t);
} // namespace structure
