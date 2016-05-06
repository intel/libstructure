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
}