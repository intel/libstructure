#pragma once

#include "type/StockTypes.hpp"
#include "structure_export.h"

namespace structure
{

class Block;
class GenericField;
class BlockValue;
class GenericFieldValue;
template <typename T>
class FieldValue;

class STRUCTURE_EXPORT StructureVisitor
{
public:
    virtual ~StructureVisitor() = default;

    virtual void visit(const Block &block) = 0;
    virtual void visit(const GenericField &field) = 0;

    virtual void visit(const Integer &i);
    virtual void visit(const FloatingPoint &f);
};

class STRUCTURE_EXPORT ValueVisitor
{
public:
    virtual ~ValueVisitor() = default;

    virtual void visit(const BlockValue &block) = 0;
    virtual void visit(const GenericFieldValue &field) = 0;
};

class STRUCTURE_EXPORT StorageVisitor
{
public:
    virtual void visitStorage(const BlockValue &block);

    using ull = unsigned long long;
    using ll = long long;
    using ld = long double;
    virtual void visitStorage(unsigned char v) { visitStorage(ull{v}); }
    virtual void visitStorage(signed char v) { visitStorage(ll{v}); }
    virtual void visitStorage(char v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned short v) { visitStorage(ull{v}); }
    virtual void visitStorage(signed short v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned int v) { visitStorage(ull{v}); }
    virtual void visitStorage(int v) { visitStorage(ll{v}); }
    virtual void visitStorage(unsigned long v) { visitStorage(ull{v}); }
    virtual void visitStorage(long v) { visitStorage(ll{v}); }
    virtual void visitStorage(float v) { visitStorage(ld{v}); }
    virtual void visitStorage(double v) { visitStorage(ld{v}); }

    virtual void visitStorage(ull) = 0;
    virtual void visitStorage(ll) = 0;
    virtual void visitStorage(ld) = 0;
};
}
