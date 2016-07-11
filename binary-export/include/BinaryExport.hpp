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

#include "structure/Visitor.hpp"

#include <vector>
#include <iterator>
#include <algorithm>

#ifdef _MSC_VER
/** Visual studio raises a warning if the check iterator feature is activated
 * but a raw pointer is used as iterator (as it can not check it's bounds).
 * As it is a safety feature, do not silent the warning, but use the
 * microsoft specific `make_check_array_iterator` that take a pointer
 * and the size of the underline buffer.
 * For other compiler, use the raw pointer.
 */
#define MAKE_ARRAY_ITERATOR(begin, size) stdext::make_checked_array_iterator(begin, size)
#else
/** By default an array iterator is a pointer to the first element. */
#define MAKE_ARRAY_ITERATOR(begin, size) begin
#endif

namespace binary_export
{
/** @example binary-export.cpp
 * This show how to use the binary export.
 */
/** A visitor for binary export.
 *
 * Builtin C types are memcopied to the output. std::strings are exported as C strings. Everything
 * is packed.
 */
class Visitor : public structure::StorageVisitor
{
public:
    using Output = std::vector<unsigned char>;
    Visitor(Output &out) : mOut(out) {}

    void visitStorage(unsigned char v) override { visitStorageT(v); }
    void visitStorage(signed char v) override { visitStorageT(v); }
    void visitStorage(char v) override { visitStorageT(v); }
    void visitStorage(unsigned short v) override { visitStorageT(v); }
    void visitStorage(signed short v) override { visitStorageT(v); }
    void visitStorage(unsigned int v) override { visitStorageT(v); }
    void visitStorage(int v) override { visitStorageT(v); }
    void visitStorage(unsigned long v) override { visitStorageT(v); }
    void visitStorage(long v) override { visitStorageT(v); }
    void visitStorage(float v) override { visitStorageT(v); }
    void visitStorage(double v) override { visitStorageT(v); }
    void visitStorage(unsigned long long v) override { visitStorageT(v); }
    void visitStorage(long long v) override { visitStorageT(v); }
    void visitStorage(long double v) override { visitStorageT(v); }

    void visitStorage(const std::string &v) override
    {
        std::copy_n(v.c_str(), v.size() + 1, std::back_inserter(mOut));
    }

private:
    template <class Storage>
    void visitStorageT(Storage v)
    {
        auto type_erased = reinterpret_cast<const Output::value_type *>(&v);
        auto first = MAKE_ARRAY_ITERATOR(type_erased, sizeof(v));
        std::copy(first, first + sizeof(v), std::back_inserter(mOut));
    }

    Output &mOut;
};

void write(Visitor::Output &out, const structure::StructureValue &value)
{
    Visitor visitor(out);
    value.accept(visitor);
}
} // namespace binary_export
