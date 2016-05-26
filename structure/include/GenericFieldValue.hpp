#pragma once

#include "StructureValue.hpp"

namespace structure
{

class GenericFieldValue : public StructureValue
{
public:
    virtual ~GenericFieldValue() = default;

    const virtual std::string getValue() const = 0;
};
}
