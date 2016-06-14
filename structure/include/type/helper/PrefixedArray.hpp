#pragma once

#include "type/Block.hpp"
#include "Exception.hpp"
#include "functions.hpp"

#include <limits>

namespace structure
{
/** Helper for the "Length-Value" idiom
 *
 * A "Length-Value" structure is an array preceded by its number of items.
 *
 * @todo Add a non-template equivalent class (for cases where we want to dynamically create a
 *       PrefixedArray).
 *
 * @tparam Prefix : The type of field prefixing the array (must contain an integer).
 */
template <typename Prefix>
class PrefixedArray : public Block
{
    // TODO: should this rely on the return type of Prefix::getValue() instead?
    static_assert(std::is_integral<typename Prefix::Storage>::value,
                  "The type of a PrefixedArray's prefix must be an integer field");

public:
    template <typename ItemType>
    PrefixedArray(const std::string &name, ItemType &&itemType,
                  const std::string &prefixName = "count")
        : Block(name, std::forward<ItemType>(itemType)), mPrefix(prefixName)
    {
    }

    std::string getTypeName() const override { return "LV (" + mPrefix.getTypeName() + ")"; }

private:
    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override
    {
        using Count = typename Prefix::Storage;
        auto value = std::make_unique<BlockValue>(*this);

        importer.onEnterBlock(*this);                    // Enter the prefixed array block ...
        auto countField = mPrefix.build(importer, path); // ... starting with the prefix ...
        Count count = 0;
        if (not convertTo(getValue(countField), count)) {
            // This can never happen because we know that countField contains a Count.
            throw std::runtime_error(
                "The impossible happened: can't retrieve a PrefixedArray's item count.");
        }
        value->addValue(std::move(countField));

        // FIXME: this block's type can't be *this
        auto arrayValue = std::make_unique<BlockValue>(*this);
        importer.onEnterBlock(*this); // ... followed by yet another block
        for (Count i = 0; i < count; ++i) {
            arrayValue->addValue(
                getFields()[0].get().build(importer, path + "/" + std::to_string(i)));
        }
        importer.onExitBlock(*this);
        importer.onExitBlock(*this);

        value->addValue(std::move(arrayValue));
        return std::move(value);
    }

    Prefix mPrefix;
};
} // namespace structure
