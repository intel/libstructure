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

#include "structure/type/Block.hpp"
#include "structure/value/StructureValue.hpp"
#include "structure/Visitor.hpp"

#include <initializer_list>
#include <string>
#include <list>

namespace structure
{
/** An instantiated Block */
class BlockValue : public StructureValue
{
public:
    BlockValue(const Block &block) : mStructure(block) {}

    /** Add a child value at the end of the block value */
    void addValue(std::unique_ptr<StructureValue> child) { mValues.push_back(std::move(child)); }

    /** @returns the list of children */
    const std::list<std::unique_ptr<StructureValue>> &getFields() const { return mValues; }
    /** @returns the Block type corresponding to this value
     *
     * Same as getStructure() but with a stronger type.
     */
    const Block &getBlock() const { return mStructure; }
    const Structure &getStructure() const override { return (const Structure &)mStructure; }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    virtual void accept(StorageVisitor &visitor) const override { visitor.visitStorage(*this); }

private:
    const Block &mStructure;
    std::list<std::unique_ptr<StructureValue>> mValues;
};
}
