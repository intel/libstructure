#pragma once

#include "structure_export.h"

#include "StructureValue.hpp"

namespace structure
{
/** The base (interface) class for all atomic values (i.e. all kinds of FieldValue) */
class STRUCTURE_EXPORT GenericFieldValue : public StructureValue
{
public:
    /** @return the human representation of the value */
    virtual std::string getValue() const = 0;
};
}
