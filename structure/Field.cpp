#include "Field.hpp"
#include "StockTypes.hpp"

namespace structure
{
template <>
const std::string Float::typeName = "Float";
template <>
const std::string Integer::typeName = "Integer";
}
