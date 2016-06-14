#include "functions.hpp"
#include "client/stock.hpp"
#include "ValueBuilder.hpp"
#include "export.hpp"
#include "ValueImporter.hpp"
#include "importer/MapImporter.hpp"

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

    std::map<std::string, std::string> values = {{"/MyData/Complex/Real", "1.2"},
                                                 {"/MyData/Complex/Imaginary", "3.4"},
                                                 {"/MyData/Counter", "5"},
                                                 {"/MyData/Fixed", "0.12345"}};
    auto importer = strc::MapImporter(values);
    auto value = strc::with(root, importer);

    std::cout << std::endl;

    strc::print(std::cout, value);

    std::cout << std::endl;

    return 0;
}
