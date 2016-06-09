#pragma once

#include "structure_export.h"

#include "type/Block.hpp"

#include <utility>

namespace structure
{
class STRUCTURE_EXPORT VarArray : public Block
{
public:
    template <class ItemType>
    VarArray(const std::string &name, ItemType &&itemType)
        : Block(name, std::forward<ItemType>(itemType))
    {
        static_assert(is_structure<ItemType>::value,
                      "The ItemType in a VarArray must be a Structure");
    }

    BlockValue with(ValueBuilder builder) const override;
    std::string getTypeName() const override;
};
} // namespace structure
