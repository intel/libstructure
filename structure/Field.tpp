#include "Field.hpp"
#include "Visitor.hpp"

namespace structure
{

template <typename T>
Field<T>::Field(std::string name) : Structure(name)
{
}

template <typename T>
Field<T>::~Field()
{
}

template <typename T>
void Field<T>::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

template <>
std::string Field<float>::fieldType();
template <>
std::string Field<int>::fieldType();
template <>
std::string Field<std::string>::fieldType();
}