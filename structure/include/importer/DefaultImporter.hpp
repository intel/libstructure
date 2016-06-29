#pragma once

#include "structure_export.h"

#include "ValueImporter.hpp"
#include "type/GenericField.hpp"

namespace structure
{

/** An importer that uses the Default attribute in a field to build it
 *  and throws an exception if the field does have a Default attribute.
 */
class DefaultImporter : public ValueImporter
{
public:
    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override
    {
        if (field.hasDefaultImporter()) {
            return field.getDefaultImporter().import(field, path);
        }
        throw NoDefaultValue(field.getName());
    }

    void onEnterBlock(const std::string &) override { throw WrongType("Block", "Atom"); }

    void onExitBlock(const std::string &) override { throw WrongType("Block", "Atom"); }
};

/** Provide a global instance of a DefaultImporter for convenience
 */
STRUCTURE_EXPORT extern DefaultImporter defaultImporter;

} // namespace structure