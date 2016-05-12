#pragma once

#include "GenericField.hpp"
#include "Visitor.hpp"

namespace structure
{

template <typename T>
class Field : public GenericField
{
public:
    Field(std::string name) : GenericField(name) {}

    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};
}
