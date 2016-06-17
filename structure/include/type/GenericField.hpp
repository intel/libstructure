#pragma once

#include "structure_export.h"

#include "Structure.hpp"

namespace structure
{

class GenericFieldValue;

/** Base class for all atomic field types. */
class STRUCTURE_EXPORT GenericField : public Structure
{
public:
    GenericField(std::string name) : Structure(name){};
    virtual ~GenericField() = default;

    /** Creates a value from the given string
     */
    virtual std::unique_ptr<GenericFieldValue> with(const std::string &value) const = 0;
};
}
