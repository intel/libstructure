#include "functions.hpp"
#include "Block.hpp"
#include "Field.hpp"
#include "FieldValue.hpp"
#include "BlockValue.hpp"
#include "Exception.hpp"
#include <iostream>

namespace structure
{

class ApplyStructureVisitor : public StructureVisitor
{
public:
    ApplyStructureVisitor(bool _recursive, BlockFunction _onEnterBlock, BlockFunction _onExitBlock,
                          FieldFunction _onEnterField)
        : recursive(_recursive), onEnterBlock(_onEnterBlock), onExitBlock(_onExitBlock),
          onEnterField(_onEnterField)
    {
    }

    void visit(const Block &block) override
    {
        onEnterBlock(block);

        if (recursive)
            for (const auto &field : block.getFields())
                field->accept(*this);

        onExitBlock(block);
    }

    void visit(const GenericField &field) override { onEnterField(field); }

private:
    bool recursive;
    BlockFunction onEnterBlock;
    BlockFunction onExitBlock;
    FieldFunction onEnterField;
};

void apply(const Structure &structure, BlockFunction onEnterBlock, BlockFunction onExitBlock,
           FieldFunction onEnterField, bool recursive)
{
    ApplyStructureVisitor visitor(recursive, onEnterBlock, onExitBlock, onEnterField);
    structure.accept(visitor);
}

class ApplyValueVisitor : public ValueVisitor
{
public:
    ApplyValueVisitor(bool _recursive, BlockValueFunction _onEnterBlock,
                      BlockValueFunction _onExitBlock, FieldValueFunction _onEnterField)
        : recursive(_recursive), onEnterBlock(_onEnterBlock), onExitBlock(_onExitBlock),
          onEnterField(_onEnterField)
    {
    }

    void visit(const BlockValue &block) override
    {
        onEnterBlock(block);

        if (recursive)
            for (const auto &field : block.getFields())
                field->accept(*this);

        onExitBlock(block);
    }

    void visit(const GenericFieldValue &field) override { onEnterField(field); }

private:
    bool recursive;
    BlockValueFunction onEnterBlock;
    BlockValueFunction onExitBlock;
    FieldValueFunction onEnterField;
};

void apply(const StructureValue &structure, BlockValueFunction onEnterBlock,
           BlockValueFunction onExitBlock, FieldValueFunction onEnterField, bool recursive)
{
    ApplyValueVisitor visitor(recursive, onEnterBlock, onExitBlock, onEnterField);
    structure.accept(visitor);
}

// Functions

void display(const Structure &structure)
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

    auto onEnterField = [&](auto &f) {
        std::cout << tab() << "Field : " << f.getName() << std::endl;
    };

    apply(structure, onEnterBlock, onExitBlock, onEnterField, true);
}

void display(const StructureValue &value)
{
    int level = 0;

    auto tab = [&]() {
        std::string tab;
        for (int i = 0; i < level; ++i)
            tab += "    ";
        return tab;
    };

    auto onEnterBlock = [&](auto &b) {
        std::cout << tab() << "BlockValue : " << b.getName() << " {" << std::endl;
        level++;
    };

    auto onExitBlock = [&](auto &) {
        level--;
        std::cout << tab() << "}" << std::endl;
    };

    auto onEnterField = [&](auto &f) {
        std::cout << tab() << "FieldValue : " << f.getName() << " = " << f.getValue() << std::endl;
    };

    apply(value, onEnterBlock, onExitBlock, onEnterField, true);
}

class GetChildVisitor : public StructureVisitor
{
public:
    GetChildVisitor(std::string path) : path(path) {}

    void visit(const Block &block) override
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

    void visit(const GenericField &) override { result = nullptr; }

    std::string path;
    std::unique_ptr<Structure> const *result;
};

Structure &getChild(const Structure &structure, std::string path)
{
    GetChildVisitor visitor(path);
    structure.accept(visitor);

    if (visitor.result) {
        return *(visitor.result->get());
    }
    throw ChildNotFound(structure.getName(), path);
}

std::unique_ptr<StructureValue> with(Structure &structure, ValueBuilder builder)
{
    return structure.with(builder);
}

// Builders

std::unique_ptr<Field<float>> makeFloat(std::string name)
{
    return std::make_unique<Field<float>>(name);
}

std::unique_ptr<Field<int>> makeInteger(std::string name)
{
    return std::make_unique<Field<int>>(name);
}
}
