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

#include "structure/structure_export.h"
#include "structure/Exception.hpp"

#include <string>
#include <memory>

namespace structure
{

class GenericField;
class StructureValue;
class GenericFieldValue;

/**
 * @defgroup ValueImporter Allow to create StructureValue from Structure and formatted values
 */

/** An abstract class that represent a value importer
 *
 * @ingroup ValueImporter
 */
class STRUCTURE_EXPORT ValueImporter
{
public:
    struct WrongType : StructureException
    {
        WrongType(const std::string &required, const std::string &actual)
            : StructureException("Required : \"" + required + "\", Actual : \"" + actual + "\"."),
              required(required), actual(actual)
        {
        }

        const std::string required;
        const std::string actual;
    };

    virtual ~ValueImporter() = default;

    /**
     * @param[in]  field : The structure of the value beign built
     * @param[in]  path  : The path of the field in the structure
     *
     * @return     The value coresponding to the field
     */
    virtual std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                                      const std::string &path) = 0;

    // We need to write the name of the parameter in the declaration, so that Doxygen will know it
    // but in the same time, that would trigger an 'unused parameter' warning.
    /** Notify the importer that the caller wants to begin importing a sub-block
     *
     * @param[in] block The path of the block being imported
     */
    virtual void onEnterBlock(const std::string &block) { (void)block; }
    /** Notify the importer that the caller is done importing a sub-block
     *
     * @param[in] block The path of the block being imported
     */
    virtual void onExitBlock(const std::string &block) { (void)block; }
};
}
