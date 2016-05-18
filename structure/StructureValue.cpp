#include "StructureValue.hpp"
#include "Structure.hpp"

namespace structure
{

std::string StructureValue::getName() const
{
    return mStructure.getName();
}

const Structure &StructureValue::getStructure() const
{
    return mStructure;
}
}