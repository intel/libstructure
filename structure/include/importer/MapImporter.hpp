#pragma once

#include "ValueImporter.hpp"

#include <map>

namespace structure
{

/** An implementation of a ValueImporter that gets its values from a map
 *
 * @ingroup ValueImporter
 *
 */
class MapImporter : public ValueImporter
{

public:
    /**
     * @param[in]   values : A map of <path -> values> must contain an entry for each path in the
     * Structure
     */
    MapImporter(const std::map<std::string, std::string> &values) : mValues(values) {}

    std::unique_ptr<StructureValue> import(const GenericField &f, const std::string &path) override
    {
        std::string val = get(path);
        return f.genericWith(val.c_str());
    }

private:
    std::string get(const std::string &path) const { return mValues.at(path); }

    const std::map<std::string, std::string> mValues;
};
} // namespace structure