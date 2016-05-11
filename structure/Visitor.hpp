#pragma once

#include "GenericField.hpp"
#include "Block.hpp"

namespace structure
{

class Visitor
{
public:
    virtual ~Visitor() = default;

    virtual void visit(Block &block) = 0;
    virtual void visit(GenericField &field) = 0;
};
}
