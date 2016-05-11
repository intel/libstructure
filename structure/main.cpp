#include "Block.hpp"
#include "Field.hpp"
#include "functions.hpp"

#include <iostream>
#include <exception>

int main(int argc, char **argv)
{

    auto root = structure::makeBlock(
        "a", structure::makeFloat("b"),
        structure::makeBlock("c", structure::makeInteger("e"), structure::makeBlock("f")));

    structure::display(root);

    root->setAttribute("toto", "true");

    try {
        structure::getChildren(structure::getChild(root, "c"));
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }

    for (auto &a : root->getAttributes())
        std::cout << a.first << " : " << a.second << std::endl;

    std::cout << std::endl;

    return 0;
}