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
    virtual ~Structure(){};

    virtual void accept(Visitor &visitor) = 0;

    std::string name();

    void setAttribute(std::string key, std::string value = "");
    std::map<std::string, std::string> const &getAttributes();

private:
    std::string mName;
    std::map<std::string, std::string> mAttributes;
};
}