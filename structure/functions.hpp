#pragma once

#include <string>
#include <list>

class Structure;

namespace structure
{

void display(Structure *structure);

Structure *getChild(Structure *structure, std::string path);
std::list<Structure *> getChildren(Structure *structure);
}