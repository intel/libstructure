#pragma once

#include "structure_export.h"

#include <memory>

namespace structure
{

class StructureValue;
class Structure;
class ValueImporter;

/** Imports a StructureValue
 *
 * Use a ValueImporter to retrieve values and match them on the Structure
 *
 * @param[in]  structure : The Structure that the importer will try to match the values on
 * @param[in]  importer  : An implementation of a ValueImporter
 *
 * @return     Returns the imported StructureValue coresponding to the Structure passed in param
 */
STRUCTURE_EXPORT std::unique_ptr<StructureValue> importValue(const Structure &structure,
                                                             ValueImporter &importer);
}
