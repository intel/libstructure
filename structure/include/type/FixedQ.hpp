#pragma once

#include "structure_export.h"
#include "type/Field.hpp"

#include <cstdint>
#include <type_traits>

namespace structure
{
class STRUCTURE_EXPORT FixedQ : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    std::string getTypeName() const override { return "FixedQ"; }
    virtual size_t getSize() const = 0;
    virtual size_t getFractional() const = 0;
    virtual size_t getIntegral() const = 0;
    virtual bool getSignedness() const = 0;
};

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
    using Base =
        detail::FieldCrtp<NewFixedQ<size, fractional, isSigned, _Storage>, FixedQ, _Storage>;

    static constexpr double mDenominator{1UL << fractional};

public:
    using Base::Base;
    size_t getSize() const override { return size; }
    size_t getFractional() const override { return fractional; }
    size_t getIntegral() const override { return size - fractional - (isSigned ? 1 : 0); }
    bool getSignedness() const override { return isSigned; }

    static _Storage fromString(const std::string &input)
    {
        // TODO: we assume that the user inputs a decimal value... what is he wants to directly
        // enter the numerator?
        // TODO: choose the intermediate type according to the storage type
        double parsed;
        if (not convertTo(input, parsed)) {
            throw std::runtime_error("FixedPoint: Incorrect or out-of-bounds value");
        }

        // For Qn.m number, multiply by 2^n and round to the nearest integer
        // Q numbers are a ratio, of which the denominator is 2^fractional
        return static_cast<_Storage>(round(parsed * mDenominator));
    }
};

// This macro defines a new Q number type and checks that its parameters are coherent.
// Calling sizeof() in the type is a trick that forces the compiler to instantiate the type. If it
// wasn't, the static assertions contained in NewFixedQ wouln't be triggered.
#define MK_NEW_QTYPE(name, size, fractional, isSigned, storage)                                    \
    using name = NewFixedQ<size, fractional, isSigned, storage>;                                   \
    static_assert(sizeof(name), "This message shouldn't be displayed.")

MK_NEW_QTYPE(Q16f15, 16, 15, true, int16_t);
MK_NEW_QTYPE(UQ16f16, 16, 16, false, uint16_t);
MK_NEW_QTYPE(Q32f31, 32, 31, true, int32_t);
} // namespace structure
