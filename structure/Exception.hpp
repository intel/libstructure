#pragma once

#include <exception>
#include <string>

namespace structure
{

class StructureException : public std::exception
{
};

class ChildNotFound : public StructureException
{
public:
    ChildNotFound(const std::string &self, const std::string &child)
        : mError("'" + self + "': no such child: '" + child + "'.")
    {
    }

    const char *what() const noexcept override { return mError.c_str(); }

private:
    std::string mError;
};

class NotABlock : public StructureException
{
public:
    NotABlock(const std::string &self) : mError("'" + self + "' is not a Block.") {}

    const char *what() const noexcept override { return mError.c_str(); }

private:
    std::string mError;
};

class ValueStructureMismatch : public StructureException
{
public:
    ValueStructureMismatch(const std::string &self)
        : mError("Instanciation values do not match with the Structure '" + self + "'")
    {
    }

    const char *what() const noexcept override { return mError.c_str(); }

private:
    std::string mError;
};
}
