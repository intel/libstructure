#pragma once

#include "StockTypes.hpp"
#include "structure_export.h"

namespace structure
{

class Block;
class GenericField;
class BlockValue;
class GenericFieldValue;

class STRUCTURE_EXPORT StructureVisitor
{
public:
    virtual ~StructureVisitor() = default;

    virtual void visit(const Block &block) = 0;
    virtual void visit(const GenericField &field) = 0;

    virtual void visit(const Integer &i);
    virtual void visit(const Float &f);
};

class STRUCTURE_EXPORT ValueVisitor
{
public:
    virtual ~ValueVisitor() = default;

    virtual void visit(const BlockValue &block) = 0;
    virtual void visit(const GenericFieldValue &field) = 0;
    // TODO: overloads for stock value types
};
}
