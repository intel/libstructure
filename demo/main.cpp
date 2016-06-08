#include "functions.hpp"
#include "client/stock.hpp"
#include "ValueBuilder.hpp"
#include "export.hpp"

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

    strc::UInt32 int32Type("SomeU32");
    auto valueFromInt = with(int32Type, 25);
    auto valueFromString = with(int32Type, "25");
    strc::print(std::cout, valueFromInt);
    strc::print(std::cout, valueFromString);

    strc::Float floatType("SomeFloat");
    auto valueFromMethod = with(floatType, 1.2);
    auto valueFromFreefunc = floatType.with("1.2");
    strc::print(std::cout, valueFromMethod);
    strc::print(std::cout, valueFromFreefunc);

    auto r = strc::with(root, {{"1.2", "3.4"}, "2", "0.2"});

    strc::print(std::cout, r);

    std::cout << std::endl;

    std::cout << exportStructure(root) << std::endl;
    std::cout << std::endl;
    std::cout << exportValue(r) << std::endl;

    return 0;
}
