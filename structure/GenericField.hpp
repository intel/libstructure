#pragma once

namespace structure
{

class GenericField
{
public:
    GenericField(){};
    virtual ~GenericField(){};

    virtual std::string getName() = 0;
};
}