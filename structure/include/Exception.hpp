#pragma once

#include <stdexcept>
#include <string>

namespace structure
{

class StructureException : public std::runtime_error
{
    using runtime_error::runtime_error;
};

class ChildNotFound : public StructureException
{
public:
    ChildNotFound(const std::string &self, const std::string &child)
        : StructureException("'" + self + "': no such child: '" + child + "'.")
    {
    }
};

class NotABlock : public StructureException
{
public:
    NotABlock(const std::string &self) : StructureException("'" + self + "' is not a Block.") {}
};

class ValueStructureMismatch : public StructureException
{
public:
    ValueStructureMismatch(const std::string &self, const std::string &details)
        : StructureException("Instanciation values do not match with the Structure '" + self +
                             "' (" + details + ").")
    {
    }
};

class ParseError : public StructureException
{
public:
    // TODO: make it possible to pass a type name
    ParseError(const std::string &input)
        : StructureException("Incompatible value \"" + input + "\".")
    {
    }
};
}
