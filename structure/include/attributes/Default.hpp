#pragma once

#include "ValueInitializer.hpp"

namespace structure
{
namespace attributes
{

/** Holds a ValueImporter that can be used by the DefaultImporter to create the Value
 * It acts as a default value.
 */
struct Default
{
    /**
     * @param[in]  value  The value or importer to be used as default value
     */
    Default(const ValueInitializer &value) : mDefaultImporter(new ValueInitializer(value)) {}

    std::shared_ptr<ValueImporter> mDefaultImporter;
};
}
}