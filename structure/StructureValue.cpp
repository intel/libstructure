#include "StructureValue.hpp"
#include "Structure.hpp"

namespace structure
{

std::string StructureValue::getName() const
{
    return getStructure().getName();
}
}
