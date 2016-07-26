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
#include "export.hpp"
#include "structure/functions.hpp"
#include "structure/type/stock.hpp"

#include <sstream>
#include <string>
#include <ios>

using namespace structure;

void exportStructure(const Structure &structure, std::list<std::string> &components,
                     std::list<std::string> &instances);

static auto tab = [](int level) { return std::string(4 * level, ' '); };

void PfwStructureVisitor::visit(const Block &block)
{
    std::string tagName(isRoot() ? "ComponentType" : "ParameterBlock");
    mCurrent << tab(mLevel) << "<" << tagName << " Name=\"" << block.getName() << "\">\n";

    mLevel++;
    for (const auto &field : block.getFields()) {
        field.get().accept(*this);
    }
    mLevel--;

    mCurrent << tab(mLevel) << "</" << tagName << ">\n";

    if (isRoot()) {
        // root component
        mComponents.push_back(mCurrent.str());
        mCurrent.clear();

        mInstances.push_back(tab(mLevel) + "<Component Name=\"" + block.getName() + "\" Type=\"" +
                             block.getName() + "\"/>");
    }
}

void PfwStructureVisitor::visit(const GenericField &)
{
    throw std::runtime_error("Unknown type");
}

void PfwStructureVisitor::visit(const FixedQ &q)
{
    if (not q.getSignedness()) {
        throw std::runtime_error("The Parameter Framework does not support unsigned Q numbers.");
    }

    mCurrent << tab(mLevel) << "<FixedPointParameter Name=\"" + q.getName() + "\" Size=\""
             << q.getSize() << "\" Integral=\"" << q.getIntegral() << "\" Fractional=\""
             << q.getFractional() << "\"/>\n";
}

void PfwStructureVisitor::visit(const Integer &i)
{
    mCurrent << tab(mLevel) << "<IntegerParameter Name=\"" + i.getName() + "\" Size=\""
             << i.getSize() << "\" Signed=\"" << std::boolalpha << i.getSignedness() << "\"/>\n";
}

void PfwStructureVisitor::visit(const FloatingPoint &f)
{
    mCurrent << tab(mLevel)
             << "<FloatingPointParameter Name=\"" + f.getName() + "\" Size=\"32\"/>\n";
}

void exportStructure(const Structure &structure, std::list<std::string> &components,
                     std::list<std::string> &instances)
{
    PfwStructureVisitor visitor;

    structure.accept(visitor);

    instances = std::move(visitor.mInstances);
    components = std::move(visitor.mComponents);
}

std::string exportStructure(const Structure &structure)
{
    std::stringstream result;

    std::list<std::string> instances, components;

    exportStructure(structure, components, instances);

    result << "<Subsystem xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Name=\"Example\" "
              "Type=\"Virtual\">\n";

    result << "<ComponentLibrary>\n\n";

    for (auto &c : components) {
        result << c << "\n";
    }
    result << "</ComponentLibrary>\n\n";

    result << "<InstanceDefinition>\n";

    for (auto &i : instances) {
        result << i << "\n";
    }

    result << "</InstanceDefinition>\n";
    result << "</Subsystem>";

    return result.str();
}
