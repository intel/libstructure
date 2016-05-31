#pragma once

#include "structure_export.h"

#include "type/StockTypes.hpp"
#include "type/Field.hpp"
#include "value/FieldValue.hpp"
#include "Visitor.hpp"
#include <cstdint>

namespace structure
{
class STRUCTURE_EXPORT Integer : public GenericField
{
public:
    using GenericField::GenericField;

    void accept(StructureVisitor &visitor) const override { visitor.visit(*this); }

    std::string getTypeName() const override { return "Integer"; }
    virtual size_t getSize() const = 0;
    virtual bool getSignedness() const = 0;
};

template <size_t size, bool isSigned, class _Storage>
class NewInteger : public detail::FieldCrtp<NewInteger<size, isSigned, _Storage>, Integer, _Storage>
{
    // We could write a clever class template to deduce the ideal storage type but for now, it's
    // easier to just pass it as argument and statically check it.
    static_assert(size <= sizeof(_Storage) * 8,
                  "The specified storage is smaller than the specified size.");
    static_assert(isSigned == std::is_signed<_Storage>::value,
                  "The specified has a different signedness than the specified one.");

private:
    using Base = detail::FieldCrtp<NewInteger<size, isSigned, _Storage>, Integer, _Storage>;

public:
    using Base::Base;
    size_t getSize() const override { return size; }
    bool getSignedness() const override { return isSigned; }
};

using UInt32 = NewInteger<32, false, uint32_t>;
using Int32 = NewInteger<32, true, int32_t>;
using UInt16 = NewInteger<16, false, uint16_t>;
using Int16 = NewInteger<16, true, int16_t>;
using UInt8 = NewInteger<8, false, uint8_t>;
using Int8 = NewInteger<8, true, int8_t>;
}
