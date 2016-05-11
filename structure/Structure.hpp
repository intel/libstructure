#pragma once

#include <initializer_list>
#include <string>
#include <map>

namespace structure
{

class Visitor;

class Structure
{
public:
    Structure(std::string name);
    virtual ~Structure() = default;

    virtual void accept(Visitor &visitor) = 0;

    std::string getName();

    void setAttribute(std::string key, std::string value = "");
    const std::map<std::string, std::string> &getAttributes();

private:
    std::string mName;
    std::map<std::string, std::string> mAttributes;
};
}
