#pragma once

/** @file
 *
 * Defines several exception classes for libstructure
 */

#include <stdexcept>
#include <string>

namespace structure
{
/** Base class for all libstructure exceptions */
class StructureException : public std::runtime_error
{
    using runtime_error::runtime_error;
};

/** Thrown when trying to access an unexisting child of a block */
class ChildNotFound : public StructureException
{
public:
    ChildNotFound(const std::string &self, const std::string &child)
        : StructureException("'" + self + "': no such child: '" + child + "'.")
    {
    }
};

/** Thrown when trying to get a Field as a Block */
class NotABlock : public StructureException
{
public:
    NotABlock(const std::string &self) : StructureException("'" + self + "' is not a Block.") {}
};

/** Thrown when instantiating a StructureValue with an incorrect layout */
class ValueStructureMismatch : public StructureException
{
public:
    ValueStructureMismatch(const std::string &self, const std::string &details)
        : StructureException("Instanciation values do not match with the Structure '" + self +
                             "' (" + details + ").")
    {
    }
};

/** Thrown when a user input cannot be converted into the required type */
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
