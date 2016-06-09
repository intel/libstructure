#pragma once

#include "structure_export.h"

#include "ValueBuilder.hpp"

#include <initializer_list>
#include <string>
#include <map>
#include <memory>

namespace structure
{

class StructureValue;
class StructureVisitor;

class STRUCTURE_EXPORT Structure
{
public:
    Structure(std::string name);
    virtual ~Structure() = default;

    virtual void accept(StructureVisitor &visitor) const = 0;

    std::string getName() const;
    virtual std::string getTypeName() const = 0;

    void setAttribute(std::string key, std::string value = "");
    const std::map<std::string, std::string> &getAttributes();

    std::unique_ptr<StructureValue> with(ValueBuilder builder) const;

private:
    virtual std::unique_ptr<StructureValue> genericWith(ValueBuilder builder) const = 0;
    std::string mName;
    std::map<std::string, std::string> mAttributes;
};

/** Helper type trait for checking that a type is a Structure
 *
 * It helps preventing complicated error messages further down the stack.
 *
 * @tparam T The type to be checked against Structure.
 */
template <typename T>
struct is_structure : std::is_base_of<Structure, T>
{
};
}
