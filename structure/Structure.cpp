#include "type/Structure.hpp"
#include "value/StructureValue.hpp"

namespace structure
{

std::string Structure::getName() const
{
    return mName;
}

void Structure::setMetadata(std::string key, std::string value)
{
    mMetadata[key] = value;
}

const std::map<std::string, std::string> &Structure::getMetadata()
{
    return mMetadata;
}

std::unique_ptr<StructureValue> Structure::build(ValueImporter &importer,
                                                 const std::string &path) const
{
    return doBuild(importer, path + "/" + getName());
}
}
