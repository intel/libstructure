#pragma once

#include "StructureValue.hpp"

namespace structure
{

class GenericFieldValue : public StructureValue
{
public:
    GenericFieldValue(const Structure &structure) : StructureValue(structure){};
    virtual ~GenericFieldValue() = default;
};
}
