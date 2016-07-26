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
#include "structure/importer/ListImporter.hpp"
#include "structure/type/GenericField.hpp"
#include "structure/value/GenericFieldValue.hpp"
#include "structure/type/Block.hpp"

#include <cassert>

namespace structure
{
ListImporter::ListImporter(std::initializer_list<ValueInitializer> list)
{
    for (auto &value : list) {
        mImporters.emplace_back(new ValueInitializer(value));
    }
    iterator = begin(mImporters);
}

std::unique_ptr<GenericFieldValue> ListImporter::import(const GenericField &field,
                                                        const std::string &path)
{
    if (mDepth == 0) {
        // depth == 0 means that we haven't entered this block yet
        throw WrongType("Atom", "Block");
    }
    if (iterator == end(mImporters)) {
        // We have already reached the end of this block. There's no field left to import.
        throw NotEnoughValues(field.getName());
    }

    auto result = iterator->get()->import(field, path);
    if (mDepth == 1) {
        // depth > 1 means that we are importing fields that are more deeply nested; as a result we
        // must only increment this block's iterator when it is the active one.
        ++iterator;
    }
    return result;
}

void ListImporter::onEnterBlock(const std::string &block) try {
    if (begin(mImporters) == end(mImporters)) {
        // Special case for empty blocks
        ++mDepth;
        return;
    }
    if (iterator == end(mImporters)) {
        // we have already reached the end of this block. There's no block left to import.
        throw NotEnoughValues(block);
    }

    ++mDepth;
    if (mDepth > 1) {
        // depth > 1 means that we are importing fields that are more deeply nested; as a result we
        // must forward the onEnterBlock event.
        iterator->get()->onEnterBlock(block);
    }
} catch (ValueInitializer::WrongType &ex) {
    throw ValueStructureMismatch(block, "Expected : \"" + ex.required + "\", Actual : \"" +
                                            ex.actual + "\".");
}

void ListImporter::onExitBlock(const std::string &block)
{
    // This would be caused by a programming error in the Block class (or a descendant) calling
    // this method.
    assert(mDepth > 0);
    --mDepth;

    if ((iterator != end(mImporters)) and (mDepth > 1)) {
        iterator->get()->onExitBlock(block);
    }

    if (mDepth == 1) {
        // We are done importing a direct child, get ready to import its next sibling
        ++iterator;
    }

    if ((mDepth == 0) and (iterator != end(mImporters))) {
        throw ValueStructureMismatch(block, "Exiting a Block with leftover values");
    }
}
} // namespace structure
