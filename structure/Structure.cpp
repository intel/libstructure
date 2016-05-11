#include "Structure.hpp"

namespace structure
{

Structure::Structure(std::string name) : mName(name)
{
}

std::string Structure::name()
{
    return mName;
}

void Structure::setAttribute(std::string key, std::string value)
{
    mAttributes[key] = value;
}

std::map<std::string, std::string> const &Structure::getAttributes()
{
    return mAttributes;
}
}