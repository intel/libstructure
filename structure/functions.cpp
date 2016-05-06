#include "functions.hpp"
#include "Block.hpp"
#include "Field.hpp"
#include <iostream>

namespace structure
{

// display

class DisplayVisitor : public Visitor
{
public:
    void visit(Block *block) override
    {
        std::cout << tab() << "Block : " << block->name() << " {" << std::endl;
        level++;
        for (auto field : block->getFields())
            field->accept(this);
        level--;
        std::cout << tab() << "}" << std::endl;
    }

    void visit(GenericField *field) override
    {
        std::cout << tab() << "Field : " << field->getName() << std::endl;
    }

private:
    std::string tab()
    {
        std::string tab;
        for (int i = 0; i < level; ++i)
            tab += "    ";
        return tab;
    }

    int level = 0;
};

void display(Structure *structure)
{
    if (!structure)
        return;

    DisplayVisitor visitor;
    structure->accept(&visitor);
}

// getChild

class GetChildVisitor : public Visitor
{
public:
    GetChildVisitor(std::string _path)
    {
        path = _path;
        result = nullptr;
    }

    void visit(Block *block) override
    {
        if (!path.length())
            return;

        if (path[0] == '/')
            path.erase(0, 1);

        int idx = path.find_first_of("/");
        std::string name;

        if (idx != std::string::npos) {
            name = path.substr(0, idx);
            path = path.substr(idx + 1);
        } else {
            name = path;
            path = "";
        }

        for (auto field : block->getFields()) {
            if (field->name() == name) {
                if (path.empty()) {
                    result = field;
                    return;
                } else {
                    field->accept(this);
                }
            }
        }
    }

    void visit(GenericField *field) override { result = nullptr; }

    std::string path;
    Structure *result;
};

Structure *getChild(Structure *structure, std::string path)
{
    if (!structure)
        return nullptr;

    GetChildVisitor visitor(path);
    structure->accept(&visitor);
    return visitor.result;
}

// getChildren

class GetChildrenVisitor : public Visitor
{
public:
    void visit(Block *block) override { result = block->getFields(); }

    void visit(GenericField *field) override {}

    std::list<Structure *> result;
};

std::list<Structure *> getChildren(Structure *structure)
{
    if (!structure)
        return std::list<Structure *>();

    GetChildrenVisitor visitor;
    structure->accept(&visitor);
    return visitor.result;
}
}