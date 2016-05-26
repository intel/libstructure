#pragma once

#include "Structure.hpp"

#include <initializer_list>
#include <string>

namespace structure
{

class ValueVisitor;

class StructureValue
{
public:
    virtual ~StructureValue() = default;
    virtual void accept(ValueVisitor &visitor) const = 0;

    std::string getName() const;

    virtual const Structure &getStructure() const = 0;
};
}
