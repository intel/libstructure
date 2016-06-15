#include "type/Structure.hpp"
#include "value/StructureValue.hpp"

namespace structure
{

Structure::Structure(std::string name) : mName(name)
{
}

std::string Structure::getName() const
{
    return mName;
}

void Structure::setAttribute(std::string key, std::string value)
{
    mAttributes[key] = value;
}

const std::map<std::string, std::string> &Structure::getAttributes()
{
    return mAttributes;
}

std::unique_ptr<StructureValue> Structure::build(ValueImporter &importer,
                                                 const std::string &path) const
{
    return doBuild(importer, path + "/" + getName());
}
}
