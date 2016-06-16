#pragma once

#include "type/Block.hpp"
#include "value/StructureValue.hpp"
#include "Visitor.hpp"

#include <initializer_list>
#include <string>
#include <list>

namespace structure
{
/** An instantiated Block */
class BlockValue : public StructureValue
{
public:
    BlockValue(const Block &block) : mStructure(block) {}

    /** Add a child value at the end of the block value */
    void addValue(std::unique_ptr<StructureValue> child) { mValues.push_back(std::move(child)); }

    /** @returns the list of children */
    const std::list<std::unique_ptr<StructureValue>> &getFields() const { return mValues; }
    /** @returns the Block type corresponding to this value
     *
     * Same as getStructure() but with a stronger type.
     */
    const Block &getBlock() const { return mStructure; }
    const Structure &getStructure() const override { return (const Structure &)mStructure; }

    virtual void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    virtual void accept(StorageVisitor &visitor) const override { visitor.visitStorage(*this); }

private:
    const Block &mStructure;
    std::list<std::unique_ptr<StructureValue>> mValues;
};
}
