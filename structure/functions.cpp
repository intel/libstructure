/*
 * Copyright (c) 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Intel Corporation nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "structure/functions.hpp"
#include "structure/type/Block.hpp"
#include "structure/type/Field.hpp"
#include "structure/value/FieldValue.hpp"
#include "structure/value/BlockValue.hpp"
#include "structure/Exception.hpp"

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
                field.get().accept(*this);

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

void print(std::ostream &outStream, const Structure &structure)
{
    int level = 0;

    auto tab = [&]() {
        std::string tab;
        for (int i = 0; i < level; ++i)
            tab += "    ";
        return tab;
    };

    auto onEnterBlock = [&](auto &b) {
        outStream << tab() << b.getTypeName() << " : " << b.getName() << " {" << std::endl;
        level++;
    };

    auto onExitBlock = [&](auto &) {
        level--;
        outStream << tab() << "}" << std::endl;
    };

    auto onEnterField = [&](auto &f) {
        outStream << tab() << f.getTypeName() << " : " << f.getName() << std::endl;
    };

    apply(structure, onEnterBlock, onExitBlock, onEnterField, true);
}

void print(std::ostream &outStream, const std::unique_ptr<Block> &structure)
{
    print(outStream, *structure);
}

void print(std::ostream &outStream, const StructureValue &value)
{
    int level = 0;

    auto tab = [&]() {
        std::string tab;
        for (int i = 0; i < level; ++i)
            tab += "    ";
        return tab;
    };

    auto onEnterBlock = [&](auto &b) {
        outStream << tab() << "BlockValue : " << b.getName() << " {" << std::endl;
        level++;
    };

    auto onExitBlock = [&](auto &) {
        level--;
        outStream << tab() << "}" << std::endl;
    };

    auto onEnterField = [&](auto &f) {
        outStream << tab() << f.getStructure().getTypeName() << " : " << f.getName() << " = "
                  << f.getValue() << std::endl;
    };

    apply(value, onEnterBlock, onExitBlock, onEnterField, true);
}

void print(std::ostream &outStream, const std::unique_ptr<StructureValue> &value)
{
    print(outStream, *value);
}

std::string getValue(const StructureValue &value)
{
    std::string result;

    auto onEnterBlock = [&](auto &b) {
        result += "{";
        auto it = b.getFields().begin();
        while (it != b.getFields().end()) {
            result += getValue(*it);
            it++;
            if (it != b.getFields().end())
                result += ", ";
        }
        result += "}";
    };

    auto onExitBlock = [&](auto &) {

    };

    auto onEnterField = [&](auto &f) { result = f.getValue(); };

    apply(value, onEnterBlock, onExitBlock, onEnterField, false);

    return result;
}

std::string getValue(const std::unique_ptr<StructureValue> &value)
{
    return getValue(*value);
}

class GetChildStructureVisitor : public StructureVisitor
{
public:
    GetChildStructureVisitor(const std::string &path) : path(path), mResult(nullptr) {}

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
            if (field.get().getName() == name) {
                if (path.empty()) {
                    mResult = std::addressof(field.get());
                    return;
                } else {
                    field.get().accept(*this);
                }
            }
        }
    }

    void visit(const GenericField &) override {}

    const Structure *get() const { return mResult; }

private:
    std::string path;
    const Structure *mResult;
};

const Structure &getChild(const Structure &structure, const std::string &path)
{
    GetChildStructureVisitor visitor(path);
    structure.accept(visitor);

    if (visitor.get()) {
        return *(visitor.get());
    }
    throw ChildNotFound(structure.getName(), path);
}

const Structure &getChild(const std::unique_ptr<Block> &structure, const std::string &path)
{
    return getChild(*structure, path);
}

class GetChildValueVisitor : public ValueVisitor
{
public:
    GetChildValueVisitor(const std::string &path) : path(path), result(nullptr) {}

    void visit(const BlockValue &block) override
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

    void visit(const GenericFieldValue &) override {}

    std::string path;
    std::unique_ptr<StructureValue> const *result;
};

StructureValue &getChild(const StructureValue &value, const std::string &path)
{
    GetChildValueVisitor visitor(path);
    value.accept(visitor);

    if (visitor.result) {
        return *(visitor.result->get());
    }
    throw ChildNotFound(value.getName(), path);
}

StructureValue &getChild(const std::unique_ptr<StructureValue> &value, const std::string &path)
{
    return getChild(*value, path);
}

std::unique_ptr<StructureValue> with(const Block &block, ValueInitializer initializer)
{
    return block.with(initializer);
}

std::unique_ptr<StructureValue> build(const Structure &structure, ValueImporter &importer)
{
    return structure.build(importer);
}
}
