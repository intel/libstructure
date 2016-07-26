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
#pragma once

#include "structure/structure_export.h"
#include "structure/ValueImporter.hpp"
#include "structure/ValueInitializer.hpp"

#include <initializer_list>
#include <list>
#include <memory>

namespace structure
{
/** Allows ValueInitializer to be created using recursive initializer lists
 *
 * Thanks to this class, a ValueInitializer can contain an arbitrary level of nested blocks.
 */
class STRUCTURE_EXPORT ListImporter : public ValueImporter
{
public:
    /** @see ValueInitializer(std::initializer_list<ValueInitializer> */
    ListImporter(std::initializer_list<ValueInitializer> list);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;

    void onEnterBlock(const std::string &block) override;
    void onExitBlock(const std::string &block) override;

private:
    // This class needs to be copiable because importers will be copied from initializer_list to
    // list when they are part of a ListImporter. (Copied and not moved because items of a
    // initializer_list can't be moved). That's why we use a shared_ptr.
    std::list<std::shared_ptr<ValueImporter>> mImporters;
    using Iterator = decltype(mImporters)::const_iterator;

    Iterator iterator;
    size_t mDepth = 0;
};
}
