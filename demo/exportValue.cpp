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
