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

#include "type/Structure.hpp"
#include "value/BlockValue.hpp"
#include "value/StructureValue.hpp"
#include "ValueImporter.hpp"
#include "ValueInitializer.hpp"

#include <vector>
#include <utility>
#include <memory>

namespace structure
{

/** Represents an aggregate. */
class STRUCTURE_EXPORT Block : public Structure
{
public:
    /** Constructs a block with any number and kinds of fields.
     *
     * In the context of this class, "field" may refer to both atomic fields and aggregates
     * (blocks).
     *
     * @param[in] fields The fields to be included in the new block.
     */
    template <typename... Fields>
    Block(std::string name, Fields &&... fields) : Structure(name)
    {
        addFields(std::forward<Fields>(fields)...);
    }

    Block(Block &&other) = default;
    Block &operator=(Block &&other) = default;

    void accept(StructureVisitor &visitor) const override;

    /** Helper type for the getFields() return type */
    using StructureRef = std::reference_wrapper<const Structure>;
    /** @returns const handles to the fields contained in the block */
    virtual std::vector<StructureRef> getFields() const;

    /** Create a BlockValue of the block's type. */
    std::unique_ptr<StructureValue> with(ValueInitializer initializer) const;

    std::string getTypeName() const override { return "Block"; }
    std::string getDescription() const override { return ""; }

protected:
    template <typename T, typename... Fields>
    void addFields(T &&first, Fields &&... fields)
    {
        addField(std::forward<T>(first));
        addFields(std::forward<Fields>(fields)...);
    }

private:
    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override;

    template <class T>
    void addField(T &&child)
    {
        // This prevents hard-to-understand compilation errors.
        static_assert(is_structure<T>::value, "The field to be added must be a Structure.");
        mFields.emplace_back(new T(std::forward<T>(child)));
    }

    void addFields(){};

    std::vector<std::unique_ptr<Structure>> mFields;
};
}
