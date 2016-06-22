#include "importer/ListImporter.hpp"
#include "type/GenericField.hpp"
#include "value/GenericFieldValue.hpp"
#include "type/Block.hpp"

#include <cassert>

namespace structure
{
ListImporter::ListImporter(std::initializer_list<ValueInitializer> list)
{
    for (auto &value : list) {
        mImporters.emplace_back(new ValueInitializer(value));
    }
    iterator = begin(mImporters);
}

std::unique_ptr<GenericFieldValue> ListImporter::import(const GenericField &field,
                                                        const std::string &path)
{
    if (mDepth == 0) {
        // depth == 0 means that we haven't entered this block yet
        throw WrongType("Atom", "Block");
    }
    if (iterator == end(mImporters)) {
        // We have already reached the end of this block. There's no field left to import.
        throw NotEnoughValues(field.getName());
    }

    auto result = iterator->get()->import(field, path);
    if (mDepth == 1) {
        // depth > 1 means that we are importing fields that are more deeply nested; as a result we
        // must only increment this block's iterator when it is the active one.
        ++iterator;
    }
    return result;
}

void ListImporter::onEnterBlock(const std::string &block) try {
    if (begin(mImporters) == end(mImporters)) {
        // Special case for empty blocks
        ++mDepth;
        return;
    }
    if (iterator == end(mImporters)) {
        // we have already reached the end of this block. There's no block left to import.
        throw NotEnoughValues(block);
    }

    ++mDepth;
    if (mDepth > 1) {
        // depth > 1 means that we are importing fields that are more deeply nested; as a result we
        // must forward the onEnterBlock event.
        iterator->get()->onEnterBlock(block);
    }
} catch (ValueInitializer::WrongType &ex) {
    throw ValueStructureMismatch(block, "Expected : \"" + ex.required + "\", Actual : \"" +
                                            ex.actual + "\".");
}

void ListImporter::onExitBlock(const std::string &block)
{
    // This would be caused by a programming error in the Block class (or a descendant) calling
    // this method.
    assert(mDepth > 0);
    --mDepth;

    if ((iterator != end(mImporters)) and (mDepth > 1)) {
        iterator->get()->onExitBlock(block);
    }

    if (mDepth == 1) {
        // We are done importing a direct child, get ready to import its next sibling
        ++iterator;
    }

    if ((mDepth == 0) and (iterator != end(mImporters))) {
        throw ValueStructureMismatch(block, "Exiting a Block with leftover values");
    }
}
} // namespace structure
