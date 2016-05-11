#include "Exception.hpp"

namespace structure
{

StructureException::StructureException(ExceptionType type, const std::string &error) throw()
    : mType(type)
{
    switch (mType) {
    case ExceptionType::ChildNotFound:
        mError = "Child '" + error + "' not found";
        break;

    case ExceptionType::NotABlock:
        mError = error + " can only be called on a Block";
        break;

    default:
        mError = error;
        break;
    }
}

const char *StructureException::what() const throw()
{
    return mError.c_str();
}

ExceptionType StructureException::getType() const
{
    return mType;
}
}