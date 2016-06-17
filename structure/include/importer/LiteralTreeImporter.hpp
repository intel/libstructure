#pragma once

#include "structure_export.h"
#include "ValueImporter.hpp"
#include "ValueInitializer.hpp"

#include <string>
#include <iostream>
#include <memory>

namespace structure
{

class STRUCTURE_EXPORT LiteralTreeImporter : public ValueImporter
{

public:
    LiteralTreeImporter(ValueInitializer initializer);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;

    void onEnterBlock(const Block &block) override;

    void onExitBlock(const Block &block) override;

private:
    std::list<ValueInitializer> initializerRoot;

    using ValueInitializerIterator = std::list<ValueInitializer>::const_iterator;
    ValueInitializerIterator iterator;
    ValueInitializerIterator iteratorEnd;
    std::list<ValueInitializerIterator> iteratorStack;
    std::list<ValueInitializerIterator> iteratorEndStack;

    bool isIteratorValid() { return iterator != iteratorEnd; }
};
}
