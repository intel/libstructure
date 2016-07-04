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

#include "structure_export.h"

#include "type/StockTypes.hpp"
#include "type/Field.hpp"
#include "attributes/Range.hpp"
#include "value/FieldValue.hpp"
#include "Visitor.hpp"

namespace structure
{
/** A type of field containing an integer value. */
class STRUCTURE_EXPORT Integer : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    virtual size_t getSize() const = 0;
    virtual bool getSignedness() const = 0;
};

template <typename Storage>
class IntegerAttributes : public GenericFieldAttributes
{
public:
    attributes::Range<Storage> mRange;

    template <class T>
    void set(const attributes::Range<T> &range)
    {
        mRange = range;
    }
    template <typename C>
    void set(const C &c)
    {
        GenericFieldAttributes::set(c);
    }
};

/** Helper class for creating new integer field types
 *
 * @tparam size The size (typically 8, 16, 32 or 64).
 * @tparam isSigned The signedness.
 * @tparam _Storage The type with which values will be stored (e.g. `uint32_t`); consistency with
 *                  other template parameters is checked at compile time.
 *
 * @ingroup StockTypes
 */
template <size_t size, bool isSigned, class _Storage>
class NewInteger : public detail::FieldCrtp<NewInteger<size, isSigned, _Storage>, Integer, _Storage,
                                            IntegerAttributes<_Storage>>
{
    // We could write a clever class template to deduce the ideal storage type but for now, it's
    // easier to just pass it as argument and statically check it.
    static_assert(std::is_integral<_Storage>::value,
                  "The specified storage must be an integral type.");
    static_assert(size <= sizeof(_Storage) * 8,
                  "The specified storage is smaller than the specified size.");
    static_assert(isSigned == std::is_signed<_Storage>::value,
                  "The specified has a different signedness than the specified one.");

private:
    using This = NewInteger<size, isSigned, _Storage>;
    using ThisValue = FieldValue<This>;
    using Base = detail::FieldCrtp<This, Integer, _Storage, IntegerAttributes<_Storage>>;

    std::unique_ptr<GenericFieldValue> withTyped(long long value) const override
    {
        return this->withTypedTemplate(value);
    }

    std::unique_ptr<GenericFieldValue> withTyped(unsigned long long value) const override
    {
        return this->withTypedTemplate(value);
    }

public:
    using Base::Base;

    size_t getSize() const override { return size; }
    bool getSignedness() const override { return isSigned; }

    bool isAllowed(const _Storage &value) const override
    {
        auto &range = this->getAttributes().mRange;
        return range.min() <= value and value <= range.max();
    }

    /** @returns the human-readable name of the field type */
    static std::string typeToString()
    {
        return std::string(isSigned ? "Int" : "UInt") + std::to_string(size);
    }
};

/** @addtogroup StockTypes
 * @{
 */
using UInt64 = NewInteger<64, false, uint64_t>;
using Int64 = NewInteger<64, true, int64_t>;
using UInt32 = NewInteger<32, false, uint32_t>;
using Int32 = NewInteger<32, true, int32_t>;
using UInt16 = NewInteger<16, false, uint16_t>;
using Int16 = NewInteger<16, true, int16_t>;
using UInt8 = NewInteger<8, false, uint8_t>;
using Int8 = NewInteger<8, true, int8_t>;
/** @} */
}
