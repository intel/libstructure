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

/** @file
 *
 * Defines several exception classes for libstructure
 */

#include <stdexcept>
#include <string>

namespace structure
{
/** Base class for all libstructure exceptions */
class StructureException : public std::runtime_error
{
    using runtime_error::runtime_error;
};

/** Thrown when trying to access an unexisting child of a block */
class ChildNotFound : public StructureException
{
public:
    ChildNotFound(const std::string &self, const std::string &child)
        : StructureException("'" + self + "': no such child: '" + child + "'.")
    {
    }
};

/** Thrown when trying to get a Field as a Block */
class NotABlock : public StructureException
{
public:
    NotABlock(const std::string &self) : StructureException("'" + self + "' is not a Block.") {}
};

/** Thrown when instantiating a StructureValue with an incorrect layout */
class ValueStructureMismatch : public StructureException
{
public:
    ValueStructureMismatch(const std::string &self, const std::string &details)
        : StructureException("Instanciation values do not match with the Structure '" + self +
                             "' (" + details + ").")
    {
    }
};

/** Thrown when a user input cannot be converted into the required type */
class ParseError : public StructureException
{
public:
    // TODO: make it possible to pass a type name
    ParseError(const std::string &input)
        : StructureException("Incompatible value \"" + input + "\".")
    {
    }
};

/** Thrown when a ValueInitializer runs out of values */
class NotEnoughValues : public ValueStructureMismatch
{
public:
    NotEnoughValues(const std::string &self)
        : ValueStructureMismatch(self, "Not enough values in initializer.")
    {
    }
};

/** Thrown when a field does not have a Default attributes and no value was provided */
class NoDefaultValue : public StructureException
{
public:
    NoDefaultValue(const std::string &self)
        : StructureException("'" + self + "' does not have a Default attribute")
    {
    }
};
}
