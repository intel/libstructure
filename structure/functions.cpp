#include "functions.hpp"
#include "Block.hpp"
#include "Field.hpp"
#include "Exception.hpp"
#include <iostream>

namespace structure
{

class ApplyVisitor : public Visitor
{
public:
    ApplyVisitor(bool _recursive, BlockFunction _onEnterBlock, BlockFunction _onExitBlock,
                 FieldFunction _onEnterField)
        : recursive(_recursive), onEnterBlock(_onEnterBlock), onExitBlock(_onExitBlock),
          onEnterField(_onEnterField)
    {
    }

    void visit(Block &block) override
    {
        onEnterBlock(block);

        if (recursive)
            for (auto &field : block.getFields())
                field->accept(*this);

        onExitBlock(block);
    }

    void visit(GenericField &field) override { onEnterField(field); }

private:
    bool recursive;
    BlockFunction onEnterBlock;
    BlockFunction onExitBlock;
    FieldFunction onEnterField;
};

void apply(std::unique_ptr<Structure> &structure, BlockFunction onEnterBlock,
           BlockFunction onExitBlock, FieldFunction onEnterField, bool recursive)
{
    ApplyVisitor visitor(recursive, onEnterBlock, onExitBlock, onEnterField);
    structure.get()->accept(visitor);
}

// Functions

void display(std::unique_ptr<Structure> &structure)
{
    int level = 0;

    auto tab = [&]() {
        std::string tab;
        for (int i = 0; i < level; ++i)
            tab += "    ";
        return tab;
    };

    auto onEnterBlock = [&](auto &b) {
        std::cout << tab() << "Block : " << b.getName() << " {" << std::endl;
        level++;
    };

    auto onExitBlock = [&](auto &) {
        level--;
        std::cout << tab() << "}" << std::endl;
    };

    auto onEnterField = [&](auto &f) { std::cout << tab() << f.getName() << std::endl; };

    apply(structure, onEnterBlock, onExitBlock, onEnterField, true);
}

void addField(std::unique_ptr<Structure> &parent, std::unique_ptr<Structure> child)
{
    auto onEnterBlock = [&](auto &b) { b.addField(std::move(child)); };

    auto onEnterField = [&](auto &) { throw NotABlock(parent->getName()); };

    apply(parent, onEnterBlock, DefaultBlockFunction, onEnterField, false);
}

class GetChildVisitor : public Visitor
{
public:
    GetChildVisitor(std::string path) : path(path) {}

    void visit(Block &block) override
    {
        if (!path.length())
            return;

        if (path[0] == '/')
            path.erase(0, 1);

        auto idx = path.find_first_of("/");
        std::string name;

        if (idx != std::string::npos) {
            name = path.substr(0, idx);
            path = path.substr(idx + 1);
        } else {
            name = path;
            path = "";
        }

        for (auto &field : block.getFields()) {
            if (field->getName() == name) {
                if (path.empty()) {
                    result = &field;
                    return;
                } else {
                    field->accept(*this);
                }
            }
        }
    }

    void visit(GenericField &) override { result = nullptr; }

    std::string path;
    const std::unique_ptr<Structure> *result;
};

const std::unique_ptr<Structure> &getChild(std::unique_ptr<Structure> &structure, std::string path)
{
    GetChildVisitor visitor(path);
    structure.get()->accept(visitor);

    if (visitor.result) {
        return *(visitor.result);
    }
    throw ChildNotFound(structure->getName(), path);
}

// Builders

std::unique_ptr<Structure> makeBlock(std::string name)
{
    return std::make_unique<Block>(name);
}

std::unique_ptr<Structure> makeFloat(std::string name)
{
    return std::make_unique<Field<float>>(name);
}

std::unique_ptr<Structure> makeInteger(std::string name)
{
    return std::make_unique<Field<int>>(name);
}

std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1)
{
    auto b = makeBlock(name);
    addField(b, std::move(f1));
    return b;
}

std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2)
{
    auto b = makeBlock(name);
    addField(b, std::move(f1));
    addField(b, std::move(f2));
    return b;
}

std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2, std::unique_ptr<Structure> f3)
{
    auto b = makeBlock(name);
    addField(b, std::move(f1));
    addField(b, std::move(f2));
    addField(b, std::move(f3));
    return b;
}

std::unique_ptr<Structure> makeBlock(std::string name, std::unique_ptr<Structure> f1,
                                     std::unique_ptr<Structure> f2, std::unique_ptr<Structure> f3,
                                     std::unique_ptr<Structure> f4)
{
    auto b = makeBlock(name);
    addField(b, std::move(f1));
    addField(b, std::move(f2));
    addField(b, std::move(f3));
    addField(b, std::move(f4));
    return b;
}
}
