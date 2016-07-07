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
#include "export.hpp"
#include "functions.hpp"
#include "client/type.hpp"
#include "client/stock.hpp"
#include <string>

using namespace structure;

class PfwTagVisitor : public structure::StructureVisitor
{
public:
    PfwTagVisitor(std::ostringstream &out, bool open) : mResult(out), mOpen(open) {}

    void visit(const structure::Block &) override
    {
        // This could never happen because it has been taken care of by the structure visitor.
        throw std::runtime_error("Trying to get a block's atomic value, which makes no sense.");
    }
    void visit(const structure::GenericField &) override
    {
        throw std::runtime_error("Trying to serialize an unsupported type.");
    }
    void visit(const structure::Integer &i) override
    {
        if (mOpen) {
            mResult << "<IntegerParameter Name=\"" + i.getName() + "\" Size=\"" << i.getSize()
                    << "\" Signed=\"" << std::boolalpha << i.getSignedness() << "\">";
        } else {
            mResult << "</IntegerParameter>";
        }
    }

    void visit(const structure::FloatingPoint &f) override
    {
        if (mOpen) {
            mResult << "<FloatingPointParameter Name=\"" + f.getName() + "\" Size=\"32\">";
        } else {
            mResult << "</FloatingPointParameter>";
        }
    }

    void visit(const structure::FixedQ &q) override
    {
        if (not q.getSignedness()) {
            throw std::runtime_error(
                "The Parameter Framework does not support unsigned Q numbers.");
        }

        if (mOpen) {
            mResult << "<FixedPointParameter Name=\"" + q.getName() + "\" Size=\"" << q.getSize()
                    << " Integral=\"" << q.getIntegral() << "\" Fractional=\"" << q.getFractional()
                    << "\">";
        } else {
            mResult << "</FixedPointParameter>";
        }
    }

    std::ostringstream &mResult;
    bool mOpen;
};

std::string exportValue(const StructureValue &value)
{
    PfwValueVisitor visitor;

    value.accept(visitor);

    return visitor.mResult.str();
}

static auto tab = [](int level) { return std::string(4 * level, ' '); };

void PfwValueVisitor::visit(const BlockValue &block)
{
    mResult << tab(mLevel) << "<ParameterBlock Name=\"" << block.getName() << "\">\n";

    mLevel++;
    for (const auto &field : block.getFields()) {
        field->accept(*this);
    }
    mLevel--;

    mResult << tab(mLevel) << "</ParameterBlock>\n";
}

void pushTag(std::ostringstream &out, bool open, const Structure &type)
{
    PfwTagVisitor tagVisitor(out, open);
    type.accept(tagVisitor);
}

void PfwValueVisitor::visit(const GenericFieldValue &value)
{
    mResult << tab(mLevel);
    pushTag(mResult, true, value.getStructure());
    mResult << value.getValue();
    pushTag(mResult, false, value.getStructure());
    mResult << std::endl;
}
