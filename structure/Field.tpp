#include "Field.hpp"
#include "Visitor.hpp"

namespace structure {

template <typename T>
Field<T>::Field(std::string name)
	: Structure(name)
{

}

template <typename T>
Field<T>::~Field()
{

}

template <typename T>
void Field<T>::accept(Visitor* visitor)
{
	visitor->visit(this);
}

}