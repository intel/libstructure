#pragma once

#include "structure_export.h"

#include "Structure.hpp"

namespace structure
{

class STRUCTURE_EXPORT GenericField : public Structure
{
public:
    GenericField(std::string name) : Structure(name){};
    virtual ~GenericField() = default;
};
}
