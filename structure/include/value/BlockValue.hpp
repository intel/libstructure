#pragma once

#include "type/Block.hpp"
#include "value/StructureValue.hpp"
#include "Visitor.hpp"

#include <initializer_list>
#include <string>

namespace structure
{

class BlockValue : public StructureValue
{
public:
    BlockValue(const Block &block) : mStructure(block) {}

    void addValue(std::unique_ptr<StructureValue> child) { mValues.push_back(std::move(child)); }

    const std::list<std::unique_ptr<StructureValue>> &getFields() const { return mValues; }
    const Block &getBlock() const { return mStructure; }
    const Structure &getStructure() const override { return (const Structure &)mStructure; }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    virtual void accept(StorageVisitor &visitor) const override { visitor.visitStorage(*this); }

private:
    const Block &mStructure;
    std::list<std::unique_ptr<StructureValue>> mValues;
};
}
