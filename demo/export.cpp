#include "export.hpp"
#include "functions.hpp"
#include "type/Field.hpp"
#include "type/StockTypes.hpp"
#include "type/Integer.hpp"
#include "type/FloatingPoint.hpp"
#include "type/FixedQ.hpp"

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
        field->accept(*this);
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
             << q.getSize() << " Integral=\"" << q.getIntegral() << "\" Fractional=\""
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
