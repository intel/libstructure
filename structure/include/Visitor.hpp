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

#include "type/StockTypes.hpp"
#include "structure_export.h"

#include <string>

namespace structure
{

class Block;
class GenericField;
class BlockValue;
class GenericFieldValue;
template <typename T>
class FieldValue;

/** @defgroup Visitors Visitors
 *
 * TODO: add detailed description.
 * @{
 */

/** Visits a Structure
 *
 * Implements the Visitor design pattern.
 */
class STRUCTURE_EXPORT StructureVisitor
{
public:
    virtual ~StructureVisitor() = default;

    virtual void visit(const Block &block) = 0;
    virtual void visit(const GenericField &field) = 0;

    /** Defaults to visit(const GenericField &) */
    virtual void visit(const Integer &i);
    /** Defaults to visit(const GenericField &) */
    virtual void visit(const FloatingPoint &f);
    /** Defaults to visit(const GenericField &) */
    virtual void visit(const FixedQ &q);
    /** Defaults to visit(const GenericField &) */
    virtual void visit(const String &s);
};

/** Visits a StructureValue
 *
 * Implements the Visitor design pattern.
 */
class STRUCTURE_EXPORT ValueVisitor
{
public:
    virtual ~ValueVisitor() = default;

    virtual void visit(const BlockValue &block) = 0;
    virtual void visit(const GenericFieldValue &field) = 0;
};

/** Visits actual values stored in StructureValue
 *
 * Implements the Visitor design pattern.
 *
 * Derived classes may simply implement the `BlockValue`, the `unsigned long long`, the `long long`
 * and the `long double` overloads of visitorStorage() because all the other defaults to one of the
 * latter three (signed integers (and `char`) default to `long long`; unsigned integers default to
 * `unsigned long long`; floating point types default to `long double`).
 */
class STRUCTURE_EXPORT StorageVisitor
{
public:
    virtual void visitStorage(const BlockValue &block);

    using ull = unsigned long long;
    using ll = long long;
    using ld = long double;
    virtual void visitStorage(unsigned char v) { visitStorage(ull{v}); }
    virtual void visitStorage(signed char v) { visitStorage(ll{v}); }
    virtual void visitStorage(char v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned short v) { visitStorage(ull{v}); }
    virtual void visitStorage(signed short v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned int v) { visitStorage(ull{v}); }
    virtual void visitStorage(int v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned long v) { visitStorage(ull{v}); }
    virtual void visitStorage(long v) { visitStorage(ll{v}); }
    virtual void visitStorage(float v) { visitStorage(ld{v}); }
    virtual void visitStorage(double v) { visitStorage(ld{v}); }

    virtual void visitStorage(ull) = 0;
    virtual void visitStorage(ll) = 0;
    virtual void visitStorage(ld) = 0;

    virtual void visitStorage(const std::string &) = 0;
};
/** @} */
}
