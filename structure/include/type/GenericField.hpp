#pragma once

#include "structure_export.h"

#include "Structure.hpp"

namespace structure
{
/** Base class for all atomic field types. */
class STRUCTURE_EXPORT GenericField : public Structure
{
public:
    GenericField(std::string name) : Structure(name){};
    virtual ~GenericField() = default;
};
}
