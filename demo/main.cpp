#include "functions.hpp"
#include "client/stock.hpp"
#include "export.hpp"
#include "importer/DefaultImporter.hpp"
#include "attributes/Default.hpp"

#include <iostream>
#include <exception>

namespace strc = structure;

int main(void)
{

    auto root =
        strc::Block("MyData", strc::UInt8("Counter", strc::attributes::Default(8)),
                    strc::Float("Double", strc::attributes::Default(6.28)), strc::Q32f31("Fixed"));

    strc::print(std::cout, root);

    std::cout << std::endl;

    auto value = root.with({strc::defaultImporter, strc::defaultImporter, 0.1});

    std::cout << std::endl;

    strc::print(std::cout, value);

    std::cout << std::endl;

    return 0;
}
