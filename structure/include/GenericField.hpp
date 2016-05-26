#pragma once

#include "Structure.hpp"

namespace structure
{

class GenericField : public Structure
{
public:
    GenericField(std::string name) : Structure(name){};
    virtual ~GenericField() = default;
};
}
