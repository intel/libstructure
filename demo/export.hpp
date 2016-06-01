#pragma once

#include "type/Structure.hpp"
#include "Visitor.hpp"

#include <sstream>

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
