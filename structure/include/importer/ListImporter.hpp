#pragma once

#include "structure_export.h"
#include "ValueImporter.hpp"
#include "ValueInitializer.hpp"

#include <initializer_list>
#include <list>
#include <memory>

namespace structure
{
/** Allows ValueInitializer to be created using recursive initializer lists
 *
 * Thanks to this class, a ValueInitializer can contain an arbitrary level of nested blocks.
 */
class STRUCTURE_EXPORT ListImporter : public ValueImporter
{
public:
    /** @see ValueInitializer(std::initializer_list<ValueInitializer> */
    ListImporter(std::initializer_list<ValueInitializer> list);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;

    void onEnterBlock(const std::string &block) override;
    void onExitBlock(const std::string &block) override;

private:
    // This class needs to be copiable because importers will be copied from initializer_list to
    // list when they are part of a ListImporter. (Copied and not moved because items of a
    // initializer_list can't be moved). That's why we use a shared_ptr.
    std::list<std::shared_ptr<ValueImporter>> mImporters;
    using Iterator = decltype(mImporters)::const_iterator;

    Iterator iterator;
    size_t mDepth = 0;
};
}
