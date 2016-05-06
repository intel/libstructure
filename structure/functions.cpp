#include "functions.hpp"
#include "Block.hpp"
#include "Field.hpp"
#include <iostream>

namespace structure
{

class DisplayVisitor : public Visitor
{
public:
    void visit(Block *block) override
    {
        std::cout << "Block : " << block->name() << std::endl;
        for (auto field : block->getFields())
            field->accept(this);
    }

    void visit(GenericField *field) override
    {
        std::cout << "Field : " << field->getName() << std::endl;
    }
};

void display(Structure &structure)
{
    DisplayVisitor display;
    structure.accept(&display);
}
}