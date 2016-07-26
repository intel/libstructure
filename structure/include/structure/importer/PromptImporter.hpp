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

#include "structure/ValueImporter.hpp"
#include "structure/type/GenericField.hpp"

#include <iostream>
#include <iomanip>

namespace structure
{

/** An implementation of a ValueImporter that prompt for the values
 *
 * @tparam     Input  : A class that must provide an overload of the operator >>(std::string&)
 * @tparam     Output : A class that must provide an overload of the operator <<(std::string&)
 *
 *
 * By default, the PromptImporter will use std::cin as input and std::cout as output, so
 * @code
 * PromptImporter<> importer();
 * @endcode
 * is equivalent to
 * @code
 * PromptImporter<std::istream, std::ostream> importer(std::cin, std::count);
 * @endcode
 *
 * @ingroup ValueImporter
 *
 */
template <class Input = std::istream, class Output = std::ostream>
class PromptImporter : public ValueImporter
{

public:
    /**
     * @param input The input stream from which to read user values.
     * @param output The output strem on which to prompt the user for values.
     * @param retry Whether to prompt the user anew when user input couldn't be importer.
     */
    PromptImporter(Input &input = std::cin, Output &output = std::cout, bool retry = true)
        : mInput(input), mOutput(output), mRetry(retry)
    {
    }

    std::unique_ptr<GenericFieldValue> import(const GenericField &f,
                                              const std::string &path) override
    {
        std::string val;
        do {
            mOutput << path << " = ";
            mInput >> std::quoted(val);
            try {
                return f.with(val);
            } catch (std::range_error &e) {
                if (mRetry) {
                    mOutput << std::string("Failed to import (") + e.what() + "). Try again."
                            << std::endl;
                } else {
                    throw e;
                }
            }
        } while (true);
    }

private:
    Input &mInput;
    Output &mOutput;

    bool mRetry;
};
} // namespace structure
