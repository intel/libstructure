#include "Field.hpp"

namespace structure
{

template <>
std::string Field<float>::fieldType()
{
    return "Float";
}
template <>
std::string Field<int>::fieldType()
{
    return "Integer";
}
template <>
std::string Field<std::string>::fieldType()
{
    return "String";
}
}