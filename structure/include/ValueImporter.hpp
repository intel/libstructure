#pragma once

#include "structure_export.h"
#include "type/GenericField.hpp"

namespace structure
{

/**
 * @defgroup ValueImporter Allow to create StructureValue from Structure and formatted values
 */

/** An abstract class that represent a value importer
 *
 * @ingroup ValueImporter
 */
class STRUCTURE_EXPORT ValueImporter
{
public:
    virtual ~ValueImporter() = default;

    /**
     * @param[in]  field : The structure of the value beign built
     * @param[in]  path  : The path of the field in the structure
     *
     * @return     The value coresponding to the field
     */
    virtual std::unique_ptr<StructureValue> import(const GenericField &field,
                                                   const std::string &path) = 0;
};
}