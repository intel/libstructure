#include "functions.hpp"
#include "client/stock.hpp"
#include "export.hpp"
#include "importer/PromptImporter.hpp"

#include <iostream>
#include <exception>

namespace strc = structure;

int main(void)
{

    auto root =
        strc::Block("MyData", strc::UInt8("Counter"), strc::Float("Double"), strc::Q32f31("Fixed"));

    strc::print(std::cout, root);

    std::cout << std::endl;

    auto value = root.with({255u, 3.14f, 0.1});

    std::cout << std::endl;

    strc::print(std::cout, value);

    std::cout << std::endl;

    return 0;
}
