#include "value/StructureValue.hpp"
#include "type/Structure.hpp"

namespace structure
{

std::string StructureValue::getName() const
{
    return getStructure().getName();
}
}
