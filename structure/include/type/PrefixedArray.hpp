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

#include "type/Block.hpp"
#include "Exception.hpp"
#include "functions.hpp"
#include "detail/disjunction.hpp"

#include <safe_cast.hpp>
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
    /** @todo Should this rely on the return type of Prefix::getValue() instead? */
    static_assert(std::is_integral<typename Prefix::Storage>::value,
                  "The type of a PrefixedArray's prefix must be an integer field");

public:
    template <typename ItemType, typename... Args>
    PrefixedArray(const std::string &name, ItemType &&itemType,
                  const std::string &prefixName = "count", Args &&... args)
        : Block(name, std::forward<ItemType>(itemType), std::forward<Args>(args)...),
          mPrefix(prefixName)
    {
        static_assert(not disjunction<is_structure<Args>...>::value,
                      "The arguments after ItemType must not be Structures");
    }

    std::string getTypeName() const override { return "LV (" + mPrefix.getTypeName() + ")"; }

private:
    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override
    {
        using Count = typename Prefix::Storage;
        auto value = std::make_unique<BlockValue>(*this);

        importer.onEnterBlock(path);                     // Enter the prefixed array block ...
        auto countField = mPrefix.build(importer, path); // ... starting with the prefix ...
        Count count = safe_cast<Count>(getValue(countField));
        value->addValue(std::move(countField));

        // FIXME: this block's type can't be *this
        auto arrayValue = std::make_unique<BlockValue>(*this);
        importer.onEnterBlock(path); // ... followed by yet another block
        for (Count i = 0; i < count; ++i) {
            arrayValue->addValue(
                getFields()[0].get().build(importer, path + "/" + std::to_string(i)));
        }
        importer.onExitBlock(path);
        importer.onExitBlock(path);

        value->addValue(std::move(arrayValue));
        return std::move(value);
    }

    Prefix mPrefix;
};
} // namespace structure
