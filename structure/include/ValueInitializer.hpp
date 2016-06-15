#pragma once

#include "Exception.hpp"

#include <list>
#include <string>

namespace structure
{

class ValueInitializer
{

public:
    struct WrongType : StructureException
    {
        WrongType(const std::string &required, const std::string &actual)
            : StructureException("Required : \"" + required + "\", Actual : \"" + actual + "\"."),
              required(required), actual(actual)
        {
        }

        const std::string required;
        const std::string actual;
    };

    ValueInitializer(const char *atomicValue) : mAtomicValue(atomicValue), mIsAtom(true) {}

    ValueInitializer(std::initializer_list<ValueInitializer> list) : mIsAtom(false)
    {
        for (auto value : list) {
            mListValue.push_back(value);
        }
    }

    const std::string &getAtomicValue() const
    {
        if (not mIsAtom) {
            throw WrongType("atom", "Block");
        }

        return mAtomicValue;
    }

    const std::list<ValueInitializer> &getListValue() const
    {
        if (mIsAtom) {
            throw WrongType("Block", "atom");
        }

        return mListValue;
    }

private:
    const std::string mAtomicValue;
    std::list<ValueInitializer> mListValue;
    bool mIsAtom;
};
}