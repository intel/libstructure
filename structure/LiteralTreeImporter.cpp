#include "importer/LiteralTreeImporter.hpp"
#include "type/GenericField.hpp"
#include "type/Block.hpp"
#include "value/GenericFieldValue.hpp"
#include "Exception.hpp"

namespace structure
{

LiteralTreeImporter::LiteralTreeImporter(ValueInitializer initializer)
{
    initializerRoot.push_back(initializer);
    iterator = initializerRoot.begin();
    iteratorEnd = initializerRoot.end();
}

std::unique_ptr<GenericFieldValue> LiteralTreeImporter::import(const GenericField &field,
                                                               const std::string & /*path*/) try {
    if (not isIteratorValid()) {
        throw NotEnoughValues(field.getName());
    }

    auto value = field.with(iterator->getAtomicValue());
    ++iterator;

    return value;
} catch (ValueInitializer::WrongType &ex) {
    throw ValueStructureMismatch(field.getName(), "Expected : \"" + ex.required +
                                                      "\", Actual : \"" + ex.actual + "\".");
}

void LiteralTreeImporter::onEnterBlock(const Block &block) try {
    if (not isIteratorValid()) {
        throw NotEnoughValues(block.getName());
    }

    iteratorEndStack.push_back(iteratorEnd);
    iteratorEnd = iterator->getListValue().cend();

    iteratorStack.push_back(iterator);
    iterator = iterator->getListValue().cbegin();
} catch (ValueInitializer::WrongType &ex) {
    throw ValueStructureMismatch(block.getName(), "Expected : \"" + ex.required +
                                                      "\", Actual : \"" + ex.actual + "\".");
}

void LiteralTreeImporter::onExitBlock(const Block &block)
{
    if (isIteratorValid()) {
        throw ValueStructureMismatch(block.getName(), "Exiting a Block with leftover values");
    }

    iterator = iteratorStack.back();
    iteratorStack.pop_back();
    ++iterator;

    iteratorEnd = iteratorEndStack.back();
    iteratorEndStack.pop_back();
}
} // namespace structure