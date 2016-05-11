#pragma once

namespace structure
{

class GenericField
{
public:
    GenericField(){};
    virtual ~GenericField(){};

    virtual std::string fieldName() = 0;
    virtual std::string fieldType() = 0;
};
}