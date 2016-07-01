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

#include "ValueImporter.hpp"
#include "type/GenericField.hpp"

#include <iostream>

namespace structure
{

/** An implementation of a ValueImporter that gets its values from a stream
 *
 * @tparam     Input : A class that must provide an overload of the operator >>(std::string&)
 *
 * Example of custom class :
 * @code
 * class MyStream
 * {
 * public:
 *     MyStream &operator>>(std::string &s)
 *     {
 *         s = "42";
 *         return *this;
 *     }
 * };
 * @endcode
 *
 * By default, the StreamImporter will use std::cin, so
 * @code
 * StreamImporter<> importer();
 * @endcode
 * is equivalent to
 * @code
 * StreamImporter<std::istream> importer(std::cin);
 * @endcode
 *
 * @ingroup ValueImporter
 *
 */
template <class Input = std::istream>
class StreamImporter : public ValueImporter
{

public:
    StreamImporter(Input &input = std::cin) : mInput(input) {}

    std::unique_ptr<GenericFieldValue> import(const GenericField &f,
                                              const std::string & /*path*/) override
    {
        std::string val;
        mInput >> std::quoted(val);
        return f.with(val);
    }

private:
    Input &mInput;
};
} // namespace structure