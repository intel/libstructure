#pragma once

#include "structure_export.h"
#include "Exception.hpp"

#include <string>
#include <memory>

namespace structure
{

class GenericField;
class StructureValue;
class Block;
class GenericFieldValue;

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

    virtual ~ValueImporter() = default;

    /**
     * @param[in]  field : The structure of the value beign built
     * @param[in]  path  : The path of the field in the structure
     *
     * @return     The value coresponding to the field
     */
    virtual std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                                      const std::string &path) = 0;

    // We need to write the name of the parameter in the declaration, so that Doxygen will know it
    // but in the same time, that would trigger an 'unused parameter' warning.
    /** Notify the importer that the caller wants to begin importing a sub-block
     *
     * @param[in] block The type of block being imported
     */
    virtual void onEnterBlock(const Block &block) { (void)block; }
    /** Notify the importer that the caller is done importing a sub-block
     *
     * @param[in] block The type of block being imported
     */
    virtual void onExitBlock(const Block &block) { (void)block; }
};
}
