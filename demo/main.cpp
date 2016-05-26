#include "functions.hpp"
#include "StockTypes.hpp"
#include "ValueBuilder.hpp"
#include "export.hpp"

#include <iostream>
#include <exception>

namespace strc = structure;

int main(void)
{

    auto root =
        strc::Block("MyData", strc::Block("Complex", strc::Float("Real"), strc::Float("Imaginary")),
                    strc::Integer("Counter"));

    strc::display(root);

    std::cout << std::endl;

    auto r = strc::with(root, {{"1.2", "3.4"}, "2"});

    strc::display(r);

    std::cout << std::endl;

    std::cout << strc::exportStructure(root) << std::endl;

    return 0;
}
