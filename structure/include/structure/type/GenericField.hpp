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

#include "structure/type/Structure.hpp"
#include "structure/Exception.hpp"
#include "structure/value/GenericFieldValue.hpp"
#include "structure/attributes/Default.hpp"

namespace structure
{

struct STRUCTURE_EXPORT GenericFieldAttributes : StructureAttributes
{
    void set(const attributes::Default &def)
    {
        mDefaultImporter = def.mDefaultImporter;
        mHasDefaultImporter = true;
    }

    template <typename C>
    void set(const C &c)
    {
        StructureAttributes::set(c);
    }

    bool mHasDefaultImporter = false;
    std::shared_ptr<ValueImporter> mDefaultImporter;
};

/** Base class for all atomic field types. */
class STRUCTURE_EXPORT GenericField : public Structure
{
public:
    using Structure::Structure;
    virtual ~GenericField() = default;

    /** Creates a value from the given string
     */
    virtual std::unique_ptr<GenericFieldValue> with(const std::string &value) const = 0;

    /** Creates a value from the given typed value
    */
    template <typename T,
              typename = typename std::enable_if<not std::is_same<T, std::string>::value and
                                                 not std::is_same<T, const char *>::value>::type>
    std::unique_ptr<GenericFieldValue> with(T value) const
    {
        return withTyped(value);
    }

    virtual ValueImporter &getDefaultImporter() const = 0;
    virtual bool hasDefaultImporter() const = 0;

private:
    /** Creates a value from the given bool
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(bool) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given long long
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(long long) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given unsigned long long
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(unsigned long long) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given float
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(float) const
    {
        throw ParseError(getName());
    }
    /** Creates a value from the given double
     */
    virtual std::unique_ptr<GenericFieldValue> withTyped(double) const
    {
        throw ParseError(getName());
    }
};
}
