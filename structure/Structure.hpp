#pragma once

#include "ValueBuilder.hpp"

#include <initializer_list>
#include <string>
#include <map>
#include <memory>

namespace structure
{

class StructureValue;
class StructureVisitor;

class Structure
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
}
