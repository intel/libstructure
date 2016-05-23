#pragma once

#include <initializer_list>
#include <string>

namespace structure
{

class Structure;
class ValueVisitor;

class StructureValue
{
public:
    explicit StructureValue(const Structure &structure) : mStructure(structure) {}

    virtual ~StructureValue() = default;
    virtual void accept(ValueVisitor &visitor) const = 0;

    std::string getName() const;

    const Structure &getStructure() const;

private:
    const Structure &mStructure;
};
}
