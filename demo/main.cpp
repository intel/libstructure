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
        strc::Block("MyData", strc::Block("Complex", strc::Float("Real"), strc::Float("Imaginary")),
                    strc::UInt32("Counter"), strc::Q32f31("Fixed"));

    strc::print(std::cout, root);

    std::cout << std::endl;

    strc::PromptImporter<> importer;
    auto value = root.with({importer, "5", "0.1"});

    std::cout << std::endl;

    strc::print(std::cout, value);

    std::cout << std::endl;

    return 0;
}
