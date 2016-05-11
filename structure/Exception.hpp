#pragma once

#include <exception>
#include <string>

namespace structure
{

enum class ExceptionType
{
    ChildNotFound,
    NotABlock,
};

class StructureException : public std::exception
{
public:
    StructureException(ExceptionType type, const std::string &error) throw();

    virtual const char *what() const throw();

    ExceptionType getType() const;

    virtual ~StructureException() throw() {}

private:
    std::string mError;
    ExceptionType mType;
};
}