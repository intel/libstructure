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

#include "type/Structure.hpp"
#include "Visitor.hpp"

#include <sstream>
#include <list>

template <typename T>
struct ExportTraits
{
    static const std::string typeName;
};

class PfwStructureVisitor : public structure::StructureVisitor
{
public:
    void visit(const structure::Block &block) override;
    void visit(const structure::GenericField &field) override;
    void visit(const structure::Integer &i) override;
    void visit(const structure::FloatingPoint &f) override;
    void visit(const structure::FixedQ &i) override;

    std::list<std::string> mInstances, mComponents;

private:
    bool isRoot() { return mLevel == 1; }
    std::ostringstream mCurrent;
    int mLevel{1};
};

std::string exportStructure(const structure::Structure &structure);

class PfwValueVisitor : public structure::ValueVisitor
{
public:
    void visit(const structure::BlockValue &block) override;
    void visit(const structure::GenericFieldValue &field) override;

    std::ostringstream mResult;

private:
    int mLevel{0};
};

std::string exportValue(const structure::StructureValue &value);
