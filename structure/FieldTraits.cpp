#include "Field.hpp"
#include "FieldTraits.hpp"

namespace structure
{
template <>
const std::string FieldTraits<Float>::name{"Float"};
template <>
const std::string FieldTraits<Integer>::name{"Integer"};
}
