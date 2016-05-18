#pragma once

#include "Block.hpp"
#include "StructureValue.hpp"
#include "Visitor.hpp"

#include <initializer_list>
#include <string>

namespace structure
{

class BlockValue : public StructureValue
{
public:
    BlockValue(const Block &structure) : StructureValue(structure) {}

    const std::string getValue() const override { return "block value"; }

    void addValue(std::unique_ptr<StructureValue> child) { mValues.push_back(std::move(child)); }

    const std::list<std::unique_ptr<StructureValue>> &getFields() const { return mValues; }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }

private:
    std::list<std::unique_ptr<StructureValue>> mValues;
};
}
