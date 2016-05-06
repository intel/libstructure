#pragma once

#include <initializer_list>
#include <string>

namespace structure
{

class Visitor;

class Structure
{
public:
    Structure(std::string name);
    virtual ~Structure(){};

    // virtual void with(std::initializer_list<std::string> values) = 0;
    virtual void accept(Visitor *visitor) = 0;

    virtual std::string name();

private:
    std::string mName;
};
}