#include "functions.hpp"
#include "ValueBuilder.hpp"

#include <iostream>
#include <exception>

namespace strc = structure;

int main(void)
{

    auto root = strc::makeBlock(
        "root", strc::makeBlock("complex", strc::makeFloat("re"), strc::makeFloat("im")),
        strc::makeInteger("count"));

    strc::display(root);

    std::cout << std::endl;

    auto r = strc::with(root, {{"1.2", "3.4"}, "2"});

    strc::display(r);

    std::cout << std::endl;

    std::cout << strc::getValue(strc::getChild(r, "complex")) << std::endl;

    return 0;
}
