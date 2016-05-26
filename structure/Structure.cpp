#include "Structure.hpp"
#include "StructureValue.hpp"

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
std::unique_ptr<StructureValue> Structure::with(ValueBuilder builder) const
{
    return genericWith(builder);
};
}
