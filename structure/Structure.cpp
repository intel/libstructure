#include "Structure.hpp"

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
}
