#include "Block.hpp"
#include "Field.hpp"
#include "functions.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    structure::Block root("root",
                          {new structure::Field<float>{"a"}, new structure::Field<float>{"b"},
                           new structure::Block("c", {new structure::Field<float>{"d"},
                                                      new structure::Field<float>{"f"}})});

    structure::display(&root);

    std::cout << std::endl;

    auto c = structure::getChild(&root, "c");
    if (c)
        for (auto child : structure::getChildren(c))
            structure::display(child);

    std::cout << std::endl;

    structure::display(structure::getChild(&root, "/c/f"));

    return 0;
}