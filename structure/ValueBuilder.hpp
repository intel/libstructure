#pragma once

#include <string>
#include <list>
#include <iostream>

namespace structure
{

class ValueBuilder
{

public:
    ValueBuilder(const char *atomicValue) : atomicValue(atomicValue), atom(true) {}

    ValueBuilder(std::initializer_list<ValueBuilder> list) : atom(false)
    {
        for (auto value : list)
            listValue.push_back(value);
    }

    std::string atomicValue;
    std::list<ValueBuilder> listValue;
    bool atom;
};
}