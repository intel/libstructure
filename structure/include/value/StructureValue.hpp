#pragma once

#include "structure_export.h"

#include "type/Structure.hpp"

#include <initializer_list>
#include <string>

namespace structure
{

class ValueVisitor;
class StorageVisitor;
/** The base class for all values (atomic and aggregates) */
class STRUCTURE_EXPORT StructureValue
{
public:
    // Needed as StructureValue is the root class of a hierarchy
    virtual ~StructureValue() = default;

    /** Implements the Visitor design pattern */
    virtual void accept(ValueVisitor &visitor) const = 0;
    /** Implements the Visitor design pattern
     *
     * This overload visits the actual in-memory values wheras the ValueVisitor overload visits
     * polymorphic values.
     */
    virtual void accept(StorageVisitor &visitor) const = 0;

    std::string getName() const;

    /** @returns the Structure with which this value was instantiated */
    virtual const Structure &getStructure() const = 0;
};
}
