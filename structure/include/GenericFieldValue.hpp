#pragma once

#include "structure_export.h"

#include "StructureValue.hpp"

namespace structure
{

class STRUCTURE_EXPORT GenericFieldValue : public StructureValue
{
public:
    virtual ~GenericFieldValue() = default;

    const virtual std::string getValue() const = 0;
};
}
