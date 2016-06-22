#pragma once

#include "structure_export.h"
#include "ValueImporter.hpp"

#include <initializer_list>
#include <memory>

namespace structure
{

class GenericField;
class GenericFieldValue;

class STRUCTURE_EXPORT ValueInitializer : public ValueImporter
{
public:
    /** Constructs a ValueInitializer from a string literal
     *
     * It should coincide with a Field.
     */
    ValueInitializer(const char *atomicValue);

    /** Constructs a ValueInitializer from an previously-defined importer
     *
     * This should coincide with a Block.
     */
    ValueInitializer(ValueImporter &importer);

    /** Constructs a ValueInitializer from a recursive initializer list
     *
     * This should coincide with a Block.
     *
     * Example 1:
     * @code
     * PromptImporter<> importer;
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * structure.with({importer, "1", "2"});
     * @endcode
     *
     * Example 2:
     * @code
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * ValueInitializer value = {{"1", "2"}, "3", "4"};
     * structure.with(value);
     * @endcode
     */
    ValueInitializer(std::initializer_list<ValueInitializer> list);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;
    void onEnterBlock(const std::string &block) override;
    void onExitBlock(const std::string &block) override;

private:
    // This class needs to be copiable because instances will be copied from initializer_list to
    // list when they are part of a ListImporter. (Copied and not moved because items of a
    // initializer_list can't be moved). That's why we use a shared_ptr.
    std::shared_ptr<ValueImporter> mImporter;
};
}
