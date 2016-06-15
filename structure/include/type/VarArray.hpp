#pragma once

#include "structure_export.h"

#include "type/Block.hpp"

#include <utility>

namespace structure
{
/** Represents a Variable-Length Array (aka VLA) */
class STRUCTURE_EXPORT VarArray : public Block
{
public:
    /** Constructs a Variable-Length Array of any kind of field.
     *
     * @param[in] itemType The structure that is repeated in the array.
     */
    template <class ItemType>
    VarArray(const std::string &name, ItemType &&itemType)
        : Block(name, std::forward<ItemType>(itemType))
    {
        static_assert(is_structure<ItemType>::value,
                      "The ItemType in a VarArray must be a Structure");
    }

    BlockValue build(ValueImporter &importer, const std::string &path = "") const override;
    std::string getTypeName() const override;

private:
    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override;
};
} // namespace structure
